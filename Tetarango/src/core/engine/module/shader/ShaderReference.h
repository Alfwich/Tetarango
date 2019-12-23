#pragma once

#include <string>
#include <map>
#include <vector>
#include "GL/glew.h"
#include "engine/module/asset/ResourceBundle.h"
#include "Shader.h"

namespace AW
{
	class ShaderReference
	{
		const std::vector<std::weak_ptr<Shader>> shaders;
		const std::weak_ptr<Shader> loader;

		GLuint loaderId = 0, cachedProgramId = 0;

		std::map<std::string, GLfloat> floatIUParams;
		std::map<GLuint, GLfloat> cachedFloatIUParams;
		bool hasSetParams = false, paramsDisabled = false, cachedParamValues = false, frameTimeParamEnabled = false;
	public:
		ShaderReference(std::vector<std::weak_ptr<Shader>> shaders, std::weak_ptr<Shader> loader);

		std::vector<GLuint> getShaderIds();
		GLuint getLoaderId();

		bool hasCustomParams();
		bool hasCachedCustomParams();
		bool hasFrameTimeParam();

		void setFloatIUParam(std::string name, GLfloat val);
		const std::map<std::string, GLfloat>& getFloatIUParams();

		void setCachedParam(GLuint modLocation, GLfloat val);
		const std::map<GLuint, GLfloat>& getCachedFloatIUParams();

		void setCachedProgramId(GLuint programId);
		GLuint getCachedProgramId();

		void lock();
		void resetCache();
	};
}

