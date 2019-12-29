#include "SystemModuleBundle.h"

namespace AW
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
		screen->bindTime(time);
		shader->bindTime(time);
		physic->bindTime(time);
		animation->bindEvent(event);
		event->bindInput(input);
		event->bindThread(thread);
		serialization->bindThread(thread);
		filesystem->bindThread(thread);
		storage->bindThread(thread);
		asset->bindFilesystem(filesystem);
		asset->bindStorage(storage);
		font->bindAsset(asset);
		sound->bindAsset(asset);
		shader->bindAsset(asset);
		screen->bindShader(shader);
		screen->bindTexture(texture);
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
		shader = createModule<ShaderContainer>();
		physic = createModule<Physic>();

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

	void SystemModuleBundle::onLoadResources()
	{
		if (loaded) return;

		for (const auto module : modules)
		{
			module->onLoadResources();
		}

		loaded = true;
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

	void SystemModuleBundle::onEnterFrame(const double& frameTime)
	{
		if (!ready) return;

		for (const auto module : modules)
		{
			module->onEnterFrame(frameTime);
		}
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