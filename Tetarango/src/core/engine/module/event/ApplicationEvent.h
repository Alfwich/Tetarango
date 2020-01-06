#pragma once

#include "engine/module/display/ScreenConfig.h"

namespace AW
{

	enum class Events {
		Unspecified,
		QuitRequested,
		ReprovisionScreen
	};

	class ApplicationEvent
	{
	public:
		ApplicationEvent(Events code) : code(code) {};

		const Events code;
	};

	class ReprovisionScreenApplicationEvent : public ApplicationEvent
	{
	public:
		ReprovisionScreenApplicationEvent(ScreenConfig config) : ApplicationEvent(Events::ReprovisionScreen), config(config) {}

		const ScreenConfig config;
	};

}

