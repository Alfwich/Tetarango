#include "ShaderReference.h"

namespace AW
{
	ShaderReference::ShaderReference(std::weak_ptr<Shader> shader)
	{
		this->shader = shader;
	}

	GLuint ShaderReference::getShaderId()
	{
		const auto shaderPtr = shader.lock();
		return shaderPtr != nullptr ? shaderPtr->getShaderId() : 0;
	}

	bool ShaderReference::shaderIsCompiled()
	{
		const auto shaderPtr = shader.lock();
		return shaderPtr != nullptr ? shaderPtr->shaderIsCompiled() : false;
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
}