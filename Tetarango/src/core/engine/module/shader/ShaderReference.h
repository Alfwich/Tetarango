#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include "GL/glew.h"
#include "engine/module/asset/ResourceBundle.h"
#include "Shader.h"

namespace AW
{
	class ShaderReference
	{
		const std::vector<std::shared_ptr<Shader>> shaders;
		const std::shared_ptr<Shader> loader;

		GLuint loaderId = 0, cachedProgramId = 0;

		std::unordered_map<std::string, GLfloat> floatIUParams;
		std::unordered_map<GLuint, GLfloat> cachedFloatIUParams;
		bool hasSetParams = false, paramsDisabled = false, cachedParamValues = false;
	public:
		ShaderReference(std::vector<std::shared_ptr<Shader>> shaders, std::shared_ptr<Shader> loader);

		std::vector<GLuint> getShaderIds();
		GLuint getLoaderId();

		bool hasCustomParams();
		bool hasCachedCustomParams();

		void setFloatIUParam(std::string name, double val);
		const std::unordered_map<std::string, GLfloat>& getFloatIUParams();

		void setCachedParam(GLuint modLocation, GLfloat val);
		const std::unordered_map<GLuint, GLfloat>& getCachedFloatIUParams();

		void setCachedProgramId(GLuint programId);
		GLuint getCachedProgramId();

		void lock();
		void resetCache();
	};
}

