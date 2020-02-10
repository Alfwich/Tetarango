#include "TestScene3.h"

namespace AWGame
{
	TestScene3::TestScene3()
		: AW::Scene("test_scene_3")
	{
		GORegister(TestScene3);
	}

	void TestScene3::onLoadResources()
	{
		modules->lua->registerObjectImplementation("res/lua/game/scene/test-3.lua", "test-3");
	}

	void TestScene3::onInitialAttach()
	{
		setLuaImplementation("test-3");
	}
}