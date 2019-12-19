#pragma once

#include <string>
#include <map>
#include "GL/glew.h"
#include "engine/module/asset/ResourceBundle.h"
#include "Shader.h"

namespace AW
{
	class ShaderReference
	{
		std::weak_ptr<Shader> shader;
		std::map<std::string, GLfloat> floatIUParams;
		bool hasSetParams = false;
	public:
		ShaderReference(std::weak_ptr<Shader> shader);

		GLuint getShaderId();
		bool shaderIsCompiled();

		bool hasCustomParams();
		void setFloatIUParam(std::string name, GLfloat val);
		const std::map<std::string, GLfloat>& getFloatIUParams();
	};
}

