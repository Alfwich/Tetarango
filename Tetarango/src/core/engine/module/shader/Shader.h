#pragma once

#include <string>
#include <map>
#include "GL/glew.h"
#include "engine/module/asset/ResourceBundle.h"

namespace AW
{
	enum class ShaderType
	{
		Fragment,
		Vertex,
		Loader
	};
	class Shader
	{
		const ShaderType shaderType;
		std::shared_ptr<ResourceBundle> data;
		std::map<int, GLuint> locToShaderIds;

		std::string getUpdatedShaderProgramForLoc(char* data, int dataSize, int loc);

		void genShaderForLoc(int locId);

	public:
		Shader(std::string name, std::shared_ptr<ResourceBundle> data, ShaderType shaderType = ShaderType::Fragment) : name(name), data(data), shaderType(shaderType) {};
		~Shader() { releaseShader(); };
		const std::string name;

		GLuint getShaderId(unsigned int location = 1);
		bool shaderIsCompiled();

		bool compileShader();
		void releaseShader();
	};
}
