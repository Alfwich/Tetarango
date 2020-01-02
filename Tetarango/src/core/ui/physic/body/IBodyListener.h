#pragma once

namespace
{
	const auto noop = std::vector<AWVec2<double>>();
}

namespace AW
{
	class IBodyListener
	{
	public:
		virtual std::shared_ptr<Renderable> getRenderableBody() = 0;

		virtual void onBodyUpdated() { /* NO-OP*/ };
		virtual void onBodyUpdated(int id) { onBodyUpdated(); };
		virtual const std::vector<AWVec2<double>>& getBodyScreenPoints() { return noop; }
	};
}
