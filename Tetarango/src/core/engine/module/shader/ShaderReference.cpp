#include "ShaderReference.h"

namespace AW
{
	ShaderReference::ShaderReference(std::vector<std::weak_ptr<Shader>> shaders, std::weak_ptr<Shader> loader) : shaders(shaders), loader(loader) {};

	const std::vector<GLuint>& ShaderReference::getShaderIds()
	{
		if (ids.empty())
		{
			ids.clear();
			int i = 1;
			for (const auto shaderWeakPtr : shaders)
			{
				const auto ptr = shaderWeakPtr.lock();
				if (ptr != nullptr)
				{
					ids.push_back(ptr->getShaderId(i++));
				}
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
		return hasSetParams;
	}

	void ShaderReference::setFloatIUParam(std::string name, GLfloat val)
	{
		hasSetParams = true;
		floatIUParams[name] = val;
	}

	const std::map<std::string, GLfloat>& ShaderReference::getFloatIUParams()
	{
		return floatIUParams;
	}

	void ShaderReference::resetCache()
	{
		ids.clear();
		loaderId = 0;
	}
}