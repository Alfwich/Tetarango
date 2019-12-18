#include "ShaderContainer.h"

#include "GL/glew.h"

namespace
{
	std::string vertexShaderPathPrefix = "/vertex/";
}

namespace AW
{
	int ShaderContainer::getShaderTypeFromPath(const std::string & path)
	{
		return AW::StringHelper::contains(path, vertexShaderPathPrefix) ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER;
	}

	void ShaderContainer::bindAsset(std::shared_ptr<Asset> asset)
	{
		this->asset = asset;
	}

	void ShaderContainer::loadShader(std::string path, std::string name)
	{
		if (shaders.count(name) == 1)
		{
			Logger::instance()->logCritical("Shader::Failed to load shader path=" + path + ", with name=" + name + " as it already exists");
			return;
		}

		auto resourceBundle = asset->getAssetBundle(path);
		auto shaderType = getShaderTypeFromPath(path);
		auto shader = std::make_shared<Shader>(path, resourceBundle, shaderType);
		if (shader != nullptr && shader->compileShader())
		{
			shaders[name] = shader;
			Logger::instance()->log("Shader::Loaded shader with name= " + name + ", path=" + path);
		}
		else
		{
			Logger::instance()->logCritical("Shader::Failed to load shader path=" + path + ", with name=" + name + " as it already exists");
		}
	}

	std::shared_ptr<Shader> ShaderContainer::getShader(std::string name)
	{
		if (shaders.count(name) == 0)
		{
			Logger::instance()->logCritical("Shader::Failed to get shader with name=" + name);
			return nullptr;
		}

		return shaders[name];
	}

	void ShaderContainer::rebindAllShaders()
	{
		for (const auto nameToShader : shaders)
		{
			nameToShader.second->releaseShader();
		}

		for (const auto nameToShader : shaders)
		{
			nameToShader.second->compileShader();
		}
	}
}