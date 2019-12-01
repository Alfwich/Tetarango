#include "ApplicationEvent.h"

namespace MT
{
	ApplicationEvent::ApplicationEvent()
	{
	}

	ApplicationEvent::ApplicationEvent(std::string what)
	{
		this->what = what;
	}

	ApplicationEvent::ApplicationEvent(Events event)
	{
		code = event;
	}

	ApplicationEvent::ApplicationEvent(std::string what, Events event)
	{
		this->what = what;
		code = event;
	}
}
