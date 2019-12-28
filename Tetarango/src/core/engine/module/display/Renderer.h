#pragma once

#include <unordered_map>
#include <algorithm>
#include <stack>
#include <SDL.h>
#include "GL/glew.h"
#include "engine/GameObject.h"
#include "ui/camera/Camera.h"
#include "util/Rect.h"
#include "ui/renderable/element/Element.h"
#include "ui/renderable/primitive/Primitive.h"
#include "RenderPackage.h"
#include "util/linmath.h"
#include "util/QuadMap.h"
#include "util/Color.h"

namespace AW
{
	class Renderer
	{
		ScreenConfig currentScreenConfig;

		SDL_GLContext glContext;
		Color clearColor;

		mat4x4 mvp, p, pAbs, pBackground, m, t, tP;
		GLuint vertexBuffer = 0, textureUVBuffer = 0, vao = 0, currentProgramId = 0, backgroundRenderBuffer = 0;

		std::shared_ptr<ShaderReference> defaultVertexShader, defaultFragmentShader;
		std::unordered_map<std::string, GLuint> programs;
		std::unordered_map<GLuint, std::unordered_map<std::string, GLuint>> programIdToProgramUniformMapId;
		GLuint inMatrixLocation = 0, inColorModLocation = 0, currentFrameBufferId = 0;

		std::stack<std::tuple<double, double, GLuint, GLuint>> frameBufferStack;
		std::stack<Color> colorStack;
		std::stack<RenderPositionMode> renderPositionModeStack;
		std::stack<RenderPositionProcessing> renderProcessingStack;
		std::stack<RenderTextureMode> textureModeStack;
		std::stack<RenderDepthTest> renderDepthStack;
		std::stack<RenderMultiSampleMode> renderMultiSampleModeStack;
		std::stack<RenderTarget> renderTargetStack;
		std::stack<RenderColorMode> renderColorMode;

		unsigned int screenWidth = 0, screenHeight = 0;
		int layerFactor = 1, maxLayers = 60, cullingOffset = 500;
		bool depthEnabled = false, msaaEnabled = false, clearEnabled = true;
		double currentFrameTimestamp = 0.0;

		void harvestFromPreviousRenderer(std::shared_ptr<Renderer> previous);
		void releaseOpenGLObjects();

		void prepareRender(Screen* screen, double renderTimestamp);

		void renderRecursive(std::shared_ptr<Renderable> ao, Rect computed, RenderPackage renderPackage);
		void renderRecursiveDoRender(const std::shared_ptr<Renderable> rend, Rect* computed, RenderPackage* renderPackage);
		void renderRecursivePushStacks(const std::shared_ptr<Renderable>& rend);
		void renderRecursivePopStacks(const std::shared_ptr<Renderable>& rend);
		void renderRecursivePushStencilBuffer(const std::shared_ptr<Renderable>& rend, RenderPackage* renderPackage);
		void renderRecursivePopStencilBuffer(const std::shared_ptr<Renderable>& rend, RenderPackage* renderPackage);
		void renderRecursiveRenderChildren(const std::shared_ptr<Renderable>& rend, const Rect* rect, RenderPackage* renderPackage);

		void renderUpdateRenderableRects(std::shared_ptr<Renderable> rend, Rect* computed, RenderPackage* renderPackage);

		void pushColorStack(const Color* color);
		void setColorModParam(RenderPackage* renderPackage);

		void renderOpenGL(std::shared_ptr<Renderable> obj, Rect rootRect, Screen* screen, RenderPackage* package);

		void renderElement(std::shared_ptr<Renderable> rend, Rect* computed, RenderPackage* renderPackage);
		void renderPrimitive(std::shared_ptr<Renderable> rend, Rect* computed, RenderPackage* renderPackage);
		void renderContainer(std::shared_ptr<Renderable> rend, Rect* computed, RenderPackage* renderPackage);

		void renderElementChildrenIntoElementTexture(std::shared_ptr<Renderable> rend, const RenderPackage* renderPackage);

		void updateClipRectOpenGL(std::shared_ptr<Renderable> rend, Rect* computed, RenderPackage* renderPackage);
		void bindGLTexture(GLuint textureId);

		void renderElementOpenGL(std::shared_ptr<Element> ele, Rect* computed, RenderPackage* renderPackage);
		void renderPrimitiveOpenGL(std::shared_ptr<Primitive> prim, Rect* computed, RenderPackage* renderPackage);
		void renderParticleSystemOpenGL(std::shared_ptr<Primitive> prim, Rect* computed, RenderPackage* renderPackage);

		void openGLDrawArrays(RenderPackage* renderPackage);
		void openGLDrawArraysStencil(RenderPackage* renderPackage);

		void changeProgram(GLuint programId);
		void changeProgram(const std::shared_ptr<Renderable>& renderable, const RenderPackage* renderPackage);
		void changeProgram(const std::shared_ptr<ShaderReference>& vertexShader, const std::shared_ptr<ShaderReference>& fragmentShader);
		GLuint createAndLinkProgramIfNeeded(const std::vector<GLuint> vertexShaderIds, const std::vector<GLuint> fragmentShaderIds, GLuint loaderShaderId);
		std::string getKeyForShaders(const std::vector<GLuint> vertexShaderIds, const std::vector<GLuint> fragmentShaderIds);

		void setViewport(unsigned int width, unsigned int height);

		void applyShaderUniforms(const std::shared_ptr<ShaderReference>& shader, const RenderPackage* renderPackage);
		GLuint getUniformLocationForCurrentProgram(const std::string& paramName, GLuint programId);

		void generateBackgroundRenderBuffer();

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
