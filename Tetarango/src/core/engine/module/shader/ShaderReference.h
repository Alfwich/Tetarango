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
		unsigned int loaderId = 0, cachedProgramId = 0, vTranslateCachedLocation = 0;
		bool hasSetParams = false, paramsDisabled = false, cachedParamsValid = false;

		const std::vector<std::shared_ptr<Shader>> shaders;
		const std::shared_ptr<Shader> loader;

		std::unordered_map<std::string, float> floatIUParams;
		std::unordered_map<unsigned int, float> cachedFloatIUParams;
		void setCachedParam(unsigned int modLocation, float v1);

		std::unordered_map<std::string, std::tuple<float, float>> floatV2IUParams;
		std::unordered_map<unsigned int, std::tuple<float, float>> cachedFloatV2IUParams;
		void setCachedParam(unsigned int modLocation, std::tuple<float, float> values);

		std::unordered_map<std::string, std::tuple<float, float, float>> floatV3IUParams;
		std::unordered_map<unsigned int, std::tuple<float, float, float>> cachedFloatV3IUParams;
		void setCachedParam(unsigned int modLocation, std::tuple<float, float, float> values);

		std::unordered_map<std::string, std::tuple<float, float, float, float>> floatV4IUParams;
		std::unordered_map<unsigned int, std::tuple<float, float, float, float>> cachedFloatV4IUParams;
		void setCachedParam(unsigned int modLocation, std::tuple<float, float, float, float> values);

		void setCachedUniforms(const ShaderUniformInfoBundle& info);
		void updateUniformCaches(unsigned int programId);

		void setVTranslate(unsigned int location, const std::tuple<float, float>& value, const ShaderUniformInfoBundle& info);

	public:
		ShaderReference(std::vector<std::shared_ptr<Shader>> shaders, std::shared_ptr<Shader> loader);

		std::vector<unsigned int> getShaderIds();
		unsigned int getLoaderId();

		void setFloatIUParam(std::string name, double v);
		void setFloatV2IUParam(std::string name, double v1, double v2);
		void setFloatV3IUParam(std::string name, double v1, double v2, double v3);
		void setFloatV4IUParam(std::string name, double v1, double v2, double v3, double v4);

		double getFloatIUParam(std::string name);
		std::tuple<double, double> getFloatV2IUParam(std::string name);
		std::tuple<double, double, double> getFloatV3IUParam(std::string name);
		std::tuple<double, double, double, double> getFloatV4IUParam(std::string name);

		void setUniforms(unsigned int programId, const ShaderUniformInfoBundle& info);

		void setCachedProgramId(unsigned int programId);
		unsigned int getCachedProgramId();

		void lock();
		void resetCache();
	};
}

