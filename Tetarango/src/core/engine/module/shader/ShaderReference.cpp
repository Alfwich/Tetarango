#include "ShaderReference.h"

namespace
{
	const auto vTranslateName = "vTranslate";
}

namespace AW
{
	ShaderReference::ShaderReference(std::vector<std::shared_ptr<Shader>> shaders, std::shared_ptr<Shader> loader) : shaders(shaders), loader(loader) {}

	std::vector<GLuint> ShaderReference::getShaderIds()
	{
		auto ids = std::vector<GLuint>();

		int i = 1;
		for (const auto shaderPtr : shaders)
		{
			ids.push_back(shaderPtr->getShaderId(i++));
		}

		return ids;
	}

	GLuint ShaderReference::getLoaderId()
	{
		if (loaderId == 0 && loader != nullptr)
		{
			return loader->getShaderId((unsigned int)shaders.size());
		}

		return loaderId;
	}

	void ShaderReference::setFloatIUParam(std::string name, double v)
	{
		if (!paramsDisabled)
		{
			const auto fVal = (GLfloat)v;
			if (floatIUParams.count(name) == 1 && floatIUParams.at(name) == fVal)
			{
				return;
			}

			hasSetParams = true;
			cachedParamsValid = false;
			floatIUParams[name] = fVal;
		}
	}

	void ShaderReference::setFloatV2IUParam(std::string name, double v1, double v2)
	{
		if (!paramsDisabled)
		{
			const auto fVal1 = (GLfloat)v1;
			const auto fVal2 = (GLfloat)v2;
			if (floatV2IUParams.count(name) == 1)
			{
				const auto t = floatV2IUParams.at(name);
				if (std::get<0>(t) == fVal1 && std::get<1>(t) == fVal2)
				{
					return;
				}
			}

			hasSetParams = true;
			cachedParamsValid = false;
			floatV2IUParams[name] = std::make_tuple(fVal1, fVal2);
		}
	}

	void ShaderReference::setFloatV3IUParam(std::string name, double v1, double v2, double v3)
	{
		if (!paramsDisabled)
		{
			const auto fVal1 = (GLfloat)v1;
			const auto fVal2 = (GLfloat)v2;
			const auto fVal3 = (GLfloat)v3;
			if (floatV3IUParams.count(name) == 1)
			{
				const auto t = floatV3IUParams.at(name);
				if (std::get<0>(t) == fVal1 && std::get<1>(t) == fVal2 && std::get<2>(t) == fVal3)
				{
					return;
				}
			}

			hasSetParams = true;
			cachedParamsValid = false;
			floatV3IUParams[name] = std::make_tuple(fVal1, fVal2, fVal3);
		}
	}

	void ShaderReference::setFloatV4IUParam(std::string name, double v1, double v2, double v3, double v4)
	{
		if (!paramsDisabled)
		{
			const auto fVal1 = (GLfloat)v1;
			const auto fVal2 = (GLfloat)v2;
			const auto fVal3 = (GLfloat)v3;
			const auto fVal4 = (GLfloat)v4;
			if (floatV4IUParams.count(name) == 1)
			{
				const auto t = floatV4IUParams.at(name);
				if (std::get<0>(t) == fVal1 && std::get<1>(t) == fVal2 && std::get<2>(t) == fVal3 && std::get<3>(t) == fVal4)
				{
					return;
				}
			}

			hasSetParams = true;
			cachedParamsValid = false;
			floatV4IUParams[name] = std::make_tuple(fVal1, fVal2, fVal3, fVal4);
		}
	}

	void ShaderReference::setCachedParam(GLuint modLocation, GLfloat val)
	{
		cachedParamsValid = true;
		cachedFloatIUParams[modLocation] = val;
	}

	void ShaderReference::setCachedParam(GLuint modLocation, std::tuple<GLfloat, GLfloat> values)
	{
		cachedParamsValid = true;
		cachedFloatV2IUParams[modLocation] = values;
	}

	void ShaderReference::setCachedParam(GLuint modLocation, std::tuple<GLfloat, GLfloat, GLfloat> values)
	{
		cachedParamsValid = true;
		cachedFloatV3IUParams[modLocation] = values;
	}

	void ShaderReference::setCachedParam(GLuint modLocation, std::tuple<GLfloat, GLfloat, GLfloat, GLfloat> values)
	{
		cachedParamsValid = true;
		cachedFloatV4IUParams[modLocation] = values;
	}

	double ShaderReference::getFloatIUParam(std::string name, double v)
	{
		auto result = 0.0;

		if (floatIUParams.count(name) == 1)
		{
			return floatIUParams.at(name);
		}

		return result;
	}

	std::tuple<double, double> ShaderReference::getFloatV2IUParam(std::string name)
	{
		auto result = std::tuple<double, double>();

		if (floatV2IUParams.count(name) == 1)
		{
			return floatV2IUParams.at(name);
		}

		return result;
	}

	std::tuple<double, double, double> ShaderReference::getFloatV3IUParam(std::string name)
	{
		auto result = std::tuple<double, double, double>();

		if (floatV3IUParams.count(name) == 1)
		{
			return floatV3IUParams.at(name);
		}

		return result;
	}

	std::tuple<double, double, double, double> ShaderReference::getFloatV4IUParam(std::string name)
	{
		auto result = std::tuple<double, double, double, double>();

		if (floatV4IUParams.count(name) == 1)
		{
			return floatV4IUParams.at(name);
		}

		return result;
	}

	void ShaderReference::setUniforms(GLuint programId, const RendererInfoBundle& info)
	{
		if (paramsDisabled || !hasSetParams)
		{
			return;
		}

		if (!cachedParamsValid)
		{
			updateUniformCaches(programId);
		}

		setCachedUniforms(info);

		const auto position = glGetUniformLocation(programId, "frameTime");
		if (position != -1)
		{
			glUniform1f(position, (GLfloat)info.currentFrameTimestamp);
		}
	}

	void ShaderReference::setCachedUniforms(const RendererInfoBundle& info)
	{
		for (const auto cachedParam : cachedFloatIUParams)
		{
			if (cachedParam.first != -1)
			{
				glUniform1f(cachedParam.first, cachedParam.second);
			}
		}

		for (const auto cachedParam : cachedFloatV2IUParams)
		{
			if (cachedParam.first != -1)
			{
				if (cachedParam.first == vTranslateCachedLocation)
				{
					setVTranslate(cachedParam.first, cachedParam.second, info);
				}
				else
				{
					glUniform2f(cachedParam.first, std::get<0>(cachedParam.second), std::get<1>(cachedParam.second));
				}
			}
		}

		for (const auto cachedParam : cachedFloatV3IUParams)
		{
			if (cachedParam.first != -1)
			{
				glUniform3f(cachedParam.first, std::get<0>(cachedParam.second), std::get<1>(cachedParam.second), std::get<2>(cachedParam.second));
			}
		}

		for (const auto cachedParam : cachedFloatV4IUParams)
		{
			if (cachedParam.first != -1)
			{
				glUniform4f(cachedParam.first, std::get<0>(cachedParam.second), std::get<1>(cachedParam.second), std::get<2>(cachedParam.second), std::get<3>(cachedParam.second));
			}
		}
	}

	void ShaderReference::updateUniformCaches(GLuint programId)
	{
		for (const auto paramNameToValue : floatIUParams)
		{
			const auto position = glGetUniformLocation(programId, paramNameToValue.first.c_str());
			setCachedParam(position, paramNameToValue.second);
		}

		vTranslateCachedLocation = 0;
		for (const auto paramNameToValue : floatV2IUParams)
		{
			const auto position = glGetUniformLocation(programId, paramNameToValue.first.c_str());
			setCachedParam(position, paramNameToValue.second);
			if (paramNameToValue.first == vTranslateName)
			{
				vTranslateCachedLocation = position;
			}
		}

		for (const auto paramNameToValue : floatV3IUParams)
		{
			const auto position = glGetUniformLocation(programId, paramNameToValue.first.c_str());
			setCachedParam(position, paramNameToValue.second);
		}

		for (const auto paramNameToValue : floatV4IUParams)
		{
			const auto position = glGetUniformLocation(programId, paramNameToValue.first.c_str());
			setCachedParam(position, paramNameToValue.second);
		}
	}

	void ShaderReference::setVTranslate(GLuint location, const std::tuple<GLfloat, GLfloat>& value, const RendererInfoBundle& info)
	{
		const double xTranslate = (std::get<0>(value) / (double)info.screenWidth) * 2.0;
		const double yTranslate = -(std::get<1>(value) / (double)info.screenHeight) * 2.0;
		glUniform2f(location, (GLfloat)xTranslate, (GLfloat)yTranslate);
	}

	void ShaderReference::setCachedProgramId(GLuint programId)
	{
		cachedProgramId = programId;
	}

	GLuint ShaderReference::getCachedProgramId()
	{
		return cachedProgramId;
	}

	void ShaderReference::lock()
	{
		paramsDisabled = true;
	}

	void ShaderReference::resetCache()
	{
		loaderId = 0;
		cachedProgramId = 0;

		cachedFloatIUParams.clear();
		cachedFloatV2IUParams.clear();
		cachedFloatV3IUParams.clear();
		cachedFloatV4IUParams.clear();
	}
}