#pragma once

#include "GL/glew.h"
#include "engine/GameObject.h"
#include "ui/camera/Camera.h"
#include "ui/renderable/element/Element.h"
#include "ui/renderable/primitive/Primitive.h"
#include "RenderPackage.h"
#include "util/linmath.h"

namespace
{
	static void GLClearError() { while (glGetError() != GL_NO_ERROR); }
	static void GLBreakError() { while (auto error = glGetError()) { __debugbreak(); } }
}

#ifdef _DEBUG
#ifdef DEBUG_CALL
#define GLDbgCall(x) GLClearError(); x; GLBreakError();
#else
#define GLDbgCall(x) x;
#endif
#else
#define GLDbgCall(x) x;
#endif // _DEBUG

namespace AW
{
	class Renderer
	{
		ScreenConfig currentScreenConfig;

		SDL_GLContext glContext;
		Color clearColor;

		LM::mat4x4 mvp, p, pAbs, pBackground, m, t, tP;
		unsigned int defaultVertexBuffer = 0, defaultTextureUVBuffer = 0, vao = 0, currentProgramId = 0, backgroundRenderBuffer = 0, defaultWhiteTexture = 0;

		std::vector<RenderPackage>::iterator nextPackage;
		std::vector<RenderPackage> renderBuffer;

		std::list<RenderPackage>::iterator nextPackageFallbackList;
		std::list<RenderPackage> renderBufferFallbackList;

		std::shared_ptr<ShaderReference> defaultVertexShader, defaultFragmentShader;
		std::unordered_map<std::string, unsigned int> programs;
		std::unordered_map<unsigned int, std::unordered_map<std::string, unsigned int>> programIdToProgramUniformMapId;
		unsigned int inMatrixLocation = 0, inColorModLocation = 0, currentFrameBufferId = 0;

		std::stack<std::tuple<double, double, unsigned int, unsigned int>> frameBufferStack;
		std::stack<Color> colorStack;
		std::stack<RenderPositionMode> renderPositionModeStack;
		std::stack<RenderPositionProcessingMode> renderProcessingStack;
		std::stack<RenderTextureMode> textureModeStack;
		std::stack<RenderDepthTestMode> renderDepthStack;
		std::stack<RenderMultiSampleMode> renderMultiSampleModeStack;
		std::stack<RenderTargetMode> renderTargetStack;
		std::stack<RenderColorMode> renderColorMode;

		unsigned int screenWidth = 0, screenHeight = 0;
		int layerFactor = 1, maxLayers = 60;
		bool depthEnabled = false, msaaEnabled = false, clearEnabled = true, hasFallenBackToRenderList = false;
		double currentFrameTimestamp = 0.0;

		void releaseOpenGLObjects();

		void preRender(Screen* screen, double renderTimestamp);
		void postRender();

		RenderPackage* getNextRenderPackageForObj(std::shared_ptr<Renderable> obj, RenderPackage* previous = nullptr);
		void updateNextBufferPackageIterator();

		void renderOpenGL(std::shared_ptr<Renderable> obj);

		void renderRecursive(RenderPackage* renderPackage);
		void renderRecursiveDoRender(RenderPackage* renderPackage);
		void renderRecursivePushStacks(RenderPackage* renderPackage);
		void renderRecursivePopStacks(RenderPackage* renderPackage);
		void renderRecursivePushStencilBuffer(RenderPackage* renderPackage);
		void renderRecursivePopStencilBuffer(RenderPackage* renderPackage);
		void renderRecursiveRenderChildren(RenderPackage* renderPackage);

		void renderUpdateRenderableRects(RenderPackage* renderPackage);

		void pushColorStack(RenderPackage* renderPackage);
		void setColorModParam(RenderPackage* renderPackage);

		void renderElement(RenderPackage* renderPackage);
		void renderPrimitive(RenderPackage* renderPackage);
		void renderContainer(RenderPackage* renderPackage);

		void renderRenderableChildrenIntoElementTexture(RenderPackage* renderPackage);

		void updateClipRectOpenGL(RenderPackage* renderPackage);
		void bindGLTexture(unsigned int textureId);

		void renderElementOpenGL(RenderPackage* renderPackage);
		void renderPrimitiveOpenGL(RenderPackage* renderPackage);
		void renderPolygonOpenGL(RenderPackage* renderPackage);
		void renderParticleSystemOpenGL(RenderPackage* renderPackage);

		void openGLDrawTriangles(RenderPackage* renderPackage);
		void openGLDrawPoints(RenderPackage* renderPackage, unsigned int vBuffer, unsigned int numPoints, bool isFilled = true);
		void openGLDrawArraysStencil(RenderPackage* renderPackage);

		void changeProgram(unsigned int programId);
		void changeProgram(RenderPackage* renderPackage);
		void changeProgram(const std::shared_ptr<ShaderReference>& vertexShader, const std::shared_ptr<ShaderReference>& fragmentShader);
		unsigned int createAndLinkProgramIfNeeded(const std::vector<unsigned int> vertexShaderIds, const std::vector<unsigned int> fragmentShaderIds, unsigned int loaderShaderId);
		std::string getKeyForShaders(const std::vector<unsigned int> vertexShaderIds, const std::vector<unsigned int> fragmentShaderIds);

		void setViewport(unsigned int width, unsigned int height);

		void applyShaderUniforms(const std::shared_ptr<ShaderReference>& shader, const RenderPackage* renderPackage);
		unsigned int getUniformLocationForCurrentProgram(const std::string& paramName, unsigned int programId);

		void generateBackgroundRenderBuffer();

		unsigned int generateVertexBuffer(const std::vector<AWVec2<float>>& points);

		void setVertexAttributePointer(const unsigned int bufferId, const unsigned int stride, const unsigned int offset);
		void setUVAttributePointer(const unsigned int bufferId, const unsigned int stride, const unsigned int offset);

		bool shouldCull(const RenderPackage* const renderPackage);

	public:
		Renderer(const ScreenConfig& screenConfig, std::shared_ptr<Renderer> oldRenderer);
		virtual ~Renderer();

		bool initOpenGL(SDL_Window* window);
		void setDefaultShaders(std::shared_ptr<ShaderReference> vertexShader, std::shared_ptr<ShaderReference> fragmentShader);
		void updateScreenConfig(const ScreenConfig& config);

		void setClearColor(int r, int g, int b, int a);
		void setClearEnabled(bool flag);
		void render(std::shared_ptr<Renderable> obj, Screen* screen, double frameTimestamp);

		void reportOpenGLErrors();
	};
}
