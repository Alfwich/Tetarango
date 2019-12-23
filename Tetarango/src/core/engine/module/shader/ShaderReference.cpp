#include "ShaderReference.h"

namespace AW
{
	ShaderReference::ShaderReference(std::vector<std::weak_ptr<Shader>> shaders, std::weak_ptr<Shader> loader) : shaders(shaders), loader(loader) {}

	std::vector<GLuint> ShaderReference::getShaderIds()
	{
		auto ids = std::vector<GLuint>();
		int i = 1;
		for (const auto shaderWeakPtr : shaders)
		{
			const auto ptr = shaderWeakPtr.lock();
			if (ptr != nullptr)
			{
				ids.push_back(ptr->getShaderId(i++));
			}
		}

		return ids;
	}

	GLuint ShaderReference::getLoaderId()
	{
		if (loaderId == 0)
		{
			const auto shaderPtr = loader.lock();
			if (shaderPtr != nullptr)
			{
				return shaderPtr->getShaderId((unsigned int)shaders.size());
			}
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

	bool ShaderReference::hasFrameTimeParam()
	{
		return frameTimeParamEnabled;
	}

	void ShaderReference::setFloatIUParam(std::string name, GLfloat val)
	{
		if (!paramsDisabled)
		{
			hasSetParams = true;
			cachedParamValues = false;
			frameTimeParamEnabled = frameTimeParamEnabled || name == "frameTime";
			floatIUParams[name] = val;
		}
	}

	const std::map<std::string, GLfloat>& ShaderReference::getFloatIUParams()
	{
		return floatIUParams;
	}

	void ShaderReference::setCachedParam(GLuint modLocation, GLfloat val)
	{
		cachedParamValues = true;
		cachedFloatIUParams[modLocation] = val;
	}

	const std::map<GLuint, GLfloat>& ShaderReference::getCachedFloatIUParams()
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