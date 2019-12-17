#pragma once

#include <memory>
#include <SDL.h>
#include "engine/module/serialization/ISerializable.h"
#include "util/Rect.h"
#include "util/Color.h"

namespace AW
{
	class ApplicationObject;

	enum class RenderType
	{
		None,
		NoneAndBlockChildren,
		Element,
		Primitive,
		ParticleSystem,
		TileMap,
		Backdrop,
		Container
	};

	enum class RenderPositionProcessing
	{
		None,
		Floor
	};

	enum class RenderTextureMode
	{
		LinearNoWrap,
		BilinearNoWrap,
		LinearWrapping,
		BilinearWrapping
	};

	enum class RenderPositionMode
	{
		Unspecified,
		Positioned,
		Absolute
	};

	class Renderable : public ISerializableDataSubscriber
	{
		double rot = 0.0, alpha = 1.0;
		double scaleX = 1.0, scaleY = 1.0;
		Rect rect;

	protected:
		std::shared_ptr<Color> colorModulation;

	public:
		bool rotateInParentSpace = true, disableCulling = false;

		virtual void setColor(int r, int g, int b, int a = 0xff);
		virtual void setColor(const Color& color);
		virtual void setColor(const Color* color);
		Color* getColor();
		Color debugColor;

		virtual Rect getRect();

		virtual double getX();
		virtual void setX(double newX);

		virtual double getY();
		virtual void setY(double newY);

		virtual double getWidth();
		virtual void setWidth(double newWidth);

		virtual double getHeight();
		virtual void setHeight(double newHeight);

		virtual double getRotation();
		virtual void setRotation(double newRotation);

		virtual double getScaleX();
		virtual void setScaleX(double newScaleX);

		virtual double getScaleY();
		virtual void setScaleY(double newScaleY);

		virtual double getAlpha();
		virtual void setAlpha(double newAlpha);

		virtual void onTransitionStart() { /* NO-OP */ };
		virtual void onTransitionFrame(double p, const Rect& targetRect, double targetAlpha, int transitionId);
		virtual void onTransitionEnd() { /* NO-OP */ };

		void setPosition(double x, double y);
		void movePosition(double xDelta, double yDelta);
		void setSize(double width, double height);
		void setSizeAndPosition(double x, double y, double width, double height);
		void setSizeAndPosition(const Rect& rect);
		void setScale(double scale);
		void setScale(double scaleX, double scaleY);
		void rotate(double rotDelta);

		double getLeft();
		double getRight();
		double getTop();
		double getBottom();
		double getHalfWidth();
		double getHalfHeight();

		void topLeftAlignSelf(double xOffset = 0.0, double yOffset = 0.0);

		void centerWithin(ApplicationObject* other, double xOffset = 0.0, double yOffset = 0.0);
		void centerWithin(std::shared_ptr<ApplicationObject> other, double xOffset = 0.0, double yOffset = 0.0);

		void toLeftOf(ApplicationObject* other, double xPadding = 0.0, double yPadding = 0.0);
		void toLeftOf(std::shared_ptr<ApplicationObject> other, double xPadding = 0.0, double yPadding = 0.0);

		void toLeftTopOf(ApplicationObject* other, double xPadding = 0.0, double yPadding = 0.0);
		void toLeftTopOf(std::shared_ptr<ApplicationObject> other, double xPadding = 0.0, double yPadding = 0.0);

		void toLeftBottomOf(ApplicationObject* other, double xPadding = 0.0, double yPadding = 0.0);
		void toLeftBottomOf(std::shared_ptr<ApplicationObject> other, double xPadding = 0.0, double yPadding = 0.0);

		void toRightOf(ApplicationObject* other, double xPadding = 0.0, double yPadding = 0.0);
		void toRightOf(std::shared_ptr<ApplicationObject> other, double xPadding = 0.0, double yPadding = 0.0);

		void toRightTopOf(ApplicationObject* other, double xPadding = 0.0, double yPadding = 0.0);
		void toRightTopOf(std::shared_ptr<ApplicationObject> other, double xPadding = 0.0, double yPadding = 0.0);

		void toRightBottomOf(ApplicationObject* other, double xPadding = 0.0, double yPadding = 0.0);
		void toRightBottomOf(std::shared_ptr<ApplicationObject> other, double xPadding = 0.0, double yPadding = 0.0);

		void toTopOf(ApplicationObject* other, double xPadding = 0.0, double yPadding = 0.0);
		void toTopOf(std::shared_ptr<ApplicationObject> other, double xPadding = 0.0, double yPadding = 0.0);

		void toTopLeftOf(ApplicationObject* other, double xPadding = 0.0, double yPadding = 0.0);
		void toTopLeftOf(std::shared_ptr<ApplicationObject> other, double xPadding = 0.0, double yPadding = 0.0);

		void toTopRightOf(ApplicationObject* other, double xPadding = 0.0, double yPadding = 0.0);
		void toTopRightOf(std::shared_ptr<ApplicationObject> other, double xPadding = 0.0, double yPadding = 0.0);

		void toBottomOf(ApplicationObject* other, double xPadding = 0.0, double yPadding = 0.0);
		void toBottomOf(std::shared_ptr<ApplicationObject> other, double xPadding = 0.0, double yPadding = 0.0);

		void toBottomLeftOf(ApplicationObject* other, double xPadding = 0.0, double yPadding = 0.0);
		void toBottomLeftOf(std::shared_ptr<ApplicationObject> other, double xPadding = 0.0, double yPadding = 0.0);

		void toBottomRightOf(ApplicationObject* other, double xPadding = 0.0, double yPadding = 0.0);
		void toBottomRightOf(std::shared_ptr<ApplicationObject> other, double xPadding = 0.0, double yPadding = 0.0);

		void matchSize(ApplicationObject* other, double wOffset = 0.0, double hOffset = 0.0);
		void matchSize(std::shared_ptr<ApplicationObject> other, double wOffset = 0.0, double hOffset = 0.0);

		void matchSizeAndCenter(ApplicationObject* other, double wOffset = 0.0, double hOffset = 0.0, double xOffset = 0.0, double yOffset = 0.0);
		void matchSizeAndCenter(std::shared_ptr<ApplicationObject> other, double wOffset = 0.0, double hOffset = 0.0, double xOffset = 0.0, double yOffset = 0.0);

		void leftAlign(ApplicationObject* other, double xOffset = 0.0, double yOffset = 0.0);
		void leftAlign(std::shared_ptr<ApplicationObject> other, double xOffset = 0.0, double yOffset = 0.0);

		void toInnerLeftIn(ApplicationObject* other, double xOffset = 0.0, double yOffset = 0.0);
		void toInnerLeftIn(std::shared_ptr<ApplicationObject> other, double xOffset = 0.0, double yOffset = 0.0);

		void toInnerRightIn(ApplicationObject* other, double xOffset = 0.0, double yOffset = 0.0);
		void toInnerRightIn(std::shared_ptr<ApplicationObject> other, double xOffset = 0.0, double yOffset = 0.0);

		void toInnerTopIn(ApplicationObject* other, double xOffset = 0.0, double yOffset = 0.0);
		void toInnerTopIn(std::shared_ptr<ApplicationObject> other, double xOffset = 0.0, double yOffset = 0.0);

		void toInnerBottomIn(ApplicationObject* other, double xOffset = 0.0, double yOffset = 0.0);
		void toInnerBottomIn(std::shared_ptr<ApplicationObject> other, double xOffset = 0.0, double yOffset = 0.0);

		void toInnerTopLeftIn(ApplicationObject* other, double xOffset = 0.0, double yOffset = 0.0);
		void toInnerTopLeftIn(std::shared_ptr<ApplicationObject> other, double xOffset = 0.0, double yOffset = 0.0);

		void toInnerTopRightIn(ApplicationObject* other, double xOffset = 0.0, double yOffset = 0.0);
		void toInnerTopRightIn(std::shared_ptr<ApplicationObject> other, double xOffset = 0.0, double yOffset = 0.0);

		void toInnerBottomLeftIn(ApplicationObject* other, double xOffset = 0.0, double yOffset = 0.0);
		void toInnerBottomLeftIn(std::shared_ptr<ApplicationObject> other, double xOffset = 0.0, double yOffset = 0.0);

		void toInnerBottomRightIn(ApplicationObject* other, double xOffset = 0.0, double yOffset = 0.0);
		void toInnerBottomRightIn(std::shared_ptr<ApplicationObject> other, double xOffset = 0.0, double yOffset = 0.0);

		virtual std::shared_ptr<SerializationClient> doSerialize(SerializationHint hint);
		virtual void doManualSerialize(SerializationHint hint, std::shared_ptr<SerializationClient> injectedClient);
	};
}

#include "engine/ApplicationObject.h"
