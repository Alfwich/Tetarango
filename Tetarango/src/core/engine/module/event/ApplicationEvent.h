#pragma once

#include "engine/module/display/ScreenConfig.h"

namespace AW
{

	enum class Events
	{
		Unspecified,
		QuitRequested,
		ReprovisionScreen,
		UpEvent
	};

	enum class EventDirection
	{
		Down,
		Up
	};

	class ApplicationEvent
	{
	public:
		ApplicationEvent(Events code, void* data = nullptr) : code(code), data(data) {};
		ApplicationEvent(const std::string& message, EventDirection direction = EventDirection::Down) : message(message), direction(direction) {};

		Events code = Events::Unspecified;
		EventDirection direction = EventDirection::Down;
		std::string message;
		bool handled = false, stopPropagation = false;
		void* data = nullptr;
	};

}

