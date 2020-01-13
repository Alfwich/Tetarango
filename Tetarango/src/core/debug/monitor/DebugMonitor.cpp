#include "DebugMonitor.h"

#include "ui/renderable/container/ScrollContainer.h"
#include "ui/scene/SceneContainer.h"

namespace
{
	const auto updateThreshold = 125;
}

namespace AW
{
	DebugMonitor::DebugMonitor()
	{
		setShouldRebuildOnLoad();
		renderPositionMode = AW::RenderPositionMode::Absolute;
		markIsDebugElement();
		GORegister(DebugMonitor);
	}

	std::string DebugMonitor::frameTimeToFPSString(const double & time)
	{
		const auto fpsInt = (int)std::round(time);
		return std::to_string(fpsInt);
	}

	void DebugMonitor::updateFrameTimeInfo(const double& frameTime)
	{
		frameTimes[frameTimeI] = frameTime;

		if (frameTimeI + 1 == (int)frameTimes.size())
		{
			hasHydrated = true;
		}
		frameTimeI = (frameTimeI + 1) % (int)frameTimes.size();

		if (updateTimer->getTicks() >= updateThreshold)
		{
			if (hasHydrated)
			{
				auto time = 0.0;

				for (const auto sample : frameTimes)
				{
					time += sample;
				}

				fpsText->setText("FPS:" + frameTimeToFPSString(1000.0 / (time / frameTimes.size())));
			}
			else
			{
				fpsText->setText("FPS:---");
			}

			frameTimeText->setText("FT:" + std::to_string((int)std::floor(frameTimes[frameTimeI])));
		}
	}

	void DebugMonitor::updateSceneGraphRepresentation()
	{
		textIdToSceneId.clear();
		sceneGraphInfoContainer->rebuild();

		const auto parentPtr = parent.lock();
		if (parentPtr != nullptr)
		{
			updateSceneGraphRepresentationRecursive(parentPtr);
			sceneGraphInfoContainer->resizeSelfToChildrenAndCenterChildren();
		}
	}

	void DebugMonitor::updateSceneGraphRepresentationRecursive(std::shared_ptr<GameObject> obj, int depth)
	{
		const auto objNameLabel = std::make_shared<Text>();
		const auto typeName = obj->getSchematic()->typeName;
		const auto depthString = AW::StringHelper::repeat(' ', depth);
		textIdToSceneId[objNameLabel->getId()] = obj->getId();
		objNameLabel->setTextColor(255, 158, 0);
		const auto renderablePtr = std::dynamic_pointer_cast<Renderable>(obj);
		if (renderablePtr != nullptr && (renderablePtr->getAlpha() == 0.0 || !renderablePtr->visible))
		{
			objNameLabel->setTextColor(128, 79, 0);
		}
		objNameLabel->renderTextureMode = RenderTextureMode::LinearNoWrap;
		objNameLabel->setFont("console", 28);
		objNameLabel->setText(depthString + typeName);
		if (sceneGraphAligningElement != nullptr)
		{
			objNameLabel->toBottomLeftOf(sceneGraphAligningElement);
		}
		else
		{
			objNameLabel->topLeftAlignSelf();
		}

		sceneGraphInfoContainer->add(objNameLabel);
		sceneGraphAligningElement = objNameLabel;

		if (!obj->name.empty())
		{
			addInfoLineToSceneGraph(depthString, "Name", obj->name, obj->getId());
		}

		if (renderablePtr != nullptr && renderablePtr->getColor() != nullptr)
		{
			const auto color = renderablePtr->getColor();
			const auto colorValue = "(" + std::to_string(color->r) + ", " + std::to_string(color->g) + ", " + std::to_string(color->b) + ", " + std::to_string(color->a) + ")";
			addInfoLineToSceneGraph(depthString, "Color", colorValue, obj->getId());

			const auto rect = std::make_shared<Rectangle>();
			rect->setColor(AW::Color::black());
			rect->setScreenSize(30.0, 30.0);
			rect->toRightOf(sceneGraphAligningElement, 10.0, 0.0);
			sceneGraphInfoContainer->add(rect);

			const auto rect2 = std::make_shared<Rectangle>();
			rect2->setColor(color);
			rect2->setScreenSize(20.0, 18.0);
			rect2->centerWithin(rect);
			sceneGraphInfoContainer->add(rect2);
		}

		const auto textPtr = std::dynamic_pointer_cast<Text>(obj);
		if (textPtr != nullptr)
		{
			addInfoLineToSceneGraph(depthString, "Text", '"' + textPtr->getText() + '"', obj->getId());
		}

		const auto sceneContainerPtr = std::dynamic_pointer_cast<SceneContainer>(obj);
		if (sceneContainerPtr != nullptr)
		{
			std::string sceneNames = "";
			for (const auto scene : sceneContainerPtr->getAvailableScenes())
			{
				if (!sceneNames.empty())
				{
					sceneNames += ", " + scene;
				}
				else
				{
					sceneNames += scene;
				}
			}
			addInfoLineToSceneGraph(depthString, "Available Scenes", sceneNames, obj->getId());
			addInfoLineToSceneGraph(depthString, "Current Scene", sceneContainerPtr->getCurrentSceneName(), obj->getId());
		}

		const auto renderChildren = obj->getChildrenOrdered();
		if (!renderChildren.empty())
		{
			if (std::find(allowedCrawlElements.begin(), allowedCrawlElements.end(), obj->getId()) == allowedCrawlElements.end())
			{
				const auto formatLabel = std::make_shared<Text>();
				textIdToSceneId[formatLabel->getId()] = obj->getId();
				formatLabel->setFont("console", 20);
				formatLabel->renderTextureMode = RenderTextureMode::LinearNoWrap;
				formatLabel->setTextColor(128, 128, 128);
				formatLabel->setText("(" + std::to_string(renderChildren.size()) + " children)");
				formatLabel->toRightOf(objNameLabel, 10.0, 0.0);

				sceneGraphInfoContainer->add(formatLabel);
			}
			else
			{
				for (const auto child : renderChildren)
				{
					updateSceneGraphRepresentationRecursive(child, depth + 1);
				}
			}
		}
	}

	void DebugMonitor::addInfoLineToSceneGraph(const std::string& depthStr, std::string propName, std::string value, int objId)
	{
		auto infoLabel = std::make_shared<Text>();
		textIdToSceneId[infoLabel->getId()] = objId;
		infoLabel->renderTextureMode = RenderTextureMode::LinearNoWrap;
		infoLabel->setFont("console", 28);
		infoLabel->setText(depthStr + " " + propName + "=" + value);
		infoLabel->toBottomLeftOf(sceneGraphAligningElement);
		sceneGraphInfoContainer->add(infoLabel);
		sceneGraphAligningElement = infoLabel;
	}

	void DebugMonitor::checkIfHovering()
	{
		auto rect = sceneGraphInfoContainer->getScreenRect();

		const auto x = modules->input->mouse->X();
		const auto y = modules->input->mouse->Y();

		isHovering =
			x < rect->x + rect->w && x > rect->x &&
			y < rect->y + rect->h && y > rect->y;
	}

	void DebugMonitor::onInitialAttach()
	{
		clear();

		updateTimer = modules->time->createPureTimer();
		updateTimer->start();

		modules->input->mouse->registerMouseButton(AWMouseButton::Left, shared_from_this());
		modules->input->mouse->registerMouseWheel(shared_from_this());
		enableEnterFrame(10);
	}

	void DebugMonitor::onAttach()
	{
		updateSceneGraphRepresentation();
	}

	void DebugMonitor::onCreateChildren()
	{
		sceneGraphInfoContainer = std::make_shared<Container>();
		add(sceneGraphInfoContainer);

		fpsText = std::make_shared<Text>();
		fpsText->renderTextureMode = RenderTextureMode::LinearNoWrap;
		fpsText->setFont("console", 40);
		fpsText->setTextColor(105, 255, 105);
		add(fpsText);

		frameTimeText = std::make_shared<Text>();
		frameTimeText->renderTextureMode = RenderTextureMode::LinearNoWrap;
		frameTimeText->setFont("console", 40);
		frameTimeText->setTextColor(105, 255, 105);
		add(frameTimeText);
	}

	void DebugMonitor::onLayoutChildren()
	{
		fpsText->topLeftAlignSelf();
		frameTimeText->toBottomLeftOf(fpsText);

		sceneGraphInfoContainer->toBottomLeftOf(frameTimeText, 0.0, sceneGraphYOffset);

		if (sceneGraphInfoContainer->getScreenRect()->y + sceneGraphInfoContainer->getScreenRect()->h <= 0.0)
		{
			sceneGraphYOffset = 0.0;
			sceneGraphInfoContainer->toBottomLeftOf(frameTimeText, 0.0, sceneGraphYOffset);
		}
	}

	void DebugMonitor::onEnterFrame(const double & frameTime)
	{
		checkIfHovering();
		updateFrameTimeInfo(frameTime);

		if (updateTimer->isAboveThresholdAndRestart(updateThreshold))
		{
			layout();
		}
	}

	void DebugMonitor::onMouseButtonLeftUp()
	{
		if (!isHovering)
		{
			return;
		}

		const auto mouseY = modules->input->mouse->Y();
		for (const auto child : sceneGraphInfoContainer->getChildrenOfType<Text>())
		{
			auto screenRect = child->getScreenRect();
			const auto textId = child->getId();
			if (screenRect->y < mouseY && screenRect->y + screenRect->h > mouseY && textIdToSceneId.count(textId) == 1)
			{
				const auto sceneEleId = textIdToSceneId.at(textId);
				const auto itr = std::find(allowedCrawlElements.begin(), allowedCrawlElements.end(), sceneEleId);
				if (itr == allowedCrawlElements.end())
				{
					allowedCrawlElements.push_back(textIdToSceneId.at(textId));
				}
				else
				{
					allowedCrawlElements.erase(itr);
				}

				updateSceneGraphRepresentation();
				layout();
				return;
			}
		}
	}

	void DebugMonitor::onMouseWheel(int x, int y)
	{
		if (!isHovering)
		{
			return;
		}

		sceneGraphYOffset += y * 28;
		layout();
	}

	void DebugMonitor::clear()
	{
		textIdToSceneId.clear();
		allowedCrawlElements.clear();

		if (sceneGraphInfoContainer != nullptr)
		{
			sceneGraphInfoContainer->destroyChildren();
			updateSceneGraphRepresentation();
		}

		frameTimes.clear();

		for (auto i = 0; i < totalFrameTimeSamplesToKeep; ++i)
		{
			frameTimes.push_back(0.0);
		}

		hasHydrated = false;
		frameTimeI = 0;
		sceneGraphYOffset = 0;
	}
}