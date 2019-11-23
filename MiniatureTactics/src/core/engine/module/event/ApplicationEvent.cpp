#include "ApplicationEvent.h"

namespace MT
{
	ApplicationEvent::ApplicationEvent()
	{
	}

	ApplicationEvent::ApplicationEvent(std::string what)
	{
		what = what;
	}

	ApplicationEvent::ApplicationEvent(Events event)
	{
		code = event;
	}

	ApplicationEvent::ApplicationEvent(std::string what, Events event)
	{
		what = what;
		code = event;
	}
}
