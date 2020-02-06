#include "Renderable.h"

namespace AW
{
	const float Renderable::scalingFactor = 200.f;

	float Renderable::screenToWorldPosition(float screen)
	{
		return screen / scalingFactor;
	}

	float Renderable::worldToScreenPosition(float world)
	{
		return world * scalingFactor;
	}

	float Renderable::screenToWorldRotation(float degrees)
	{
		return degrees * -(float)(AW::NumberHelper::PI / 180.0);
	}

	float Renderable::worldToScreenRotation(float radians)
	{
		return radians * -(float)(180.0 / AW::NumberHelper::PI);
	}

	AWVec2<float> Renderable::worldToScreen(const AWVec2<float>& world)
	{
		return AWVec2<float>(worldToScreenPosition(world.x), -worldToScreenPosition(world.y));
	}

	AWVec2<float> Renderable::screenToWorld(const AWVec2<float>& screen)
	{
		return AWVec2<float>(screenToWorldPosition(screen.x), -screenToWorldPosition(screen.y));
	}

	AWVec2<float> Renderable::worldToScreen(const AWVec2<double>& world)
	{
		return AWVec2<float>(worldToScreenPosition((float)world.x), -worldToScreenPosition((float)world.y));
	}

	AWVec2<float> Renderable::screenToWorld(const AWVec2<double>& screen)
	{
		return AWVec2<float>(screenToWorldPosition((float)screen.x), -screenToWorldPosition((float)screen.y));
	}

	const std::shared_ptr<ShaderReference>& Renderable::getVertexShader()
	{
		return vertexShader;
	}

	const std::shared_ptr<ShaderReference>& Renderable::getFragmentShader()
	{
		return fragmentShader;
	}

	const std::shared_ptr<ShaderReference>& Renderable::getClipRectVertexShader()
	{
		return clipRectVertexShader;
	}

	const std::shared_ptr<ShaderReference>& Renderable::getClipRectFragmentShader()
	{
		return clipRectFragmentShader;
	}

	void Renderable::setVertexShader(std::shared_ptr<ShaderReference> shader)
	{
		vertexShader = shader;
	}

	void Renderable::setFragmentShader(std::shared_ptr<ShaderReference> shader)
	{
		fragmentShader = shader;
	}

	void Renderable::setClipRectVertexShader(std::shared_ptr<ShaderReference> shader)
	{
		clipRectVertexShader = shader;
	}

	void Renderable::setClipRectFragmentShader(std::shared_ptr<ShaderReference> shader)
	{
		clipRectFragmentShader = shader;
	}

	void Renderable::setColor(int r, int g, int b, int a)
	{
		if (colorModulation == nullptr)
		{
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

	void Renderable::setColor(const Color& color)
	{
		setColor(color.r, color.g, color.b, color.a);
	}

	void Renderable::setColor(const Color* color)
	{
		if (color != nullptr)
		{
			setColor(color->r, color->g, color->b, color->a);
		}
	}

	Color* Renderable::getColor()
	{
		return colorModulation != nullptr ? colorModulation.get() : nullptr;
	}

	Rect Renderable::getRect()
	{
		return rect;
	}

	void Renderable::setWorldRectFromScreenRect(Rect* r)
	{
		worldRect.x = (double)Renderable::screenToWorldPosition((float)r->x);
		worldRect.y = (double)Renderable::screenToWorldPosition((float)r->y);
		worldRect.w = (double)Renderable::screenToWorldPosition((float)r->w);
		worldRect.h = (double)Renderable::screenToWorldPosition((float)r->h);
	}

	void Renderable::setScreenRect(Rect* r)
	{
		screenRect = r;
	}

	const Rect* Renderable::getWorldRect() const
	{
		return &worldRect;
	}

	const Rect* Renderable::getScreenRect() const
	{
		return &screenRect;
	}

	Rect* Renderable::getCollisionRect()
	{
		return &worldRect;
	}

	bool Renderable::getHasClipRect()
	{
		return hasClipRect;
	}

	const Rect* Renderable::getClipRect()
	{
		return &clipRect;
	}

	void Renderable::setClipRect(const Rect* rect)
	{
		clipRect = *rect;
		hasClipRect = (clipRect.x > 0.0 || clipRect.y > 0.0 || clipRect.w > 0.0 || clipRect.h > 0.0);
	}

	void Renderable::setClipRect(const Rect& rect)
	{
		setClipRect(&rect);
	}

	float Renderable::getWorldX() const
	{
		return Renderable::screenToWorldPosition((float)rect.x);
	}

	void Renderable::setWorldX(float newX)
	{
		rect.x = Renderable::worldToScreenPosition(newX);
	}

	double Renderable::getScreenX() const
	{
		return rect.x;
	}

	void Renderable::setScreenX(double newX)
	{
		rect.x = newX;
	}

	float Renderable::getWorldY() const
	{
		return -Renderable::screenToWorldPosition((float)rect.y);
	}

	void Renderable::setWorldY(float newY)
	{
		rect.y = Renderable::worldToScreenPosition(newY);
	}

	double Renderable::getScreenY() const
	{
		return rect.y;
	}

	void Renderable::setScreenY(double newY)
	{
		rect.y = newY;
	}

	float Renderable::getWorldWidth() const
	{
		return Renderable::screenToWorldPosition((float)rect.w);
	}

	void Renderable::setWorldWidth(float newWidth)
	{
		rect.w = Renderable::worldToScreenPosition(newWidth);
	}

	double Renderable::getScreenWidth() const
	{
		return rect.w;
	}

	void Renderable::setScreenWidth(double newWidth)
	{
		rect.w = newWidth;
	}

	float Renderable::getWorldHeight() const
	{
		return Renderable::screenToWorldPosition((float)rect.h);
	}

	void Renderable::setWorldHeight(float newHeight)
	{
		rect.h = Renderable::worldToScreenPosition(newHeight);
	}

	double Renderable::getScreenHeight() const
	{
		return rect.h;
	}

	void Renderable::setScreenHeight(double newHeight)
	{
		rect.h = newHeight;
	}

	double Renderable::getAlpha()
	{
		return alpha;
	}

	void Renderable::setAlpha(double newAlpha)
	{
		alpha = AW::NumberHelper::clamp(newAlpha, 0.0, 1.0);
	}

	void Renderable::onTransitionFrame(double p, const Rect& targetRect, double targetAlpha, int transitionId)
	{
		setScreenPositionSize(targetRect);
		setAlpha(targetAlpha);
	}

	void Renderable::setScreenPosition(const std::shared_ptr<Renderable>& other)
	{
		setScreenX(other->getScreenX());
		setScreenY(other->getScreenY());
	}

	void Renderable::setScreenPosition(double x, double y)
	{
		setScreenX(x);
		setScreenY(y);
	}

	void Renderable::moveScreenPosition(double xDelta, double yDelta)
	{
		setScreenX(rect.x + xDelta);
		setScreenY(rect.y + yDelta);
	}

	void Renderable::setScreenSize(double width, double height)
	{
		setScreenWidth(width);
		setScreenHeight(height);
	}

	void Renderable::setScreenPositionSize(double x, double y, double width, double height)
	{
		setScreenPosition(x, y);
		setScreenSize(width, height);
	}

	void Renderable::setScreenPositionSize(const Rect& rect)
	{
		setScreenPositionSize(rect.x, rect.y, rect.w, rect.h);
	}

	void Renderable::setScale(double scale)
	{
		this->scale = scale;
	}

	double Renderable::getScale()
	{
		return scale;
	}

	void Renderable::rotateScreen(double rotDelta)
	{
		setScreenRotation(rot + rotDelta);
	}

	double Renderable::getWorldRotation()
	{
		return -Renderable::screenToWorldRotation((float)rot);
	}

	void Renderable::setWorldRotation(float newRotation)
	{
		rot = Renderable::worldToScreenRotation(newRotation);
	}

	double Renderable::getScreenRotation()
	{
		return rot;
	}

	void Renderable::setScreenRotation(double newRotation)
	{
		rot = newRotation;
	}

	void Renderable::setWorldPosition(float x, float y)
	{
		setScreenPosition(
			Renderable::worldToScreenPosition(x),
			-Renderable::worldToScreenPosition(y)
		);
	}

	void Renderable::setWorldSize(float width, float height)
	{
		setScreenSize(
			Renderable::worldToScreenPosition(width),
			Renderable::worldToScreenPosition(height)
		);
	}

	void Renderable::setWorldPositionAndSize(float x, float y, float width, float height)
	{
		setWorldPosition(x, y);
		setWorldSize(width, height);
	}

	void Renderable::rotateWorld(float radians)
	{
		rotateScreen(Renderable::worldToScreenRotation(radians));
	}

	double Renderable::getScreenLeft() const
	{
		return getScreenX() - getScreenHalfWidth();
	}

	double Renderable::getScreenRight() const
	{
		return getScreenX() + getScreenHalfWidth();
	}

	double Renderable::getScreenTop() const
	{
		return getScreenY() - getScreenHalfHeight();
	}

	double Renderable::getScreenBottom() const
	{
		return getScreenY() + getScreenHalfHeight();
	}

	double Renderable::getScreenHalfWidth() const
	{
		return getScreenWidth() / 2.0;
	}

	double Renderable::getScreenHalfHeight() const
	{
		return getScreenHeight() / 2.0;
	}

	float Renderable::getWorldLeft() const
	{
		return getWorldX() - getWorldHalfWidth();
	}

	float Renderable::getWorldRight() const
	{
		return getWorldX() + getWorldHalfWidth();
	}

	float Renderable::getWorldTop() const
	{
		return getWorldY() + getWorldHalfHeight();
	}

	float Renderable::getWorldBottom() const
	{
		return getWorldY() - getWorldHalfHeight();
	}

	float Renderable::getWorldHalfWidth() const
	{
		return getWorldWidth() / 2.f;
	}

	float Renderable::getWorldHalfHeight() const
	{
		return getWorldHeight() / 2.f;
	}

	void Renderable::topLeftAlignSelf(float xOffset, float yOffset)
	{
		if (layoutSpace == LayoutSpace::Screen)
			setScreenPosition(getScreenHalfWidth() + xOffset, getScreenHalfHeight() + yOffset);
		else
			setWorldPosition(getWorldHalfWidth() + xOffset, getWorldHalfHeight() - yOffset);
	}

	void Renderable::centerWithin(const Renderable* other, float xOffset, float yOffset)
	{
		if (other != nullptr)
		{
			if (layoutSpace == LayoutSpace::Screen)
				setScreenPosition(other->getScreenX() + xOffset, other->getScreenY() + yOffset);
			else
				setWorldPosition(other->getWorldX() + xOffset, other->getWorldY() - yOffset);
		}
	}

	void Renderable::centerAlignWithin(const Renderable* other, float xOffset, float yOffset)
	{
		if (other != nullptr)
		{
			if (layoutSpace == LayoutSpace::Screen)
				setScreenPosition(other->getScreenHalfWidth() + xOffset, other->getScreenHalfHeight() + yOffset);
			else
				setWorldPosition(other->getWorldHalfWidth() + xOffset, other->getWorldHalfHeight() - yOffset);
		}
	}

	void Renderable::matchPosition(const Renderable* other, float xOffset, float yOffset)
	{
		if (other != nullptr)
		{
			if (layoutSpace == LayoutSpace::Screen)
				setScreenPosition(other->getScreenX() + xOffset, other->getScreenY() + yOffset);
			else
				setWorldPosition(other->getWorldX() + xOffset, other->getWorldY() - yOffset);
		}
	}

	void Renderable::toLeftOf(const Renderable* other, float xOffset, float yOffset)
	{
		if (other != nullptr)
		{
			if (layoutSpace == LayoutSpace::Screen)
				setScreenPosition(other->getScreenLeft() - getScreenHalfWidth() - xOffset, other->getScreenY() + yOffset);
			else
				setWorldPosition(other->getWorldLeft() - getWorldHalfWidth() - xOffset, other->getWorldY() - yOffset);
		}
	}

	void Renderable::toLeftTopOf(const Renderable* other, float xOffset, float yOffset)
	{
		if (other != nullptr)
		{
			if (layoutSpace == LayoutSpace::Screen)
				setScreenPosition(other->getScreenLeft() - getScreenHalfWidth() - xOffset, other->getScreenTop() + getScreenHalfHeight() + yOffset);
			else
				setWorldPosition(other->getWorldLeft() - getWorldHalfWidth() - xOffset, other->getWorldTop() - getWorldHalfHeight() - yOffset);
		}
	}

	void Renderable::toLeftBottomOf(const Renderable* other, float xOffset, float yOffset)
	{
		if (other != nullptr)
		{
			if (layoutSpace == LayoutSpace::Screen)
				setScreenPosition(other->getScreenLeft() - getScreenHalfWidth() - xOffset, other->getScreenBottom() - getScreenHalfHeight() - yOffset);
			else
				setWorldPosition(other->getWorldLeft() - getWorldHalfWidth() - xOffset, other->getWorldBottom() + getWorldHalfHeight() + yOffset);
		}
	}

	void Renderable::toRightOf(const Renderable* other, float xOffset, float yOffset)
	{
		if (other != nullptr)
		{
			if (layoutSpace == LayoutSpace::Screen)
				setScreenPosition(other->getScreenRight() + getScreenHalfWidth() + xOffset, other->getScreenY() + yOffset);
			else
				setWorldPosition(other->getWorldRight() + getWorldHalfWidth() + xOffset, other->getWorldY() - yOffset);
		}
	}

	void Renderable::toRightTopOf(const Renderable* other, float xOffset, float yOffset)
	{
		if (other != nullptr)
		{
			if (layoutSpace == LayoutSpace::Screen)
				setScreenPosition(other->getScreenRight() + getScreenHalfWidth() + xOffset, other->getScreenTop() + getScreenHalfHeight() + yOffset);
			else
				setWorldPosition(other->getWorldRight() + getWorldHalfWidth() + xOffset, other->getWorldTop() - getWorldHalfHeight() - yOffset);
		}
	}

	void Renderable::toRightBottomOf(const Renderable* other, float xOffset, float yOffset)
	{
		if (other != nullptr)
		{
			if (layoutSpace == LayoutSpace::Screen)
				setScreenPosition(other->getScreenRight() + getScreenHalfWidth() + xOffset, other->getScreenBottom() - getScreenHalfHeight() - yOffset);
			else
				setWorldPosition(other->getWorldRight() + getWorldHalfWidth() + xOffset, other->getWorldBottom() + getWorldHalfHeight() + yOffset);
		}
	}

	void Renderable::toTopOf(const Renderable* other, float xOffset, float yOffset)
	{
		if (other != nullptr)
		{
			if (layoutSpace == LayoutSpace::Screen)
				setScreenPosition(other->getScreenX() + xOffset, other->getScreenTop() - getScreenHalfHeight() - yOffset);
			else
				setWorldPosition(other->getWorldX() + xOffset, other->getWorldTop() + getWorldHalfHeight() + yOffset);
		}
	}

	void Renderable::toTopLeftOf(const Renderable* other, float xOffset, float yOffset)
	{
		if (other != nullptr)
		{
			if (layoutSpace == LayoutSpace::Screen)
				setScreenPosition(other->getScreenLeft() + getScreenHalfWidth() + xOffset, other->getScreenTop() - getScreenHalfHeight() - yOffset);
			else
				setWorldPosition(other->getWorldLeft() + getWorldHalfWidth() + xOffset, other->getWorldTop() + getWorldHalfHeight() + yOffset);
		}
	}

	void Renderable::toTopRightOf(const Renderable* other, float xOffset, float yOffset)
	{
		if (other != nullptr)
		{
			if (layoutSpace == LayoutSpace::Screen)
				setScreenPosition(other->getScreenRight() - getScreenHalfWidth() - xOffset, other->getScreenTop() - getScreenHalfHeight() - yOffset);
			else
				setWorldPosition(other->getWorldRight() - getWorldHalfWidth() - xOffset, other->getWorldTop() + getWorldHalfHeight() + yOffset);
		}
	}

	void Renderable::toBottomOf(const Renderable* other, float xOffset, float yOffset)
	{
		if (other != nullptr)
		{
			if (layoutSpace == LayoutSpace::Screen)
				setScreenPosition(other->getScreenX() + xOffset, other->getScreenBottom() + getScreenHalfHeight() + yOffset);
			else
				setWorldPosition(other->getWorldX() + xOffset, other->getWorldBottom() - getWorldHalfHeight() - yOffset);
		}
	}

	void Renderable::toBottomRightOf(const Renderable* other, float xOffset, float yOffset)
	{
		if (other != nullptr)
		{
			if (layoutSpace == LayoutSpace::Screen)
				setScreenPosition(other->getScreenRight() - getScreenHalfWidth() - xOffset, other->getScreenBottom() + getScreenHalfHeight() + yOffset);
			else
				setWorldPosition(other->getWorldRight() - getWorldHalfWidth() - xOffset, other->getWorldBottom() - getWorldHalfHeight() - yOffset);
		}
	}

	void Renderable::toBottomLeftOf(const Renderable* other, float xOffset, float yOffset)
	{
		if (other != nullptr)
		{
			if (layoutSpace == LayoutSpace::Screen)
				setScreenPosition(other->getScreenLeft() + getScreenHalfWidth() + xOffset, other->getScreenBottom() + getScreenHalfHeight() + yOffset);
			else
				setWorldPosition(other->getWorldLeft() + getWorldHalfWidth() + xOffset, other->getWorldBottom() - getWorldHalfHeight() - yOffset);
		}
	}

	void Renderable::setSize(const Renderable* other, float wOffset, float hOffset)
	{
		if (other != nullptr)
		{
			if (layoutSpace == LayoutSpace::Screen)
				setScreenSize(other->getScreenWidth() + wOffset, other->getScreenHeight() + hOffset);
			else
				setWorldSize(other->getWorldWidth() + wOffset, other->getWorldHeight() + hOffset);
		}
	}

	void Renderable::setSizeAndCenter(const Renderable* other, float xOffset, float yOffset, float wOffset, float hOffset)
	{
		if (other != nullptr)
		{
			if (layoutSpace == LayoutSpace::Screen)
				setScreenPositionSize(other->getScreenHalfWidth() + xOffset, other->getScreenHalfHeight() + yOffset, other->getScreenWidth() + wOffset * 2.f, other->getScreenHeight() + hOffset * 2.f);
			else
				setWorldPositionAndSize(other->getWorldHalfWidth() + xOffset, other->getWorldHalfHeight() + yOffset, other->getWorldWidth() + wOffset * 2.f, other->getWorldHeight() + hOffset * 2.f);
		}
	}

	void Renderable::leftAlign(const Renderable* other, float xOffset, float yOffset)
	{
		if (other != nullptr)
		{
			if (layoutSpace == LayoutSpace::Screen)
				setScreenPosition(other->getScreenLeft() + getScreenHalfWidth() + xOffset, other->getScreenY() + yOffset);
			else
				setWorldPosition(other->getWorldLeft() + getWorldHalfWidth() + xOffset, other->getWorldY() + yOffset);
		}
	}

	void Renderable::toInnerLeftIn(const Renderable* other, float xOffset, float yOffset)
	{
		if (other != nullptr)
		{
			if (layoutSpace == LayoutSpace::Screen)
				setScreenPosition(other->getScreenLeft() + getScreenHalfWidth() + xOffset, other->getScreenY() + yOffset);
			else
				setWorldPosition(other->getWorldLeft() + getWorldHalfWidth() + xOffset, other->getWorldY() - yOffset);
		}
	}

	void Renderable::toInnerRightIn(const Renderable* other, float xOffset, float yOffset)
	{
		if (other != nullptr)
		{
			if (layoutSpace == LayoutSpace::Screen)
				setScreenPosition(other->getScreenRight() - getScreenHalfWidth() - xOffset, other->getScreenY() + yOffset);
			else
				setWorldPosition(other->getWorldRight() - getWorldHalfWidth() - xOffset, other->getWorldY() - yOffset);
		}
	}

	void Renderable::toInnerTopIn(const Renderable* other, float xOffset, float yOffset)
	{
		if (other != nullptr)
		{
			if (layoutSpace == LayoutSpace::Screen)
				setScreenPosition(other->getScreenX() + xOffset, other->getScreenTop() + getScreenHalfHeight() + yOffset);
			else
				setWorldPosition(other->getWorldX() + xOffset, other->getWorldTop() - getWorldHalfHeight() - yOffset);
		}
	}

	void Renderable::toInnerBottomIn(const Renderable* other, float xOffset, float yOffset)
	{
		if (other != nullptr)
		{
			if (layoutSpace == LayoutSpace::Screen)
				setScreenPosition(other->getScreenX() + xOffset, other->getScreenBottom() - getScreenHalfHeight() - yOffset);
			else
				setWorldPosition(other->getWorldX() + xOffset, other->getWorldBottom() + getWorldHalfHeight() + yOffset);
		}
	}

	void Renderable::toInnerTopLeftIn(const Renderable* other, float xOffset, float yOffset)
	{
		if (other != nullptr)
		{
			if (layoutSpace == LayoutSpace::Screen)
				setScreenPosition(other->getScreenLeft() + getScreenHalfWidth() + xOffset, other->getScreenTop() + getScreenHalfHeight() + yOffset);
			else
				setWorldPosition(other->getWorldLeft() + getWorldHalfWidth() + xOffset, other->getWorldTop() - getWorldHalfHeight() - yOffset);
		}
	}

	void Renderable::toInnerTopRightIn(const Renderable* other, float xOffset, float yOffset)
	{
		if (other != nullptr)
		{
			if (layoutSpace == LayoutSpace::Screen)
				setScreenPosition(other->getScreenRight() - getScreenHalfWidth() - xOffset, other->getScreenTop() + getScreenHalfHeight() + yOffset);
			else
				setWorldPosition(other->getWorldRight() - getWorldHalfWidth() - xOffset, other->getWorldTop() - getWorldHalfHeight() - yOffset);
		}
	}

	void Renderable::toInnerBottomLeftIn(const Renderable* other, float xOffset, float yOffset)
	{
		if (other != nullptr)
		{
			if (layoutSpace == LayoutSpace::Screen)
				setScreenPosition(other->getScreenLeft() + getScreenHalfWidth() + xOffset, other->getScreenBottom() - getScreenHalfHeight() - yOffset);
			else
				setWorldPosition(other->getWorldLeft() + getWorldHalfWidth() + xOffset, other->getWorldBottom() + getWorldHalfHeight() + yOffset);
		}
	}

	void Renderable::toInnerBottomRightIn(const Renderable* other, float xOffset, float yOffset)
	{
		if (other != nullptr)
		{
			if (layoutSpace == LayoutSpace::Screen)
				setScreenPosition(other->getScreenRight() - getScreenHalfWidth() - xOffset, other->getScreenBottom() - getScreenHalfHeight() - yOffset);
			else
				setWorldPosition(other->getWorldRight() - getWorldHalfWidth() - xOffset, other->getWorldBottom() + getWorldHalfHeight() + yOffset);
		}
	}

	void Renderable::centerWithin(const std::shared_ptr<Renderable>& other, float xOffset, float yOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			centerWithin(otherPtr, xOffset, yOffset);
		}
	}

	void Renderable::centerAlignWithin(const std::shared_ptr<Renderable>& other, float xOffset, float yOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			centerAlignWithin(otherPtr, xOffset, yOffset);
		}
	}

	void Renderable::toLeftOf(const std::shared_ptr<Renderable>& other, float xOffset, float yOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			toLeftOf(otherPtr, xOffset, yOffset);
		}
	}

	void Renderable::matchPosition(const std::shared_ptr<Renderable>& other, float xOffset, float yOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			matchPosition(otherPtr, xOffset, yOffset);
		}
	}

	void Renderable::toLeftTopOf(const std::shared_ptr<Renderable>& other, float xOffset, float yOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			toLeftTopOf(otherPtr, xOffset, yOffset);
		}
	}

	void Renderable::toLeftBottomOf(const std::shared_ptr<Renderable>& other, float xOffset, float yOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			toLeftBottomOf(otherPtr, xOffset, yOffset);
		}
	}

	void Renderable::toRightOf(const std::shared_ptr<Renderable>& other, float xOffset, float yOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			toRightOf(otherPtr, xOffset, yOffset);
		}
	}

	void Renderable::toRightTopOf(const std::shared_ptr<Renderable>& other, float xOffset, float yOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			toRightTopOf(otherPtr, xOffset, yOffset);
		}
	}

	void Renderable::toRightBottomOf(const std::shared_ptr<Renderable>& other, float xOffset, float yOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			toRightBottomOf(otherPtr, xOffset, yOffset);
		}
	}

	void Renderable::toTopOf(const std::shared_ptr<Renderable>& other, float xOffset, float yOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			toTopOf(otherPtr, xOffset, yOffset);
		}
	}

	void Renderable::toTopLeftOf(const std::shared_ptr<Renderable>& other, float xOffset, float yOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			toTopLeftOf(otherPtr, xOffset, yOffset);
		}
	}

	void Renderable::toTopRightOf(const std::shared_ptr<Renderable>& other, float xOffset, float yOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			toTopRightOf(otherPtr, xOffset, yOffset);
		}
	}

	void Renderable::toBottomOf(const std::shared_ptr<Renderable>& other, float xOffset, float yOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			toBottomOf(otherPtr, xOffset, yOffset);
		}
	}

	void Renderable::toBottomLeftOf(const std::shared_ptr<Renderable>& other, float xOffset, float yOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			toBottomLeftOf(otherPtr, xOffset, yOffset);
		}
	}

	void Renderable::toBottomRightOf(const std::shared_ptr<Renderable>& other, float xOffset, float yOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			toBottomRightOf(otherPtr, xOffset, yOffset);
		}
	}

	void Renderable::setSize(const std::shared_ptr<Renderable>& other, float wOffset, float hOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			setSize(otherPtr, wOffset, hOffset);
		}
	}

	void Renderable::setSizeAndCenter(const std::shared_ptr<Renderable>& other, float xOffset, float yOffset, float wOffset, float hOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			setSizeAndCenter(otherPtr, xOffset, yOffset, wOffset, hOffset);
		}
	}

	void Renderable::leftAlign(const std::shared_ptr<Renderable>& other, float xOffset, float yOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			leftAlign(otherPtr, xOffset, yOffset);
		}
	}

	void Renderable::toInnerLeftIn(const std::shared_ptr<Renderable>& other, float xOffset, float yOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			toInnerLeftIn(otherPtr, xOffset, yOffset);
		}
	}

	void Renderable::toInnerRightIn(const std::shared_ptr<Renderable>& other, float xOffset, float yOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			toInnerRightIn(otherPtr, xOffset, yOffset);
		}
	}

	void Renderable::toInnerTopIn(const std::shared_ptr<Renderable>& other, float xOffset, float yOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			toInnerTopIn(otherPtr, xOffset, yOffset);
		}
	}

	void Renderable::toInnerBottomIn(const std::shared_ptr<Renderable>& other, float xOffset, float yOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			toInnerBottomIn(otherPtr, xOffset, yOffset);
		}
	}

	void Renderable::toInnerTopLeftIn(const std::shared_ptr<Renderable>& other, float xOffset, float yOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			toInnerTopLeftIn(otherPtr, xOffset, yOffset);
		}
	}

	void Renderable::toInnerTopRightIn(const std::shared_ptr<Renderable>& other, float xOffset, float yOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			toInnerTopRightIn(otherPtr, xOffset, yOffset);
		}
	}

	void Renderable::toInnerBottomLeftIn(const std::shared_ptr<Renderable>& other, float xOffset, float yOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			toInnerBottomLeftIn(otherPtr, xOffset, yOffset);
		}
	}

	void Renderable::toInnerBottomRightIn(const std::shared_ptr<Renderable>& other, float xOffset, float yOffset)
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

		setScreenX(client->serializeDouble("x", getScreenX()));
		setScreenY(client->serializeDouble("y", getScreenY()));
		setScreenWidth(client->serializeDouble("w", rect.w));
		setScreenHeight(client->serializeDouble("h", rect.h));
		setScreenRotation(client->serializeDouble("r", getScreenRotation()));
		setAlpha(client->serializeDouble("al", Renderable::getAlpha()));
		setScale(client->serializeDouble("scale", getScale()));

		visible = client->serializeBool("visible", visible);
		renderMode = (RenderMode)client->serializeInt("r-mode", (int)renderMode);
		renderPositionMode = (RenderPositionMode)client->serializeInt("r-p-m", (int)renderPositionMode);
		renderPositionProcessing = (RenderPositionProcessingMode)client->serializeInt("r-p-p-m", (int)renderPositionProcessing);
		renderTextureMode = (RenderTextureMode)client->serializeInt("r-t-m", (int)renderTextureMode);
		renderDepthTest = (RenderDepthTestMode)client->serializeInt("r-d-t-e", (int)renderDepthTest);
		renderMultiSampleMode = (RenderMultiSampleMode)client->serializeInt("r-m-s-m", (int)renderMultiSampleMode);
		renderUpdateMode = (RenderUpdateMode)client->serializeInt("r-u-m", (int)renderUpdateMode);
		renderTarget = (RenderTargetMode)client->serializeInt("r-target", (int)renderTarget);
		renderColorMode = (RenderColorMode)client->serializeInt("r-color-m", (int)renderColorMode);
		layoutSpace = (LayoutSpace)client->serializeInt("r-layout-s", (int)layoutSpace);

		clipRect.x = client->serializeDouble("cr-x", clipRect.x);
		clipRect.y = client->serializeDouble("cr-y", clipRect.y);
		clipRect.w = client->serializeDouble("cr-w", clipRect.w);
		clipRect.h = client->serializeDouble("cr-h", clipRect.h);
		setClipRect(clipRect);

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


	std::string Renderable::getLuaBindingId()
	{
		return std::string();
	}

	void Renderable::onRegisterLuaHooks(std::shared_ptr<Lua>& lua, const std::shared_ptr<ILuaObject>& obj)
	{
		lua->registerBoundFunction("setColor", obj);
		lua->registerBoundFunction("setShaderUniform", obj);
		lua->registerBoundFunction("setShaderUniformV2", obj);
		lua->registerBoundFunction("setShaderUniformV3", obj);
		lua->registerBoundFunction("setShaderUniformV4", obj);
		lua->registerBoundFunction("setPosition", obj);
		lua->registerBoundFunction("setSize", obj);
	}

	void Renderable::onLuaCallback(const std::string& func, LuaBoundObject* obj)
	{
		if (func == "setColor" && obj->args.size() == 3)
			setColor(
				std::stoi(obj->args[0]),
				std::stoi(obj->args[1]),
				std::stoi(obj->args[2])
			);

		else if (func == "setColor" && obj->args.size() == 4)
			setColor(
				std::stoi(obj->args[0]),
				std::stoi(obj->args[1]),
				std::stoi(obj->args[2]),
				std::stoi(obj->args[3])
			);

		else if (func == "setShaderUniform" && obj->args.size() == 2 && fragmentShader != nullptr)
			fragmentShader->setFloatIUParam(obj->args[0], std::stod(obj->args[1]));

		else if (func == "setShaderUniformV2" && obj->args.size() == 3 && fragmentShader != nullptr)
			fragmentShader->setFloatV2IUParam(obj->args[0], std::stod(obj->args[1]), std::stod(obj->args[2]));

		else if (func == "setShaderUniformV3" && obj->args.size() == 4 && fragmentShader != nullptr)
			fragmentShader->setFloatV3IUParam(obj->args[0], std::stod(obj->args[1]), std::stod(obj->args[2]), std::stod(obj->args[3]));

		else if (func == "setShaderUniformV4" && obj->args.size() == 5 && fragmentShader != nullptr)
			fragmentShader->setFloatV4IUParam(obj->args[0], std::stod(obj->args[1]), std::stod(obj->args[2]), std::stod(obj->args[3]), std::stod(obj->args[4]));

		else if (func == "setPosition" && obj->args.size() == 2)
			setScreenPosition(std::stod(obj->args[0]), std::stod(obj->args[1]));

		else if (func == "setSize" && obj->args.size() == 2)
			setScreenSize(std::stod(obj->args[0]), std::stod(obj->args[1]));
	}

	void Renderable::markDirty()
	{
		dirty = true;
	}

	void Renderable::markClean()
	{
		dirty = false;
	}

	bool Renderable::isClean()
	{
		return renderUpdateMode == RenderUpdateMode::WhenDirty && !dirty;
	}
}
