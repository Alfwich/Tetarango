#pragma once

#include <string>
#include "GL/glew.h"
#include "engine/module/asset/ResourceBundle.h"

namespace AW
{
	class Shader
	{
		GLuint shaderId = 0;
		const int shaderType = 0;
		std::shared_ptr<ResourceBundle> data;
	public:
		Shader(std::string name, std::shared_ptr<ResourceBundle> data, int shaderType = GL_FRAGMENT_SHADER) : name(name), data(data), shaderType(shaderType) {};
		~Shader() { releaseShader(); };
		const std::string name;

		GLuint getShaderId();
		bool shaderIsCompiled();

		bool compileShader();
		void releaseShader();
	};
}
