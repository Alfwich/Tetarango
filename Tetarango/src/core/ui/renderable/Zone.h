#pragma once

#include "ui/renderable/container/Container.h"

namespace AW
{

	class Zone : public Container
	{
		bool requestReconciliation;

	public:
		Zone();

		void visualize();
		void setRequestReconciliation(bool flag);

		virtual void onInitialAttach();
	};

}
