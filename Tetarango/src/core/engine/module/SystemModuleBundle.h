#pragma once

#include "IBaseModule.h"
#include "ModuleBundleStatus.h"
#include "filesystem/Filesystem.h"
#include "font/FontContainer.h"
#include "input/Input.h"
#include "serialization/Serialization.h"
#include "animation/AnimationContainer.h"
#include "storage/Storage.h"
#include "event/Event.h"
#include "sound/Sound.h"
#include "thread/Thread.h"
#include "asset/Asset.h"
#include "shader/ShaderContainer.h"
#include "physic/Physic.h"

namespace AW
{
	class TextureContainer;
	class AnimationContainer;
	class Screen;
	class Time;


	class SystemModuleBundle : public IBaseModule
	{
		static SystemModuleBundle* staticModuleBundle;
		std::vector<std::shared_ptr<IBaseModule>> modules;
		bool init = false, loaded = false, ready = false, cleanup = false;

		template <typename T>
		std::shared_ptr<T> createModule();
		void bindModules();

	public:
		static std::shared_ptr<SystemModuleBundle> getModuleBundle();

		SystemModuleBundle();

		ModuleBundleStatus status = ModuleBundleStatus::UNINITIALIZED;

		std::shared_ptr<Logger> logger;
		std::shared_ptr<Screen> screen;
		std::shared_ptr<Filesystem> filesystem;
		std::shared_ptr<Input> input;
		std::shared_ptr<Serialization> serialization;
		std::shared_ptr<Storage> storage;
		std::shared_ptr<Time> time;
		std::shared_ptr<AnimationContainer> animation;
		std::shared_ptr<TextureContainer> texture;
		std::shared_ptr<FontContainer> font;
		std::shared_ptr<Event> event;
		std::shared_ptr<Sound> sound;
		std::shared_ptr<Thread> thread;
		std::shared_ptr<Asset> asset;
		std::shared_ptr<ShaderContainer> shader;
		std::shared_ptr<Physic> physic;

		void onAssignConfig(std::shared_ptr<Config::GameConfig> config);
		void onInit();
		void onLoadResources();
		void onReady();
		void onEnterFrame(const double& frameTime);
		void onCleanup();
	};

	template<typename T>
	inline std::shared_ptr<T> SystemModuleBundle::createModule()
	{
		const auto module = std::make_shared<T>();
		this->modules.push_back(module);
		return module;
	}
}

#include "display/TextureContainer.h"
#include "animation/AnimationContainer.h"
#include "display/Screen.h"
#include "time/Time.h"
