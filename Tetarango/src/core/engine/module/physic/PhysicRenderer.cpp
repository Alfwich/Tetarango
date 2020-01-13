#include "PhysicRenderer.h"

#include "GL/glew.h"

#include "ui/renderable/Renderable.h"
#include "ui/camera/Camera.h"

namespace AW
{
	AWVec2<float> PhysicRenderer::translateB2Point(const b2Vec2& vert)
	{
		const auto aspect = (float)(render.h / render.w);
		const auto offsetScale = ((render.w / Renderable::scalingFactor) / 2.0) + 1.0;
		const auto offsetX = (float)(render.x / render.w) * -offsetScale;
		const auto offsetY = (float)(render.y / render.h) * offsetScale;
		const auto z = (float)zoom;
		const auto x = (vert.x * aspect) + offsetX;
		const auto y = vert.y + offsetY;

		return AWVec2<float>(x / z, y / z);
	}

	void PhysicRenderer::prepare(double screenW, double screenH)
	{
		SetFlags(e_jointBit | e_shapeBit | e_pairBit | e_centerOfMassBit);

		render.w = screenW;
		render.h = screenH;

		glUseProgram(0);
	}

	void PhysicRenderer::matchWithCamera(const std::shared_ptr<GameObject>& camera)
	{
		const auto cameraPtr = std::dynamic_pointer_cast<Camera>(camera);
		if (cameraPtr == nullptr) return;

		render.x = cameraPtr->getScreenAnchorX();
		render.y = cameraPtr->getScreenAnchorY();
		zoom = (1 / cameraPtr->getZoom()) * 10.0;
	}

	void PhysicRenderer::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
	{
		glBegin(GL_LINE_LOOP);
		glColor4f(color.r, color.g, color.b, color.a);
		for (auto i = 0; i < vertexCount; ++i)
		{
			auto v = translateB2Point(vertices[i]);
			glVertex2f(v.x, v.y);
		}
		glEnd();
	}

	void PhysicRenderer::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
	{
		glBegin(GL_POLYGON);
		glColor4f(color.r, color.g, color.b, color.a);
		for (auto i = 0; i < vertexCount; ++i)
		{
			auto v = translateB2Point(vertices[i]);
			glVertex2f(v.x, v.y);
		}
		glEnd();
	}

	void PhysicRenderer::DrawCircle(const b2Vec2& center, float radius, const b2Color& color)
	{
		glBegin(GL_LINE_LOOP);
		glColor4f(color.r, color.g, color.b, color.a);
		const auto points = 16;
		const auto piD = (AW::NumberHelper::PI * 2.0) / points;
		for (auto i = 0; i < 16; ++i)
		{
			const auto p = b2Vec2(center.x + (float)std::cos(i * piD) * radius, center.y + (float)std::sin(i * piD) * radius);
			auto v = translateB2Point(p);
			glVertex2f(v.x, v.y);
		}
		glEnd();
	}

	void PhysicRenderer::DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color)
	{
		glBegin(GL_POLYGON);
		glColor4f(color.r, color.g, color.b, color.a);
		const auto points = 16;
		const auto piD = (AW::NumberHelper::PI * 2.0) / points;
		for (auto i = 0; i < 16; ++i)
		{
			const auto p = b2Vec2(center.x + (float)std::cos(i * piD) * radius, center.y + (float)std::sin(i * piD) * radius);
			auto v = translateB2Point(p);
			glVertex2f(v.x, v.y);
		}
		glEnd();
	}

	void PhysicRenderer::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
	{
		glBegin(GL_LINES);
		glColor4f(color.r, color.g, color.b, color.a);
		const auto p1V = translateB2Point(p1), p2V = translateB2Point(p2);
		glVertex2f(p1V.x, p1V.y);
		glVertex2f(p2V.x, p2V.y);
		glEnd();
	}

	void PhysicRenderer::DrawTransform(const b2Transform& xf)
	{
		// TODO: If needed
	}

	void PhysicRenderer::DrawPoint(const b2Vec2& p, float size, const b2Color& color)
	{
		glBegin(GL_POINT);
		glColor4f(color.r, color.g, color.b, color.a);
		const auto pV = translateB2Point(p);
		glVertex2f(pV.x, pV.y);
		glEnd();
	}
}
