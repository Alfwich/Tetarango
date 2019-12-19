#include "Animated.h"
#include "ui/renderable/container/Container.h"

namespace AW
{

	Animated::Animated()
	{
		frame = 0;
		fpsOffset = 0;
		registerSerialization<Animated>();
	}

	void Animated::updateFrame(double frameTime)
	{
		int targetFps = currentAnimation->getFps() + fpsOffset;

		if (playing && targetFps > 0 && currentAnimation != nullptr && frameTimer->getTicks() >= (1000.0 / targetFps))
		{
			frame = currentAnimation->frameInAnimation(frame + 1) ? frame + 1 : 0;

			frameTimer->start();
		}
	}

	void Animated::setAnimationSet(std::shared_ptr<AnimationSet> animationSet, std::string setName)
	{
		this->animationSet = animationSet;
		this->currentAnimationSetName = setName;

		if (currentAnimationName != "")
		{
			pause();
			play(currentAnimationName);
		}
	}

	void Animated::setAnimationSet(std::string animationSetName)
	{
		std::shared_ptr<AnimationSet> animationSet = modules->animation->getAnimationSet(animationSetName);
		setAnimationSet(animationSet, animationSetName);
	}

	void Animated::play(std::string animationName)
	{
		if (!prefixHasChanged && animationName != "" && (animationName == currentAnimationName && frameTimer->isRunning()) && !paused)
		{
			return;
		}

		setCurrentAnimation(animationName);

		if (!paused)
		{
			frame = 0;
		}

		playing = true;
		paused = false;

		if (frameTimer != nullptr)
		{
			frameTimer->start();
		}
	}

	void Animated::pause()
	{
		playing = false;
		paused = true;
	}

	void Animated::stop()
	{
		playing = false;
	}

	bool Animated::isPlaying()
	{
		return playing;
	}

	bool Animated::isPaused()
	{
		return paused;
	}

	void Animated::setCurrentAnimation(std::string animationName)
	{
		if (animationSet == nullptr) return;

		currentAnimationName = animationName;
		currentAnimation = animationSet->getAnimation(animationPrefix + animationName);
		prefixHasChanged = false;

		if (sizeToAnimation && currentAnimation != nullptr)
		{
			const auto clipRect = getTextureClipRect();
			setSize(clipRect->w, clipRect->h);
		}
	}

	std::string Animated::getCurrentAnimationName()
	{
		return currentAnimationName;
	}

	std::shared_ptr<Rect> Animated::getTextureClipRect()
	{
		return currentAnimation != nullptr ? currentAnimation->getFrameRect(frame) : NULL;
	}

	void Animated::enterFrame(const double& frameTime)
	{
		if (currentAnimation != nullptr && frameTimer != nullptr)
		{
			updateFrame(frameTime);
		}

		GameObject::enterFrame(frameTime);
	}

	void Animated::onInitialAttach()
	{
		frameTimer = modules->time->createTimer(getTimeScope(), true);

		if (!currentAnimationSetName.empty())
		{
			setAnimationSet(currentAnimationSetName);
		}
	}

	void Animated::onAttach()
	{
		frameTimer->setTimeScope(getTimeScope());

		if (!currentAnimationName.empty())
		{
			play(currentAnimationName);
		}
		else if (!defaultAnimationName.empty())
		{
			play(defaultAnimationName);
		}
	}

	void Animated::onTimeScopeChanged()
	{
		if (frameTimer == nullptr)
		{
			return;
		}

		frameTimer->setTimeScope(getTimeScope());
	}

	std::shared_ptr<SerializationClient> Animated::doSerialize(SerializationHint hint)
	{
		const auto client = serializationClient->getClient("__animated__", hint);
		currentAnimationName = client->serializeString("currentAN", currentAnimationName);
		currentAnimationSetName = client->serializeString("currentASN", currentAnimationSetName);
		playing = client->serializeBool("isPlaying", playing);
		paused = client->serializeBool("isPaused", paused);
		frame = client->serializeInt("frame", frame);
		fpsOffset = client->serializeInt("fpsOffset", fpsOffset);
		sizeToAnimation = client->serializeBool("sizeToAnimation", sizeToAnimation);
		animationPrefix = client->serializeString("animationPrefix", animationPrefix);

		return Element::doSerialize(hint);
	}

	void Animated::setFpsOffset(int fpsOffset)
	{
		this->fpsOffset = fpsOffset;
	}

	void Animated::setDefaultAnimationName(std::string name)
	{
		defaultAnimationName = name;
	}

	void Animated::setAnimationPrefix(std::string prefix)
	{
		if (animationPrefix != prefix)
		{
			animationPrefix = prefix;
			prefixHasChanged = true;
		}
	}
}
