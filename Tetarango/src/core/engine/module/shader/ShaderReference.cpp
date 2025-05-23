#include "ShaderReference.h"

namespace
{
	const auto vTranslateName = "vTranslate";
}

namespace AW
{
	ShaderReference::ShaderReference(std::vector<std::shared_ptr<Shader>> shaders, std::shared_ptr<Shader> loader) : shaders(shaders), loader(loader) {}

	std::vector<unsigned int> ShaderReference::getShaderIds()
	{
		auto ids = std::vector<unsigned int>();

		int i = 1;
		for (const auto shaderPtr : shaders)
		{
			ids.push_back(shaderPtr->getShaderId(i++));
		}

		return ids;
	}

	unsigned int ShaderReference::getLoaderId()
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
			const auto fVal = (float)v;
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
			const auto fVal1 = (float)v1;
			const auto fVal2 = (float)v2;
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
			const auto fVal1 = (float)v1;
			const auto fVal2 = (float)v2;
			const auto fVal3 = (float)v3;
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
			const auto fVal1 = (float)v1;
			const auto fVal2 = (float)v2;
			const auto fVal3 = (float)v3;
			const auto fVal4 = (float)v4;
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

	void ShaderReference::setCachedParam(unsigned int modLocation, float val)
	{
		cachedParamsValid = true;
		cachedFloatIUParams[modLocation] = val;
	}

	void ShaderReference::setCachedParam(unsigned int modLocation, std::tuple<float, float> values)
	{
		cachedParamsValid = true;
		cachedFloatV2IUParams[modLocation] = values;
	}

	void ShaderReference::setCachedParam(unsigned int modLocation, std::tuple<float, float, float> values)
	{
		cachedParamsValid = true;
		cachedFloatV3IUParams[modLocation] = values;
	}

	void ShaderReference::setCachedParam(unsigned int modLocation, std::tuple<float, float, float, float> values)
	{
		cachedParamsValid = true;
		cachedFloatV4IUParams[modLocation] = values;
	}

	double ShaderReference::getFloatIUParam(std::string name)
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

	void ShaderReference::setUniforms(unsigned int programId, const ShaderUniformInfoBundle& info)
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
			glUniform1f(position, (float)info.currentFrameTimestamp);
		}
	}

	void ShaderReference::setCachedUniforms(const ShaderUniformInfoBundle& info)
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

	void ShaderReference::updateUniformCaches(unsigned int programId)
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

	void ShaderReference::setVTranslate(unsigned int location, const std::tuple<float, float>& value, const ShaderUniformInfoBundle& info)
	{
		const double xTranslate = (std::get<0>(value) / (double)info.screenWidth) * 2.0;
		const double yTranslate = -(std::get<1>(value) / (double)info.screenHeight) * 2.0;
		glUniform2f(location, (float)xTranslate, (float)yTranslate);
	}

	void ShaderReference::setCachedProgramId(unsigned int programId)
	{
		cachedProgramId = programId;
	}

	unsigned int ShaderReference::getCachedProgramId()
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