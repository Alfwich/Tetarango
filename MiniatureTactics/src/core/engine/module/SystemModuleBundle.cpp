#include "SystemModuleBundle.h"

namespace MT
{
	SystemModuleBundle* SystemModuleBundle::staticModuleBundle;

	std::shared_ptr<SystemModuleBundle> SystemModuleBundle::getModuleBundle()
	{
		return std::static_pointer_cast<SystemModuleBundle>(SystemModuleBundle::staticModuleBundle->shared_from_this());
	}

	void SystemModuleBundle::bindModules()
	{
		texture->bindScreen(screen);
		texture->bindAsset(asset);
		logger->bindFilesystem(filesystem);
		logger->bindTime(time);
		logger->bindThread(thread);
		storage->bindFilesystem(filesystem);
		animation->bindTime(time);
		animation->bindEvent(event);
		event->bindInput(input);
		event->bindThread(thread);
		serialization->bindThread(thread);
		filesystem->bindThread(thread);
		storage->bindThread(thread);
		screen->bindCollision(collision);
		asset->bindFilesystem(filesystem);
		asset->bindStorage(storage);
		font->bindAsset(asset);
		sound->bindAsset(asset);
	}

	SystemModuleBundle::SystemModuleBundle()
	{
		SystemModuleBundle::staticModuleBundle = this;

		thread = createModule<Thread>();
		screen = createModule<Screen>();
		filesystem = createModule<Filesystem>();
		input = createModule<Input>();
		serialization = createModule<Serialization>();
		storage = createModule<Storage>();
		time = createModule<Time>();
		animation = createModule<AnimationContainer>();
		texture = createModule<TextureContainer>();
		font = createModule<FontContainer>();
		event = createModule<Event>();
		sound = createModule<Sound>();
		collision = createModule<Collision>();
		logger = createModule<Logger>();
		asset = createModule<Asset>();

		bindModules();
	}

	void SystemModuleBundle::onAssignConfig(std::shared_ptr<Config::GameConfig> config)
	{
		this->gameConfig = config;

		for (const auto module : modules)
		{
			module->gameConfig = config;
		}
	}

	void SystemModuleBundle::onInit()
	{
		if (init) return;

		for (const auto module : modules)
		{
			module->gameConfig = gameConfig;
			module->onInit();
		}

		status = ModuleBundleStatus::INITIALIZED;
		init = true;
	}

	void SystemModuleBundle::onReady()
	{
		if (ready) return;

		for (const auto module : modules)
		{
			module->onReady();
		}

		status = ModuleBundleStatus::READY;
		ready = true;
	}

	void SystemModuleBundle::onCleanup()
	{
		if (cleanup) return;

		for (const auto module : modules)
		{
			module->onCleanup();
			module->isCleanedUp = true;
		}

		status = ModuleBundleStatus::CLEANED_UP;
		cleanup = true;

		SystemModuleBundle::staticModuleBundle = nullptr;
	}
}