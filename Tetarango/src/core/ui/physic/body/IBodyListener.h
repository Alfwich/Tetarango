#pragma once

namespace AW
{
	class IBodyListener
	{
	public:
		virtual void onBodyUpdated() { /* NO-OP*/ };
		virtual void onBodyUpdated(int id) { onBodyUpdated(); };
	};
}
