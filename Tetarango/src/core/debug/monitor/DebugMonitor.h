#pragma once

#include "ui/renderable/container/Container.h"
#include "ui/renderable/element/Text.h"

namespace AW
{
	class DebugMonitor : public Container
	{
		std::vector<double> frameTimes;
		int frameTimeI = 0, totalFrameTimeSamplesToKeep = 100, sceneGraphYOffset = 0;
		bool hasHydrated = false, isHovering = false;

		std::shared_ptr<Timer> updateTimer;

		std::shared_ptr<Text> fpsText;
		std::shared_ptr<Text> frameTimeText;

		std::shared_ptr<Container> sceneGraphInfoContainer;
		std::shared_ptr<Renderable> sceneGraphAligningElement;
		std::vector<int> allowedCrawlElements;
		std::map<int, int> textIdToSceneId;

		std::string frameTimeToFPSString(const double& time);

		void updateFrameTimeInfo(const double& frameTime);
		void updateSceneGraphRepresentation();
		void updateSceneGraphRepresentationRecursive(std::shared_ptr<GameObject> obj, int depth = 0);
		void addInfoLineToSceneGraph(const std::string& depthStr, std::string propName, std::string value, int objId);

		void checkIfHovering();

	public:
		DebugMonitor();

		void onInitialAttach();

		void onAttach();

		void onCreateChildren();
		void onLayoutChildren();

		void onEnterFrame(const double& frameTime);

		void onMouseButtonLeftUp();
		void onMouseWheel(int x, int y);

		void clear();
	};
}
