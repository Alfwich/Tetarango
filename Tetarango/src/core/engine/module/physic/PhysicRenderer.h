#pragma once

#include "box2d/b2_draw.h"
#include "util/Rect.h"
#include "util/Vec.h"


namespace AW
{
	class GameObject;

	class PhysicRenderer : public b2Draw
	{
	public:
		Rect render;
		double zoom = 50.0;
		AWVec2<float> translateB2Point(const b2Vec2& vert);

		void prepare(double screenW, double screenH);
		void matchWithCamera(const std::shared_ptr<GameObject>& camera);

		// Inherited via b2Draw
		virtual void DrawPolygon(const b2Vec2 * vertices, int32 vertexCount, const b2Color & color) override;
		virtual void DrawSolidPolygon(const b2Vec2 * vertices, int32 vertexCount, const b2Color & color) override;
		virtual void DrawCircle(const b2Vec2 & center, float radius, const b2Color & color) override;
		virtual void DrawSolidCircle(const b2Vec2 & center, float radius, const b2Vec2 & axis, const b2Color & color) override;
		virtual void DrawSegment(const b2Vec2 & p1, const b2Vec2 & p2, const b2Color & color) override;
		virtual void DrawTransform(const b2Transform & xf) override;
		virtual void DrawPoint(const b2Vec2 & p, float size, const b2Color & color) override;
	};
}

#include "engine/GameObject.h"
