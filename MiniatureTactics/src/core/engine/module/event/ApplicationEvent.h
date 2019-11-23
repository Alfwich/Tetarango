#pragma once

#include <string>
#include "Events.h"

namespace MT
{

	class ApplicationEvent
	{
	public:
		ApplicationEvent();
		ApplicationEvent(std::string what);
		ApplicationEvent(Events event);
		ApplicationEvent(std::string what, Events event);

		Events code;
		std::string what;
	};

}

