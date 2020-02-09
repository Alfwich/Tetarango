#pragma once

#include "engine/module/serialization/ISerializable.h"
#include "engine/module/shader/ShaderReference.h"
#include "engine/module/display/RenderPackage.h"
#include "engine/module/lua/ILuaObject.h"

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
		AbsoluteSelfOnly,
		NoPositionSelfOnly
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

	enum class LayoutSpace
	{
		Screen,
		World
	};

	class Renderable : public ISerializableDataSubscriber, public ILuaObject
	{
		double rot = 0.0, alpha = 1.0, scale = 1.0;
		bool hasClipRect = false, dirty = true;

	protected:
		Rect rect, clipRect, worldRect, screenRect;
		std::shared_ptr<Color> colorModulation = nullptr;

		std::shared_ptr<ShaderReference> vertexShader = nullptr, fragmentShader = nullptr, clipRectVertexShader = nullptr, clipRectFragmentShader = nullptr;

	public:
		static const float scalingFactor;

		static float screenToWorldPosition(float screen);
		static float worldToScreenPosition(float world);

		static float screenToWorldRotation(float degrees);
		static float worldToScreenRotation(float radians);

		static AWVec2<float> worldToScreen(const AWVec2<float>& world);
		static AWVec2<float> screenToWorld(const AWVec2<float>& screen);

		static AWVec2<float> worldToScreen(const AWVec2<double>& world);
		static AWVec2<float> screenToWorld(const AWVec2<double>& screen);

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

		LayoutSpace layoutSpace = LayoutSpace::Screen;

		void markDirty();
		void markClean();
		bool isClean();

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
		virtual const Rect* getWorldRect() const;
		virtual const Rect* getScreenRect() const;

		virtual Rect* getCollisionRect();

		bool getHasClipRect();
		const Rect* getClipRect();
		void setClipRect(const Rect* rect);
		void setClipRect(const Rect& rect);

		virtual float getWorldX() const;
		virtual float getWorldY() const;
		virtual float getWorldWidth() const;
		virtual float getWorldHeight() const;

		virtual void setWorldX(float newX);
		virtual void setWorldY(float newY);
		virtual void setWorldWidth(float newWidth);
		virtual void setWorldHeight(float newHeight);

		virtual double getScreenX() const;
		virtual double getScreenY() const;
		virtual double getScreenWidth() const;
		virtual double getScreenHeight() const;

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
		void setScreenPositionSize(double x, double y, double width, double height);
		void setScreenPositionSize(const Rect& rect);
		void rotateScreen(double degrees);

		void setWorldPosition(float x, float y);
		void setWorldSize(float width, float height);
		void setWorldPositionAndSize(float x, float y, float width, float height);
		void rotateWorld(float radians);

		double getScreenLeft() const;
		double getScreenRight() const;
		double getScreenTop() const;
		double getScreenBottom() const;
		double getScreenHalfWidth() const;
		double getScreenHalfHeight() const;

		float getWorldLeft() const;
		float getWorldRight() const;
		float getWorldTop() const;
		float getWorldBottom() const;
		float getWorldHalfWidth() const;
		float getWorldHalfHeight() const;

		void topLeftAlignSelf(float xOffset = 0.0, float yOffset = 0.0);
		void centerWithin(const Renderable* other, float xOffset = 0.0, float yOffset = 0.0);
		void centerAlignWithin(const Renderable* other, float xOffset = 0.0, float yOffset = 0.0);
		void matchPosition(const Renderable* other, float xOffset = 0.0, float yOffset = 0.0);
		void toLeftOf(const Renderable* other, float xPadding = 0.0, float yPadding = 0.0);
		void toLeftTopOf(const Renderable* other, float xPadding = 0.0, float yPadding = 0.0);
		void toLeftBottomOf(const Renderable* other, float xPadding = 0.0, float yPadding = 0.0);
		void toRightOf(const Renderable* other, float xPadding = 0.0, float yPadding = 0.0);
		void toRightTopOf(const Renderable* other, float xPadding = 0.0, float yPadding = 0.0);
		void toRightBottomOf(const Renderable* other, float xPadding = 0.0, float yPadding = 0.0);
		void toTopOf(const Renderable* other, float xPadding = 0.0, float yPadding = 0.0);
		void toTopLeftOf(const Renderable* other, float xPadding = 0.0, float yPadding = 0.0);
		void toTopRightOf(const Renderable* other, float xPadding = 0.0, float yPadding = 0.0);
		void toBottomOf(const Renderable* other, float xPadding = 0.0, float yPadding = 0.0);
		void toBottomLeftOf(const Renderable* other, float xPadding = 0.0, float yPadding = 0.0);
		void toBottomRightOf(const Renderable* other, float xPadding = 0.0, float yPadding = 0.0);
		void setSize(const Renderable* other, float wOffset = 0.0, float hOffset = 0.0);
		void setSizeAndCenter(const Renderable* other, float xOffset = 0.0, float yOffset = 0.0, float wOffset = 0.0, float hOffset = 0.0);
		void leftAlign(const Renderable* other, float xOffset = 0.0, float yOffset = 0.0);
		void toInnerLeftIn(const Renderable* other, float xOffset = 0.0, float yOffset = 0.0);
		void toInnerRightIn(const Renderable* other, float xOffset = 0.0, float yOffset = 0.0);
		void toInnerTopIn(const Renderable* other, float xOffset = 0.0, float yOffset = 0.0);
		void toInnerBottomIn(const Renderable* other, float xOffset = 0.0, float yOffset = 0.0);
		void toInnerTopLeftIn(const Renderable* other, float xOffset = 0.0, float yOffset = 0.0);
		void toInnerTopRightIn(const Renderable* other, float xOffset = 0.0, float yOffset = 0.0);
		void toInnerBottomLeftIn(const Renderable* other, float xOffset = 0.0, float yOffset = 0.0);
		void toInnerBottomRightIn(const Renderable* other, float xOffset = 0.0, float yOffset = 0.0);

		void setScreenPosition(const std::shared_ptr<Renderable>& other);
		void centerAlignWithin(const std::shared_ptr<Renderable>& other, float xOffset = 0.0, float yOffset = 0.0);
		void centerWithin(const std::shared_ptr<Renderable>& other, float xOffset = 0.0, float yOffset = 0.0);
		void matchPosition(const std::shared_ptr<Renderable>& other, float xOffset = 0.0, float yOffset = 0.0);
		void toLeftOf(const std::shared_ptr<Renderable>& other, float xPadding = 0.0, float yPadding = 0.0);
		void toLeftTopOf(const std::shared_ptr<Renderable>& other, float xPadding = 0.0, float yPadding = 0.0);
		void toLeftBottomOf(const std::shared_ptr<Renderable>& other, float xPadding = 0.0, float yPadding = 0.0);
		void toRightOf(const std::shared_ptr<Renderable>& other, float xPadding = 0.0, float yPadding = 0.0);
		void toRightTopOf(const std::shared_ptr<Renderable>& other, float xPadding = 0.0, float yPadding = 0.0);
		void toRightBottomOf(const std::shared_ptr<Renderable>& other, float xPadding = 0.0, float yPadding = 0.0);
		void toTopOf(const std::shared_ptr<Renderable>& other, float xPadding = 0.0, float yPadding = 0.0);
		void toTopLeftOf(const std::shared_ptr<Renderable>& other, float xPadding = 0.0, float yPadding = 0.0);
		void toTopRightOf(const std::shared_ptr<Renderable>& other, float xPadding = 0.0, float yPadding = 0.0);
		void toBottomOf(const std::shared_ptr<Renderable>& other, float xPadding = 0.0, float yPadding = 0.0);
		void toBottomLeftOf(const std::shared_ptr<Renderable>& other, float xPadding = 0.0, float yPadding = 0.0);
		void toBottomRightOf(const std::shared_ptr<Renderable>& other, float xPadding = 0.0, float yPadding = 0.0);
		void setSize(const std::shared_ptr<Renderable>& other, float wOffset = 0.0, float hOffset = 0.0);
		void setSizeAndCenter(const std::shared_ptr<Renderable>& other, float xOffset = 0.0, float yOffset = 0.0, float wOffset = 0.0, float hOffset = 0.0);
		void leftAlign(const std::shared_ptr<Renderable>& other, float xOffset = 0.0, float yOffset = 0.0);
		void toInnerLeftIn(const std::shared_ptr<Renderable>& other, float xOffset = 0.0, float yOffset = 0.0);
		void toInnerRightIn(const std::shared_ptr<Renderable>& other, float xOffset = 0.0, float yOffset = 0.0);
		void toInnerTopIn(const std::shared_ptr<Renderable>& other, float xOffset = 0.0, float yOffset = 0.0);
		void toInnerBottomIn(const std::shared_ptr<Renderable>& other, float xOffset = 0.0, float yOffset = 0.0);
		void toInnerTopLeftIn(const std::shared_ptr<Renderable>& other, float xOffset = 0.0, float yOffset = 0.0);
		void toInnerTopRightIn(const std::shared_ptr<Renderable>& other, float xOffset = 0.0, float yOffset = 0.0);
		void toInnerBottomLeftIn(const std::shared_ptr<Renderable>& other, float xOffset = 0.0, float yOffset = 0.0);
		void toInnerBottomRightIn(const std::shared_ptr<Renderable>& other, float xOffset = 0.0, float yOffset = 0.0);

		virtual std::shared_ptr<SerializationClient> doSerialize(SerializationHint hint);
		virtual void doManualSerialize(SerializationHint hint, std::shared_ptr<SerializationClient> injectedClient);

		void onBindLuaHooks(std::shared_ptr<Lua>& lua, const std::shared_ptr<ILuaObject>& obj);

		// Inherited via ILuaObject
		virtual std::string getLuaBindingId() override;
		virtual void onLuaCallback(const std::string& func, LuaBoundObject* obj) override;
};
}
