#pragma once

#include <string>
#include "engine/module/display/ScreenConfig.h"

namespace AWCore
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

