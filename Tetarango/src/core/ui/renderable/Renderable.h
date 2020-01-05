#pragma once

#include <memory>
#include <SDL.h>
#include "engine/module/serialization/ISerializable.h"
#include "util/Rect.h"
#include "util/Color.h"
#include "engine/module/shader/ShaderReference.h"
#include "engine/module/display/RenderPackage.h"

namespace AW
{
	enum class RenderMode
	{
		None,
		ChildrenOnly,
		Element,
		CachedElement,
		Primitive,
		ParticleSystem,
		Container,
		Polygon
	};

	enum class RenderPositionProcessingMode
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
		Absolute,
		AbsoluteSelfOnly
	};

	enum class RenderDepthTestMode
	{
		Unspecified,
		Enabled,
		Disabled
	};

	enum class RenderMultiSampleMode
	{
		Unspecified,
		Enabled,
		Disabled
	};

	enum class RenderUpdateMode
	{
		EveryFrame,
		WhenDirty
	};

	enum class RenderTargetMode
	{
		Screen,
		Background
	};

	enum class RenderColorMode
	{
		Multiplicative,
		Absolute
	};

	enum class LayoutUnit
	{
		Pixel,
		Meter
	};

	class Renderable : public ISerializableDataSubscriber
	{
		double rot = 0.0, alpha = 1.0, scale = 1.0;
		bool hasClipRect = false, dirty = true;

	protected:
		Rect rect, clipRect, worldRect, screenRect;
		std::shared_ptr<Color> colorModulation = nullptr;

		std::shared_ptr<ShaderReference> vertexShader = nullptr, fragmentShader = nullptr, clipRectVertexShader = nullptr, clipRectFragmentShader = nullptr;

	public:
		unsigned int cachedProgramId = 0, cachedClipRectProgram = 0;
		RenderMode renderMode = RenderMode::ChildrenOnly;
		RenderPositionMode renderPositionMode = RenderPositionMode::Unspecified;
		RenderDepthTestMode renderDepthTest = RenderDepthTestMode::Unspecified;
		RenderMultiSampleMode renderMultiSampleMode = RenderMultiSampleMode::Unspecified;
		RenderPositionProcessingMode renderPositionProcessing = RenderPositionProcessingMode::None;
		RenderTextureMode renderTextureMode = RenderTextureMode::LinearNoWrap;
		RenderUpdateMode renderUpdateMode = RenderUpdateMode::EveryFrame;
		RenderTargetMode renderTarget = RenderTargetMode::Screen;
		RenderColorMode renderColorMode = RenderColorMode::Multiplicative;

		LayoutUnit layoutUnit = LayoutUnit::Pixel;

		const std::shared_ptr<ShaderReference>& getVertexShader();
		const std::shared_ptr<ShaderReference>& getFragmentShader();
		const std::shared_ptr<ShaderReference>& getClipRectVertexShader();
		const std::shared_ptr<ShaderReference>& getClipRectFragmentShader();

		virtual void setVertexShader(std::shared_ptr<ShaderReference> shader);
		virtual void setFragmentShader(std::shared_ptr<ShaderReference> shader);
		virtual void setClipRectVertexShader(std::shared_ptr<ShaderReference> shader);
		virtual void setClipRectFragmentShader(std::shared_ptr<ShaderReference> shader);

		bool visible = true, rotateInParentSpace = true, disableCulling = false;

		virtual void setColor(int r, int g, int b, int a = 0xff);
		virtual void setColor(const Color& color);
		virtual void setColor(const Color* color);
		Color* getColor();
		Color debugColor;

		virtual Rect getRect();

		virtual void setWorldRectFromScreenRect(Rect* r);
		virtual void setScreenRect(Rect* r);
		virtual const Rect* getWorldRect();
		virtual const Rect* getScreenRect();

		virtual Rect* getCollisionRect();

		bool getHasClipRect();
		const Rect* getClipRect();
		void setClipRect(const Rect* rect);
		void setClipRect(const Rect& rect);

		virtual float getWorldX();
		virtual float getWorldY();
		virtual float getWorldWidth();
		virtual float getWorldHeight();

		virtual void setWorldX(float newX);
		virtual void setWorldY(float newY);
		virtual void setWorldWidth(float newWidth);
		virtual void setWorldHeight(float newHeight);

		virtual double getScreenX();
		virtual double getScreenY();
		virtual double getScreenWidth();
		virtual double getScreenHeight();

		virtual void setScreenX(double newX);
		virtual void setScreenY(double newY);
		virtual void setScreenWidth(double newWidth);
		virtual void setScreenHeight(double newHeight);

		virtual double getWorldRotation();
		virtual void setWorldRotation(float radians);

		virtual double getScreenRotation();
		virtual void setScreenRotation(double degrees);

		virtual double getAlpha();
		virtual void setAlpha(double newAlpha);

		void setScale(double scale);
		double getScale();

		virtual void onTransitionStart() { /* NO-OP */ };
		virtual void onTransitionFrame(double p, const Rect& targetRect, double targetAlpha, int transitionId);
		virtual void onTransitionEnd() { /* NO-OP */ };

		void setScreenPosition(double x, double y);
		void moveScreenPosition(double x, double y);
		void setScreenSize(double width, double height);
		void setScreenSizeAndPosition(double x, double y, double width, double height);
		void setScreenSizeAndPosition(const Rect& rect);
		void rotateScreen(double degrees);

		void setWorldPosition(float x, float y);
		void setWorldSize(float width, float height);
		void rotateWorld(float radians);

		double getScreenLeft();
		double getScreenRight();
		double getScreenTop();
		double getScreenBottom();
		double getScreenHalfWidth();
		double getScreenHalfHeight();

		void topLeftAlignSelf(double xOffset = 0.0, double yOffset = 0.0);

		void setPosition(const std::shared_ptr<Renderable>& other);

		void centerWithin(Renderable* other, double xOffset = 0.0, double yOffset = 0.0);
		void centerWithin(std::shared_ptr<Renderable> other, double xOffset = 0.0, double yOffset = 0.0);

		void centerAlignWithin(Renderable* other, double xOffset = 0.0, double yOffset = 0.0);
		void centerAlignWithin(std::shared_ptr<Renderable> other, double xOffset = 0.0, double yOffset = 0.0);

		void matchPosition(Renderable* other, double xOffset = 0.0, double yOffset = 0.0);
		void matchPosition(std::shared_ptr<Renderable> other, double xOffset = 0.0, double yOffset = 0.0);

		void toLeftOf(Renderable* other, double xPadding = 0.0, double yPadding = 0.0);
		void toLeftOf(std::shared_ptr<Renderable> other, double xPadding = 0.0, double yPadding = 0.0);

		void toLeftTopOf(Renderable* other, double xPadding = 0.0, double yPadding = 0.0);
		void toLeftTopOf(std::shared_ptr<Renderable> other, double xPadding = 0.0, double yPadding = 0.0);

		void toLeftBottomOf(Renderable* other, double xPadding = 0.0, double yPadding = 0.0);
		void toLeftBottomOf(std::shared_ptr<Renderable> other, double xPadding = 0.0, double yPadding = 0.0);

		void toRightOf(Renderable* other, double xPadding = 0.0, double yPadding = 0.0);
		void toRightOf(std::shared_ptr<Renderable> other, double xPadding = 0.0, double yPadding = 0.0);

		void toRightTopOf(Renderable* other, double xPadding = 0.0, double yPadding = 0.0);
		void toRightTopOf(std::shared_ptr<Renderable> other, double xPadding = 0.0, double yPadding = 0.0);

		void toRightBottomOf(Renderable* other, double xPadding = 0.0, double yPadding = 0.0);
		void toRightBottomOf(std::shared_ptr<Renderable> other, double xPadding = 0.0, double yPadding = 0.0);

		void toTopOf(Renderable* other, double xPadding = 0.0, double yPadding = 0.0);
		void toTopOf(std::shared_ptr<Renderable> other, double xPadding = 0.0, double yPadding = 0.0);

		void toTopLeftOf(Renderable* other, double xPadding = 0.0, double yPadding = 0.0);
		void toTopLeftOf(std::shared_ptr<Renderable> other, double xPadding = 0.0, double yPadding = 0.0);

		void toTopRightOf(Renderable* other, double xPadding = 0.0, double yPadding = 0.0);
		void toTopRightOf(std::shared_ptr<Renderable> other, double xPadding = 0.0, double yPadding = 0.0);

		void toBottomOf(Renderable* other, double xPadding = 0.0, double yPadding = 0.0);
		void toBottomOf(std::shared_ptr<Renderable> other, double xPadding = 0.0, double yPadding = 0.0);

		void toBottomLeftOf(Renderable* other, double xPadding = 0.0, double yPadding = 0.0);
		void toBottomLeftOf(std::shared_ptr<Renderable> other, double xPadding = 0.0, double yPadding = 0.0);

		void toBottomRightOf(Renderable* other, double xPadding = 0.0, double yPadding = 0.0);
		void toBottomRightOf(std::shared_ptr<Renderable> other, double xPadding = 0.0, double yPadding = 0.0);

		void setSize(Renderable* other, double wOffset = 0.0, double hOffset = 0.0);
		void setSize(std::shared_ptr<Renderable> other, double wOffset = 0.0, double hOffset = 0.0);

		void setSizeAndCenter(Renderable* other, double xOffset = 0.0, double yOffset = 0.0, double wOffset = 0.0, double hOffset = 0.0);
		void setSizeAndCenter(std::shared_ptr<Renderable> other, double xOffset = 0.0, double yOffset = 0.0, double wOffset = 0.0, double hOffset = 0.0);

		void leftAlign(Renderable* other, double xOffset = 0.0, double yOffset = 0.0);
		void leftAlign(std::shared_ptr<Renderable> other, double xOffset = 0.0, double yOffset = 0.0);

		void toInnerLeftIn(Renderable* other, double xOffset = 0.0, double yOffset = 0.0);
		void toInnerLeftIn(std::shared_ptr<Renderable> other, double xOffset = 0.0, double yOffset = 0.0);

		void toInnerRightIn(Renderable* other, double xOffset = 0.0, double yOffset = 0.0);
		void toInnerRightIn(std::shared_ptr<Renderable> other, double xOffset = 0.0, double yOffset = 0.0);

		void toInnerTopIn(Renderable* other, double xOffset = 0.0, double yOffset = 0.0);
		void toInnerTopIn(std::shared_ptr<Renderable> other, double xOffset = 0.0, double yOffset = 0.0);

		void toInnerBottomIn(Renderable* other, double xOffset = 0.0, double yOffset = 0.0);
		void toInnerBottomIn(std::shared_ptr<Renderable> other, double xOffset = 0.0, double yOffset = 0.0);

		void toInnerTopLeftIn(Renderable* other, double xOffset = 0.0, double yOffset = 0.0);
		void toInnerTopLeftIn(std::shared_ptr<Renderable> other, double xOffset = 0.0, double yOffset = 0.0);

		void toInnerTopRightIn(Renderable* other, double xOffset = 0.0, double yOffset = 0.0);
		void toInnerTopRightIn(std::shared_ptr<Renderable> other, double xOffset = 0.0, double yOffset = 0.0);

		void toInnerBottomLeftIn(Renderable* other, double xOffset = 0.0, double yOffset = 0.0);
		void toInnerBottomLeftIn(std::shared_ptr<Renderable> other, double xOffset = 0.0, double yOffset = 0.0);

		void toInnerBottomRightIn(Renderable* other, double xOffset = 0.0, double yOffset = 0.0);
		void toInnerBottomRightIn(std::shared_ptr<Renderable> other, double xOffset = 0.0, double yOffset = 0.0);

		virtual std::shared_ptr<SerializationClient> doSerialize(SerializationHint hint);
		virtual void doManualSerialize(SerializationHint hint, std::shared_ptr<SerializationClient> injectedClient);

		void markDirty();
		void markClean();

		bool isClean();
	};
}
