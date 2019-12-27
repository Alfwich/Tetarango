#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include "GL/glew.h"
#include "engine/module/asset/ResourceBundle.h"
#include "Shader.h"

namespace AW
{
	class ShaderUniformInfoBundle
	{
	public:
		double currentFrameTimestamp, screenWidth, screenHeight;
	};

	class ShaderReference
	{
		GLuint loaderId = 0, cachedProgramId = 0, vTranslateCachedLocation = 0;
		bool hasSetParams = false, paramsDisabled = false, cachedParamsValid = false;

		const std::vector<std::shared_ptr<Shader>> shaders;
		const std::shared_ptr<Shader> loader;

		std::unordered_map<std::string, GLfloat> floatIUParams;
		std::unordered_map<GLuint, GLfloat> cachedFloatIUParams;
		void setCachedParam(GLuint modLocation, GLfloat v1);

		std::unordered_map<std::string, std::tuple<GLfloat, GLfloat>> floatV2IUParams;
		std::unordered_map<GLuint, std::tuple<GLfloat, GLfloat>> cachedFloatV2IUParams;
		void setCachedParam(GLuint modLocation, std::tuple<GLfloat, GLfloat> values);

		std::unordered_map<std::string, std::tuple<GLfloat, GLfloat, GLfloat>> floatV3IUParams;
		std::unordered_map<GLuint, std::tuple<GLfloat, GLfloat, GLfloat>> cachedFloatV3IUParams;
		void setCachedParam(GLuint modLocation, std::tuple<GLfloat, GLfloat, GLfloat> values);

		std::unordered_map<std::string, std::tuple<GLfloat, GLfloat, GLfloat, GLfloat>> floatV4IUParams;
		std::unordered_map<GLuint, std::tuple<GLfloat, GLfloat, GLfloat, GLfloat>> cachedFloatV4IUParams;
		void setCachedParam(GLuint modLocation, std::tuple<GLfloat, GLfloat, GLfloat, GLfloat> values);

		void setCachedUniforms(const ShaderUniformInfoBundle& info);
		void updateUniformCaches(GLuint programId);

		void setVTranslate(GLuint location, const std::tuple<GLfloat, GLfloat>& value, const ShaderUniformInfoBundle& info);

	public:
		ShaderReference(std::vector<std::shared_ptr<Shader>> shaders, std::shared_ptr<Shader> loader);

		std::vector<GLuint> getShaderIds();
		GLuint getLoaderId();

		void setFloatIUParam(std::string name, double v);
		void setFloatV2IUParam(std::string name, double v1, double v2);
		void setFloatV3IUParam(std::string name, double v1, double v2, double v3);
		void setFloatV4IUParam(std::string name, double v1, double v2, double v3, double v4);

		double getFloatIUParam(std::string name);
		std::tuple<double, double> getFloatV2IUParam(std::string name);
		std::tuple<double, double, double> getFloatV3IUParam(std::string name);
		std::tuple<double, double, double, double> getFloatV4IUParam(std::string name);

		void setUniforms(GLuint programId, const ShaderUniformInfoBundle& info);

		void setCachedProgramId(GLuint programId);
		GLuint getCachedProgramId();

		void lock();
		void resetCache();
	};
}

