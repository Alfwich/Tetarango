#include "Line.h"

#include "ui/physic/body/Body.h"

namespace
{
	const auto bodyId = "line-body";
}

namespace AWGame
{
	Line::Line()
	{
		setSize(100, 1);
		GORegister(Line);
	}

	void Line::onCreateChildren()
	{
		const auto body = std::make_shared<AW::Body>();
		body->name = bodyId;
		body->setStaticBody();
		body->setBodyType(AW::BodyType::Line);
		add(body);
	}

	void Line::onChildrenHydrated()
	{
		body = findChildWithName<AW::Body>(bodyId);
	}

	std::shared_ptr<AW::Renderable> Line::getRenderableBody()
	{
		return std::dynamic_pointer_cast<Renderable>(shared_from_this());
	}

	std::shared_ptr<AW::Body> Line::getBodyObject()
	{
		return body;
	}
}