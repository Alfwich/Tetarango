#include "Renderable.h"

#include "util/NumberHelper.h"

namespace AWCore
{
	void Renderable::setColor(const Color& color)
	{
		setColor(color.r, color.g, color.b, color.a);
	}

	void Renderable::setColor(const Color * color)
	{
		if (color != nullptr)
		{
			setColor(color->r, color->g, color->g, color->a);
		}
	}

	void Renderable::setColor(int r, int g, int b, int a)
	{
		if (colorModulation == nullptr) {
			colorModulation = std::make_shared<Color>(r, g, b, a);
		}
		else
		{
			colorModulation->r = r;
			colorModulation->g = g;
			colorModulation->b = b;
			colorModulation->a = a;
		}
	}

	Color* Renderable::getColor()
	{
		return colorModulation != nullptr ? colorModulation.get() : nullptr;
	}

	Rect Renderable::getRect()
	{
		Rect r = rect;

		r.w *= scaleX;
		r.h *= scaleY;

		return r;
	}



	double Renderable::getX()
	{
		return rect.x;
	}

	void Renderable::setX(double newX)
	{
		rect.x = newX;
	}

	double Renderable::getY()
	{
		return rect.y;
	}

	void Renderable::setY(double newY)
	{
		rect.y = newY;
	}

	double Renderable::getWidth()
	{
		return rect.w * scaleX;
	}

	void Renderable::setWidth(double newWidth)
	{
		rect.w = newWidth;
	}

	double Renderable::getHeight()
	{
		return rect.h * scaleY;
	}

	void Renderable::setHeight(double newHeight)
	{
		rect.h = newHeight;
	}

	void Renderable::setRotation(double newRotation)
	{
		rot = NumberHelper::clampWrap(newRotation, 0.0, 359.0);
	}

	double Renderable::getScaleX()
	{
		return scaleX;
	}

	void Renderable::setScaleX(double newScaleX)
	{
		scaleX = newScaleX;
	}

	double Renderable::getScaleY()
	{
		return scaleY;
	}

	void Renderable::setScaleY(double newScaleY)
	{
		scaleY = newScaleY;
	}

	double Renderable::getAlpha()
	{
		return alpha;
	}

	void Renderable::setAlpha(double newAlpha)
	{
		alpha = newAlpha;
	}

	void Renderable::onTransitionFrame(double p, const Rect& targetRect, double targetAlpha, int transitionId)
	{
		setSizeAndPosition(targetRect);
		setAlpha(targetAlpha);
	}

	void Renderable::setPosition(double x, double y)
	{
		setX(x);
		setY(y);
	}

	void Renderable::movePosition(double xDelta, double yDelta)
	{
		setX(rect.x + xDelta);
		setY(rect.y + yDelta);
	}

	void Renderable::setSize(double width, double height)
	{
		setWidth(width);
		setHeight(height);
	}

	void Renderable::setSizeAndPosition(double x, double y, double width, double height)
	{
		setPosition(x, y);
		setSize(width, height);
	}

	void Renderable::setSizeAndPosition(const Rect& rect)
	{
		setSizeAndPosition(rect.x, rect.y, rect.w, rect.h);
	}

	void Renderable::setScale(double scale)
	{
		scaleX = scale;
		scaleY = scale;
	}

	void Renderable::setScale(double scaleX, double scaleY)
	{
		this->scaleX = scaleX;
		this->scaleY = scaleY;
	}

	void Renderable::rotate(double rotDelta)
	{
		setRotation(rot + rotDelta);
	}

	double Renderable::getRotation()
	{
		return rot;
	}

	double Renderable::getLeft()
	{
		return getX() - getHalfWidth();
	}

	double Renderable::getRight()
	{
		return getX() + getHalfWidth();
	}

	double Renderable::getTop()
	{
		return getY() - getHalfHeight();
	}

	double Renderable::getBottom()
	{
		return getY() + getHalfHeight();
	}

	double Renderable::getHalfWidth()
	{
		return getWidth() / 2.0;
	}

	double Renderable::getHalfHeight()
	{
		return getHeight() / 2.0;
	}

	void Renderable::centerAlignSelf(double xOffset, double yOffset)
	{
		setPosition(this->getHalfWidth() + xOffset, this->getHalfHeight() + yOffset);
	}

	void Renderable::floorAlignSelf(double xOffset, double yOffset)
	{
		setPosition(std::floor(getX() + xOffset), std::floor(getY() + yOffset));
	}

	void Renderable::centerWithin(ApplicationObject* other, double xOffset, double yOffset)
	{
		const auto otherR = dynamic_cast<Renderable*>(other);

		if (otherR != nullptr)
		{
			setPosition(otherR->getHalfWidth() + xOffset, otherR->getHalfHeight() + yOffset);
		}
	}

	void Renderable::toLeftOf(ApplicationObject* other, double xOffset, double yOffset)
	{
		const auto otherR = dynamic_cast<Renderable*>(other);

		if (otherR != nullptr)
		{
			setPosition(otherR->getLeft() - getHalfWidth() - xOffset, otherR->getY() + yOffset);
		}
	}

	void Renderable::toLeftTopOf(ApplicationObject* other, double xOffset, double yOffset)
	{
		const auto otherR = dynamic_cast<Renderable*>(other);

		if (otherR != nullptr)
		{
			setPosition(otherR->getLeft() - getHalfWidth() - xOffset, otherR->getTop() + getHalfHeight() + yOffset);
		}
	}

	void Renderable::toLeftBottomOf(ApplicationObject* other, double xOffset, double yOffset)
	{
		const auto otherR = dynamic_cast<Renderable*>(other);

		if (otherR != nullptr)
		{
			setPosition(otherR->getLeft() - getHalfWidth() - xOffset, otherR->getBottom() - getHalfHeight() - yOffset);
		}
	}

	void Renderable::toRightOf(ApplicationObject* other, double xOffset, double yOffset)
	{
		const auto otherR = dynamic_cast<Renderable*>(other);

		if (otherR != nullptr)
		{
			setPosition(otherR->getRight() + getHalfWidth() + xOffset, otherR->getY() + yOffset);
		}
	}

	void Renderable::toRightTopOf(ApplicationObject* other, double xOffset, double yOffset)
	{
		const auto otherR = dynamic_cast<Renderable*>(other);

		if (otherR != nullptr)
		{
			setPosition(otherR->getRight() + getHalfWidth() + xOffset, otherR->getTop() + getHalfHeight() + yOffset);
		}
	}

	void Renderable::toRightBottomOf(ApplicationObject* other, double xOffset, double yOffset)
	{
		const auto otherR = dynamic_cast<Renderable*>(other);

		if (otherR != nullptr)
		{
			setPosition(otherR->getRight() + getHalfWidth() + xOffset, otherR->getBottom() - getHalfHeight() - yOffset);
		}
	}

	void Renderable::toTopOf(ApplicationObject* other, double xOffset, double yOffset)
	{
		const auto otherR = dynamic_cast<Renderable*>(other);

		if (otherR != nullptr)
		{
			setPosition(otherR->getX() + xOffset, otherR->getTop() - getHalfHeight() - yOffset);
		}
	}

	void Renderable::toTopLeftOf(ApplicationObject* other, double xOffset, double yOffset)
	{
		const auto otherR = dynamic_cast<Renderable*>(other);

		if (otherR != nullptr)
		{
			setPosition(otherR->getLeft() + getHalfWidth() + xOffset, otherR->getTop() - getHalfHeight() - yOffset);
		}
	}

	void Renderable::toTopRightOf(ApplicationObject* other, double xOffset, double yOffset)
	{
		const auto otherR = dynamic_cast<Renderable*>(other);

		if (otherR != nullptr)
		{
			setPosition(otherR->getRight() - getHalfWidth() - xOffset, otherR->getTop() - getHalfHeight() - yOffset);
		}
	}

	void Renderable::toBottomOf(ApplicationObject* other, double xOffset, double yOffset)
	{
		const auto otherR = dynamic_cast<Renderable*>(other);

		if (otherR != nullptr)
		{
			setPosition(otherR->getX() + xOffset, otherR->getBottom() + getHalfHeight() + yOffset);
		}
	}

	void Renderable::toBottomRightOf(ApplicationObject* other, double xOffset, double yOffset)
	{
		const auto otherR = dynamic_cast<Renderable*>(other);

		if (otherR != nullptr)
		{
			setPosition(otherR->getRight() - getHalfWidth() - xOffset, otherR->getBottom() + getHalfHeight() + yOffset);
		}
	}

	void Renderable::toBottomLeftOf(ApplicationObject* other, double xOffset, double yOffset)
	{
		const auto otherR = dynamic_cast<Renderable*>(other);

		if (otherR != nullptr)
		{
			setPosition(otherR->getLeft() + getHalfWidth() + xOffset, otherR->getBottom() + getHalfHeight() + yOffset);
		}
	}

	void Renderable::matchSize(ApplicationObject* other, double wOffset, double hOffset)
	{
		const auto otherR = dynamic_cast<Renderable*>(other);

		if (otherR != nullptr)
		{
			setSize(otherR->getWidth() - wOffset, otherR->getHeight() + hOffset);
		}
	}

	void Renderable::matchSizeAndCenter(ApplicationObject* other, double wOffset, double hOffset, double xOffset, double yOffset)
	{
		const auto otherR = dynamic_cast<Renderable*>(other);

		if (otherR != nullptr)
		{
			setSizeAndPosition(otherR->getHalfWidth() + xOffset, otherR->getHalfHeight() + yOffset, otherR->getWidth() + wOffset * 2.0, otherR->getHeight() + hOffset * 2.0);
		}
	}

	void Renderable::centerWithin(std::shared_ptr<ApplicationObject> other, double xOffset, double yOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			centerWithin(otherPtr, xOffset, yOffset);
		}
	}

	void Renderable::toLeftOf(std::shared_ptr<ApplicationObject> other, double xOffset, double yOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			toLeftOf(otherPtr, xOffset, yOffset);
		}
	}

	void Renderable::toLeftTopOf(std::shared_ptr<ApplicationObject> other, double xOffset, double yOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			toLeftTopOf(otherPtr, xOffset, yOffset);
		}
	}

	void Renderable::toLeftBottomOf(std::shared_ptr<ApplicationObject> other, double xOffset, double yOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			toLeftBottomOf(otherPtr, xOffset, yOffset);
		}
	}

	void Renderable::toRightOf(std::shared_ptr<ApplicationObject> other, double xOffset, double yOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			toRightOf(otherPtr, xOffset, yOffset);
		}
	}

	void Renderable::toRightTopOf(std::shared_ptr<ApplicationObject> other, double xOffset, double yOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			toRightTopOf(otherPtr, xOffset, yOffset);
		}
	}

	void Renderable::toRightBottomOf(std::shared_ptr<ApplicationObject> other, double xOffset, double yOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			toRightBottomOf(otherPtr, xOffset, yOffset);
		}
	}

	void Renderable::toTopOf(std::shared_ptr<ApplicationObject> other, double xOffset, double yOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			toTopOf(otherPtr, xOffset, yOffset);
		}
	}

	void Renderable::toTopLeftOf(std::shared_ptr<ApplicationObject> other, double xOffset, double yOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			toTopLeftOf(otherPtr, xOffset, yOffset);
		}
	}

	void Renderable::toTopRightOf(std::shared_ptr<ApplicationObject> other, double xOffset, double yOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			toTopRightOf(otherPtr, xOffset, yOffset);
		}
	}

	void Renderable::toBottomOf(std::shared_ptr<ApplicationObject> other, double xOffset, double yOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			toBottomOf(otherPtr, xOffset, yOffset);
		}
	}

	void Renderable::toBottomLeftOf(std::shared_ptr<ApplicationObject> other, double xOffset, double yOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			toBottomLeftOf(otherPtr, xOffset, yOffset);
		}
	}

	void Renderable::toBottomRightOf(std::shared_ptr<ApplicationObject> other, double xOffset, double yOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			toBottomRightOf(otherPtr, xOffset, yOffset);
		}
	}

	void Renderable::matchSize(std::shared_ptr<ApplicationObject> other, double wOffset, double hOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			matchSize(otherPtr, wOffset, hOffset);
		}
	}

	void Renderable::matchSizeAndCenter(std::shared_ptr<ApplicationObject> other, double wOffset, double hOffset, double xOffset, double yOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			matchSizeAndCenter(otherPtr, wOffset, hOffset, xOffset, yOffset);
		}
	}

	void Renderable::leftAlign(ApplicationObject* other, double xOffset, double yOffset)
	{
		const auto otherR = dynamic_cast<Renderable*>(other);

		if (otherR != nullptr)
		{
			setPosition(otherR->getLeft() + getHalfWidth() + xOffset, otherR->getY() + yOffset);
		}
	}

	void Renderable::leftAlign(std::shared_ptr<ApplicationObject> other, double xOffset, double yOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			leftAlign(otherPtr, xOffset, yOffset);
		}
	}

	void Renderable::toInnerLeftIn(ApplicationObject* other, double xOffset, double yOffset)
	{
		const auto otherR = dynamic_cast<Renderable*>(other);

		if (otherR != nullptr)
		{
			setPosition(otherR->getLeft() + getHalfWidth() + xOffset, otherR->getY() + yOffset);
		}
	}

	void Renderable::toInnerLeftIn(std::shared_ptr<ApplicationObject> other, double xOffset, double yOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			toInnerLeftIn(otherPtr, xOffset);
		}
	}

	void Renderable::toInnerRightIn(ApplicationObject* other, double xOffset, double yOffset)
	{
		const auto otherR = dynamic_cast<Renderable*>(other);

		if (otherR != nullptr)
		{
			setPosition(otherR->getRight() - getHalfWidth() - xOffset, otherR->getY() + yOffset);
		}
	}

	void Renderable::toInnerRightIn(std::shared_ptr<ApplicationObject> other, double xOffset, double yOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			toInnerRightIn(otherPtr, xOffset);
		}
	}

	void Renderable::toInnerTopIn(ApplicationObject* other, double xOffset, double yOffset)
	{
		const auto otherR = dynamic_cast<Renderable*>(other);

		if (otherR != nullptr)
		{
			setPosition(otherR->getX() + xOffset, otherR->getTop() + getHalfHeight() + yOffset);
		}
	}

	void Renderable::toInnerTopIn(std::shared_ptr<ApplicationObject> other, double xOffset, double yOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			toInnerTopIn(otherPtr, yOffset);
		}
	}

	void Renderable::toInnerBottomIn(ApplicationObject* other, double xOffset, double yOffset)
	{
		const auto otherR = dynamic_cast<Renderable*>(other);

		if (otherR != nullptr)
		{
			setPosition(otherR->getX() + xOffset, otherR->getBottom() - getHalfHeight() - yOffset);
		}
	}

	void Renderable::toInnerBottomIn(std::shared_ptr<ApplicationObject> other, double xOffset, double yOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			toInnerBottomIn(otherPtr, yOffset);
		}
	}

	void Renderable::toInnerTopLeftIn(ApplicationObject* other, double xOffset, double yOffset)
	{
		const auto otherR = dynamic_cast<Renderable*>(other);

		if (otherR != nullptr)
		{
			setPosition(otherR->getLeft() + getHalfWidth() + xOffset, otherR->getTop() + getHalfHeight() + yOffset);
		}
	}

	void Renderable::toInnerTopLeftIn(std::shared_ptr<ApplicationObject> other, double xOffset, double yOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			toInnerTopLeftIn(otherPtr, xOffset, yOffset);
		}
	}

	void Renderable::toInnerTopRightIn(ApplicationObject* other, double xOffset, double yOffset)
	{
		const auto otherR = dynamic_cast<Renderable*>(other);

		if (otherR != nullptr)
		{
			setPosition(otherR->getRight() - getHalfWidth() - xOffset, otherR->getTop() + getHalfHeight() + yOffset);
		}
	}

	void Renderable::toInnerTopRightIn(std::shared_ptr<ApplicationObject> other, double xOffset, double yOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			toInnerTopRightIn(otherPtr, xOffset, yOffset);
		}
	}

	void Renderable::toInnerBottomLeftIn(ApplicationObject* other, double xOffset, double yOffset)
	{
		const auto otherR = dynamic_cast<Renderable*>(other);

		if (otherR != nullptr)
		{
			setPosition(otherR->getLeft() + getHalfWidth() + xOffset, otherR->getBottom() - getHalfHeight() - yOffset);
		}
	}

	void Renderable::toInnerBottomLeftIn(std::shared_ptr<ApplicationObject> other, double xOffset, double yOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			toInnerBottomLeftIn(otherPtr, xOffset, yOffset);
		}
	}

	void Renderable::toInnerBottomRightIn(ApplicationObject* other, double xOffset, double yOffset)
	{
		const auto otherR = dynamic_cast<Renderable*>(other);

		if (otherR != nullptr)
		{
			setPosition(otherR->getRight() - getHalfWidth() - xOffset, otherR->getBottom() - getHalfHeight() - yOffset);
		}
	}

	void Renderable::toInnerBottomRightIn(std::shared_ptr<ApplicationObject> other, double xOffset, double yOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			toInnerBottomRightIn(otherPtr, xOffset, yOffset);
		}
	}

	std::shared_ptr<SerializationClient> Renderable::doSerialize(SerializationHint hint)
	{
		return nullptr;
	}

	void Renderable::doManualSerialize(SerializationHint hint, std::shared_ptr<SerializationClient> injectedClient)
	{
		auto client = injectedClient->getClient("__renderable__", hint);

		setX(client->serializeDouble("x", getX()));
		setY(client->serializeDouble("y", getY()));
		setWidth(client->serializeDouble("w", rect.w));
		setHeight(client->serializeDouble("h", rect.h));
		setRotation(client->serializeDouble("r", getRotation()));
		setAlpha(client->serializeDouble("al", Renderable::getAlpha()));
		setScaleX(client->serializeDouble("sX", getScaleX()));
		setScaleY(client->serializeDouble("sY", getScaleY()));

		if (colorModulation == nullptr && client->getInt("cm.r", -1) != -1)
		{
			colorModulation = std::make_shared<Color>();
		}

		if (colorModulation != nullptr)
		{
			colorModulation->r = client->serializeInt("cm.r", colorModulation->r);
			colorModulation->g = client->serializeInt("cm.g", colorModulation->g);
			colorModulation->b = client->serializeInt("cm.b", colorModulation->b);
			colorModulation->a = client->serializeInt("cm.a", colorModulation->a);
		}

	}
}
