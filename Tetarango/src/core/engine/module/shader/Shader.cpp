#include "Shader.h"

#include <vector>
#include "engine/module/logger/Logger.h"

namespace AW
{
	bool Shader::compileShader()
	{
		if (!shaderIsCompiled())
		{
			GLint compileResult = GL_FALSE;
			int infoLength;

			shaderId = glCreateShader(shaderType);
			const auto source = data->data.get();
			const GLint dataLength = data->size;
			glShaderSource(shaderId, 1, &source, &dataLength);
			glCompileShader(shaderId);

			glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compileResult);
			glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLength);

			if (compileResult == GL_FALSE && infoLength > 0) {
				std::vector<char> infoText(infoLength + 1);
				glGetShaderInfoLog(shaderId, infoLength, NULL, &infoText[0]);
				Logger::instance()->logCritical("Renderer::OpenGL::Failed to compile vertex shader: " + std::string(infoText.begin(), infoText.end()));
				return false;
			}
		}

		return true;
	}

	void Shader::releaseShader()
	{
		if (shaderId != 0)
		{
			glDeleteShader(shaderId);
			shaderId = 0;
		}
	}

	GLuint Shader::getShaderId()
	{
		return shaderId;
	}

	bool Shader::shaderIsCompiled()
	{
		return shaderId != 0;
	}
}