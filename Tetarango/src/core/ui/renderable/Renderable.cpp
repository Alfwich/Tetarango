#include "Renderable.h"

#include "util/NumberHelper.h"
#include "engine/module/physic/RigidBody.h"

#define RendManagedLayoutOp(x) x

namespace AW
{
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
		worldRect.x = (double)RigidBody::screenToWorldPosition((float)r->x);
		worldRect.y = (double)RigidBody::screenToWorldPosition((float)r->y);
		worldRect.w = (double)RigidBody::screenToWorldPosition((float)r->w);
		worldRect.h = (double)RigidBody::screenToWorldPosition((float)r->h);
	}

	void Renderable::setScreenRect(Rect* r)
	{
		screenRect = r;
	}

	const Rect* Renderable::getWorldRect()
	{
		return &worldRect;
	}

	const Rect* Renderable::getScreenRect()
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

	const Rect * Renderable::getClipRect()
	{
		return &clipRect;
	}

	void Renderable::setClipRect(const Rect * rect)
	{
		clipRect = *rect;
		hasClipRect = (clipRect.x > 0.0 || clipRect.y > 0.0 || clipRect.w > 0.0 || clipRect.h > 0.0);
	}

	void Renderable::setClipRect(const Rect & rect)
	{
		setClipRect(&rect);
	}

	float Renderable::getWorldX()
	{
		return RigidBody::screenToWorldPosition((float)rect.x);
	}

	void Renderable::setWorldX(float newX)
	{
		rect.x = RigidBody::worldToScreenPosition(newX);
	}

	double Renderable::getScreenX()
	{
		return rect.x;
	}

	void Renderable::setScreenX(double newX)
	{
		rect.x = newX;
	}

	float Renderable::getWorldY()
	{
		return -RigidBody::screenToWorldPosition((float)rect.y);
	}

	void Renderable::setWorldY(float newY)
	{
		rect.y = RigidBody::worldToScreenPosition(newY);
	}

	double Renderable::getScreenY()
	{
		return rect.y;
	}

	void Renderable::setScreenY(double newY)
	{
		rect.y = newY;
	}

	float Renderable::getWorldWidth()
	{
		return RigidBody::screenToWorldPosition((float)rect.w);
	}

	void Renderable::setWorldWidth(float newWidth)
	{
		rect.w = RigidBody::worldToScreenPosition(newWidth);
	}

	double Renderable::getScreenWidth()
	{
		return rect.w;
	}

	void Renderable::setScreenWidth(double newWidth)
	{
		rect.w = newWidth;
	}

	float Renderable::getWorldHeight()
	{
		return RigidBody::screenToWorldPosition((float)rect.h);
	}

	void Renderable::setWorldHeight(float newHeight)
	{
		rect.h = RigidBody::worldToScreenPosition(newHeight);
	}

	double Renderable::getScreenHeight()
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
		setScreenSizeAndPosition(targetRect);
		setAlpha(targetAlpha);
	}

	void Renderable::setPosition(const std::shared_ptr<Renderable>& other)
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

	void Renderable::setScreenSizeAndPosition(double x, double y, double width, double height)
	{
		setScreenPosition(x, y);
		setScreenSize(width, height);
	}

	void Renderable::setScreenSizeAndPosition(const Rect& rect)
	{
		setScreenSizeAndPosition(rect.x, rect.y, rect.w, rect.h);
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
		return -RigidBody::screenToWorldRotation((float)rot);
	}

	void Renderable::setWorldRotation(float newRotation)
	{
		rot = RigidBody::worldToScreenRotation(newRotation);
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
			RigidBody::worldToScreenPosition(x),
			-RigidBody::worldToScreenPosition(y)
		);
	}

	void Renderable::setWorldSize(float width, float height)
	{
		setScreenSize(
			RigidBody::worldToScreenPosition(width),
			RigidBody::worldToScreenPosition(height)
		);
	}

	void Renderable::rotateWorld(float radians)
	{
		rotateScreen(RigidBody::worldToScreenRotation(radians));
	}

	double Renderable::getLeft()
	{
		return getScreenX() - getHalfWidth();
	}

	double Renderable::getRight()
	{
		return getScreenX() + getHalfWidth();
	}

	double Renderable::getTop()
	{
		return getScreenY() - getHalfHeight();
	}

	double Renderable::getBottom()
	{
		return getScreenY() + getHalfHeight();
	}

	double Renderable::getHalfWidth()
	{
		return getScreenWidth() / 2.0;
	}

	double Renderable::getHalfHeight()
	{
		return getScreenHeight() / 2.0;
	}

	void Renderable::topLeftAlignSelf(double xOffset, double yOffset)
	{
		RendManagedLayoutOp(setScreenPosition(this->getHalfWidth() + xOffset, this->getHalfHeight() + yOffset));
	}

	void Renderable::centerWithin(Renderable* other, double xOffset, double yOffset)
	{
		if (other != nullptr)
		{
			RendManagedLayoutOp(setScreenPosition(other->getScreenX() + xOffset, other->getScreenY() + yOffset));
		}
	}

	void Renderable::centerAlignWithin(Renderable* other, double xOffset, double yOffset)
	{
		if (other != nullptr)
		{
			RendManagedLayoutOp(setScreenPosition(other->getHalfWidth() + xOffset, other->getHalfHeight() + yOffset));
		}
	}

	void Renderable::matchPosition(Renderable * other, double xOffset, double yOffset)
	{
		if (other != nullptr)
		{
			RendManagedLayoutOp(setScreenPosition(other->getScreenX() + xOffset, other->getScreenY() + yOffset));
		}
	}

	void Renderable::toLeftOf(Renderable* other, double xOffset, double yOffset)
	{
		if (other != nullptr)
		{
			RendManagedLayoutOp(setScreenPosition(other->getLeft() - getHalfWidth() - xOffset, other->getScreenY() + yOffset));
		}
	}

	void Renderable::toLeftTopOf(Renderable* other, double xOffset, double yOffset)
	{
		if (other != nullptr)
		{
			RendManagedLayoutOp(setScreenPosition(other->getLeft() - getHalfWidth() - xOffset, other->getTop() + getHalfHeight() + yOffset));
		}
	}

	void Renderable::toLeftBottomOf(Renderable* other, double xOffset, double yOffset)
	{
		if (other != nullptr)
		{
			RendManagedLayoutOp(setScreenPosition(other->getLeft() - getHalfWidth() - xOffset, other->getBottom() - getHalfHeight() - yOffset));
		}
	}

	void Renderable::toRightOf(Renderable* other, double xOffset, double yOffset)
	{
		if (other != nullptr)
		{
			RendManagedLayoutOp(setScreenPosition(other->getRight() + getHalfWidth() + xOffset, other->getScreenY() + yOffset));
		}
	}

	void Renderable::toRightTopOf(Renderable* other, double xOffset, double yOffset)
	{
		if (other != nullptr)
		{
			RendManagedLayoutOp(setScreenPosition(other->getRight() + getHalfWidth() + xOffset, other->getTop() + getHalfHeight() + yOffset));
		}
	}

	void Renderable::toRightBottomOf(Renderable* other, double xOffset, double yOffset)
	{
		if (other != nullptr)
		{
			RendManagedLayoutOp(setScreenPosition(other->getRight() + getHalfWidth() + xOffset, other->getBottom() - getHalfHeight() - yOffset));
		}
	}

	void Renderable::toTopOf(Renderable* other, double xOffset, double yOffset)
	{
		if (other != nullptr)
		{
			RendManagedLayoutOp(setScreenPosition(other->getScreenX() + xOffset, other->getTop() - getHalfHeight() - yOffset));
		}
	}

	void Renderable::toTopLeftOf(Renderable* other, double xOffset, double yOffset)
	{
		if (other != nullptr)
		{
			RendManagedLayoutOp(setScreenPosition(other->getLeft() + getHalfWidth() + xOffset, other->getTop() - getHalfHeight() - yOffset));
		}
	}

	void Renderable::toTopRightOf(Renderable* other, double xOffset, double yOffset)
	{
		if (other != nullptr)
		{
			RendManagedLayoutOp(setScreenPosition(other->getRight() - getHalfWidth() - xOffset, other->getTop() - getHalfHeight() - yOffset));
		}
	}

	void Renderable::toBottomOf(Renderable* other, double xOffset, double yOffset)
	{
		if (other != nullptr)
		{
			RendManagedLayoutOp(setScreenPosition(other->getScreenX() + xOffset, other->getBottom() + getHalfHeight() + yOffset));
		}
	}

	void Renderable::toBottomRightOf(Renderable* other, double xOffset, double yOffset)
	{
		if (other != nullptr)
		{
			RendManagedLayoutOp(setScreenPosition(other->getRight() - getHalfWidth() - xOffset, other->getBottom() + getHalfHeight() + yOffset));
		}
	}

	void Renderable::toBottomLeftOf(Renderable* other, double xOffset, double yOffset)
	{
		if (other != nullptr)
		{
			RendManagedLayoutOp(setScreenPosition(other->getLeft() + getHalfWidth() + xOffset, other->getBottom() + getHalfHeight() + yOffset));
		}
	}

	void Renderable::setSize(Renderable* other, double wOffset, double hOffset)
	{
		if (other != nullptr)
		{
			RendManagedLayoutOp(setScreenSize(other->getScreenWidth() - wOffset, other->getScreenHeight() + hOffset));
		}
	}

	void Renderable::setSizeAndCenter(Renderable* other, double xOffset, double yOffset, double wOffset, double hOffset)
	{
		if (other != nullptr)
		{
			RendManagedLayoutOp(setScreenSizeAndPosition(other->getHalfWidth() + xOffset, other->getHalfHeight() + yOffset, other->getScreenWidth() + wOffset * 2.0, other->getScreenHeight() + hOffset * 2.0));
		}
	}

	void Renderable::centerWithin(std::shared_ptr<Renderable> other, double xOffset, double yOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			centerWithin(otherPtr, xOffset, yOffset);
		}
	}

	void Renderable::centerAlignWithin(std::shared_ptr<Renderable> other, double xOffset, double yOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			centerAlignWithin(otherPtr, xOffset, yOffset);
		}
	}

	void Renderable::toLeftOf(std::shared_ptr<Renderable> other, double xOffset, double yOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			toLeftOf(otherPtr, xOffset, yOffset);
		}
	}

	void Renderable::matchPosition(std::shared_ptr<Renderable> other, double xOffset, double yOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			matchPosition(otherPtr, xOffset, yOffset);
		}
	}

	void Renderable::toLeftTopOf(std::shared_ptr<Renderable> other, double xOffset, double yOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			toLeftTopOf(otherPtr, xOffset, yOffset);
		}
	}

	void Renderable::toLeftBottomOf(std::shared_ptr<Renderable> other, double xOffset, double yOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			toLeftBottomOf(otherPtr, xOffset, yOffset);
		}
	}

	void Renderable::toRightOf(std::shared_ptr<Renderable> other, double xOffset, double yOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			toRightOf(otherPtr, xOffset, yOffset);
		}
	}

	void Renderable::toRightTopOf(std::shared_ptr<Renderable> other, double xOffset, double yOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			toRightTopOf(otherPtr, xOffset, yOffset);
		}
	}

	void Renderable::toRightBottomOf(std::shared_ptr<Renderable> other, double xOffset, double yOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			toRightBottomOf(otherPtr, xOffset, yOffset);
		}
	}

	void Renderable::toTopOf(std::shared_ptr<Renderable> other, double xOffset, double yOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			toTopOf(otherPtr, xOffset, yOffset);
		}
	}

	void Renderable::toTopLeftOf(std::shared_ptr<Renderable> other, double xOffset, double yOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			toTopLeftOf(otherPtr, xOffset, yOffset);
		}
	}

	void Renderable::toTopRightOf(std::shared_ptr<Renderable> other, double xOffset, double yOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			toTopRightOf(otherPtr, xOffset, yOffset);
		}
	}

	void Renderable::toBottomOf(std::shared_ptr<Renderable> other, double xOffset, double yOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			toBottomOf(otherPtr, xOffset, yOffset);
		}
	}

	void Renderable::toBottomLeftOf(std::shared_ptr<Renderable> other, double xOffset, double yOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			toBottomLeftOf(otherPtr, xOffset, yOffset);
		}
	}

	void Renderable::toBottomRightOf(std::shared_ptr<Renderable> other, double xOffset, double yOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			toBottomRightOf(otherPtr, xOffset, yOffset);
		}
	}

	void Renderable::setSize(std::shared_ptr<Renderable> other, double wOffset, double hOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			setSize(otherPtr, wOffset, hOffset);
		}
	}

	void Renderable::setSizeAndCenter(std::shared_ptr<Renderable> other, double xOffset, double yOffset, double wOffset, double hOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			setSizeAndCenter(otherPtr, xOffset, yOffset, wOffset, hOffset);
		}
	}

	void Renderable::leftAlign(Renderable* other, double xOffset, double yOffset)
	{
		if (other != nullptr)
		{
			setScreenPosition(other->getLeft() + getHalfWidth() + xOffset, other->getScreenY() + yOffset);
		}
	}

	void Renderable::leftAlign(std::shared_ptr<Renderable> other, double xOffset, double yOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			leftAlign(otherPtr, xOffset, yOffset);
		}
	}

	void Renderable::toInnerLeftIn(Renderable* other, double xOffset, double yOffset)
	{
		if (other != nullptr)
		{
			setScreenPosition(other->getLeft() + getHalfWidth() + xOffset, other->getScreenY() + yOffset);
		}
	}

	void Renderable::toInnerLeftIn(std::shared_ptr<Renderable> other, double xOffset, double yOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			toInnerLeftIn(otherPtr, xOffset);
		}
	}

	void Renderable::toInnerRightIn(Renderable* other, double xOffset, double yOffset)
	{
		if (other != nullptr)
		{
			setScreenPosition(other->getRight() - getHalfWidth() - xOffset, other->getScreenY() + yOffset);
		}
	}

	void Renderable::toInnerRightIn(std::shared_ptr<Renderable> other, double xOffset, double yOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			toInnerRightIn(otherPtr, xOffset);
		}
	}

	void Renderable::toInnerTopIn(Renderable* other, double xOffset, double yOffset)
	{
		if (other != nullptr)
		{
			setScreenPosition(other->getScreenX() + xOffset, other->getTop() + getHalfHeight() + yOffset);
		}
	}

	void Renderable::toInnerTopIn(std::shared_ptr<Renderable> other, double xOffset, double yOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			toInnerTopIn(otherPtr, yOffset);
		}
	}

	void Renderable::toInnerBottomIn(Renderable* other, double xOffset, double yOffset)
	{
		if (other != nullptr)
		{
			setScreenPosition(other->getScreenX() + xOffset, other->getBottom() - getHalfHeight() - yOffset);
		}
	}

	void Renderable::toInnerBottomIn(std::shared_ptr<Renderable> other, double xOffset, double yOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			toInnerBottomIn(otherPtr, yOffset);
		}
	}

	void Renderable::toInnerTopLeftIn(Renderable* other, double xOffset, double yOffset)
	{
		if (other != nullptr)
		{
			setScreenPosition(other->getLeft() + getHalfWidth() + xOffset, other->getTop() + getHalfHeight() + yOffset);
		}
	}

	void Renderable::toInnerTopLeftIn(std::shared_ptr<Renderable> other, double xOffset, double yOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			toInnerTopLeftIn(otherPtr, xOffset, yOffset);
		}
	}

	void Renderable::toInnerTopRightIn(Renderable* other, double xOffset, double yOffset)
	{
		if (other != nullptr)
		{
			setScreenPosition(other->getRight() - getHalfWidth() - xOffset, other->getTop() + getHalfHeight() + yOffset);
		}
	}

	void Renderable::toInnerTopRightIn(std::shared_ptr<Renderable> other, double xOffset, double yOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			toInnerTopRightIn(otherPtr, xOffset, yOffset);
		}
	}

	void Renderable::toInnerBottomLeftIn(Renderable* other, double xOffset, double yOffset)
	{
		if (other != nullptr)
		{
			setScreenPosition(other->getLeft() + getHalfWidth() + xOffset, other->getBottom() - getHalfHeight() - yOffset);
		}
	}

	void Renderable::toInnerBottomLeftIn(std::shared_ptr<Renderable> other, double xOffset, double yOffset)
	{
		if (other != nullptr)
		{
			const auto otherPtr = other.get();
			toInnerBottomLeftIn(otherPtr, xOffset, yOffset);
		}
	}

	void Renderable::toInnerBottomRightIn(Renderable* other, double xOffset, double yOffset)
	{
		if (other != nullptr)
		{
			setScreenPosition(other->getRight() - getHalfWidth() - xOffset, other->getBottom() - getHalfHeight() - yOffset);
		}
	}

	void Renderable::toInnerBottomRightIn(std::shared_ptr<Renderable> other, double xOffset, double yOffset)
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
