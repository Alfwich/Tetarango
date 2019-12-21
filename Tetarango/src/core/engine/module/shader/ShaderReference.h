#pragma once

#include <string>
#include <map>
#include <vector>
#include "GL/glew.h"
#include "engine/module/asset/ResourceBundle.h"
#include "Shader.h"

namespace AW
{
	class ShaderReference
	{
		const std::vector<std::weak_ptr<Shader>> shaders;
		const std::weak_ptr<Shader> loader;

		std::vector<GLuint> ids;
		GLuint loaderId = 0;

		std::map<std::string, GLfloat> floatIUParams;
		bool hasSetParams = false;
	public:
		ShaderReference(std::vector<std::weak_ptr<Shader>> shaders, std::weak_ptr<Shader> loader);

		const std::vector<GLuint>& getShaderIds();
		GLuint getLoaderId();

		bool hasCustomParams();
		void setFloatIUParam(std::string name, GLfloat val);
		const std::map<std::string, GLfloat>& getFloatIUParams();

		void resetCache();
	};
}

