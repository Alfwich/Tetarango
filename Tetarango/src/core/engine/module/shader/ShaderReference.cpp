#include "ShaderReference.h"

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

	bool ShaderReference::hasCustomParams()
	{
		return !paramsDisabled && hasSetParams;
	}

	bool ShaderReference::hasCachedCustomParams()
	{
		return cachedParamValues;
	}

	void ShaderReference::setFloatIUParam(std::string name, double val)
	{
		if (!paramsDisabled)
		{
			auto fVal = (GLfloat)val;
			if (floatIUParams.count(name) == 1 && floatIUParams.at(name) == fVal)
			{
				return;
			}

			hasSetParams = true;
			cachedParamValues = false;
			floatIUParams[name] = fVal;
		}
	}

	const std::unordered_map<std::string, GLfloat>& ShaderReference::getFloatIUParams()
	{
		return floatIUParams;
	}

	void ShaderReference::setCachedParam(GLuint modLocation, GLfloat val)
	{
		cachedParamValues = true;
		cachedFloatIUParams[modLocation] = val;
	}

	const std::unordered_map<GLuint, GLfloat>& ShaderReference::getCachedFloatIUParams()
	{
		return cachedFloatIUParams;
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
	}
}