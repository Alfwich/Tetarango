#include "ShaderContainer.h"

#include "GL/glew.h"
#include <map>

namespace
{
	std::string vertexShaderPathPrefix = "/vertex/";
	std::string loaderShaderPathPrefix = "/fragment/loader/";
}

namespace AW
{
	ShaderType ShaderContainer::getShaderTypeFromPath(const std::string & path)
	{
		return AW::StringHelper::contains(path, vertexShaderPathPrefix) ? ShaderType::Vertex : ShaderType::Fragment;
	}

	void ShaderContainer::bindAsset(std::shared_ptr<Asset> asset)
	{
		this->asset = asset;
	}

	void ShaderContainer::bindTime(std::shared_ptr<Time> time)
	{
		this->time = time;
	}

	void ShaderContainer::loadShaderLoader(std::string path, std::string name)
	{
		if (loaderShaders.count(name) == 1)
		{
			Logger::instance()->logCritical("ShaderContainer::Failed to load shader loader on path=" + path + ", with name=" + name + " as it already exists");
			return;
		}

		if (defaultShaderLoaderName.empty())
		{
			defaultShaderLoaderName = name;
		}

		auto resourceBundle = asset->getAssetBundle(path);
		auto shaderType = ShaderType::Loader;
		auto shader = std::make_shared<Shader>(path, resourceBundle, shaderType);
		if (shader != nullptr && shader->compileShader())
		{
			loaderShaders[name] = shader;
			Logger::instance()->log("ShaderContainer::Loaded shader loader with name= " + name);
		}
		else
		{
			Logger::instance()->logCritical("ShaderContainer::Failed to load shader loader on path=" + path + ", with name=" + name);
		}
	}

	void ShaderContainer::loadShader(std::string path, std::string name)
	{
		if (shaders.count(name) == 1)
		{
			Logger::instance()->logCritical("ShaderContainer::Failed to load shader path=" + path + ", with name=" + name + " as it already exists");
			return;
		}

		auto resourceBundle = asset->getAssetBundle(path);
		auto shaderType = getShaderTypeFromPath(path);
		auto shader = std::make_shared<Shader>(path, resourceBundle, shaderType);
		if (shader != nullptr && shader->compileShader())
		{
			shaders[name] = shader;
			Logger::instance()->log("ShaderContainer::Loaded shader with name= " + name + ", path=" + path);
		}
		else
		{
			Logger::instance()->logCritical("ShaderContainer::Failed to load shader path=" + path + ", with name=" + name + " as it already exists");
		}
	}

	void ShaderContainer::registerShaderComposition(std::vector<std::string> shaderNames, std::string name)
	{
		if (shaderComps.count(name) == 1)
		{
			Logger::instance()->logCritical("ShaderContainer::Failed to load shader composition name=" + name + ", as it already exists");
			return;
		}

		shaderComps[name] = shaderNames;
	}

	std::shared_ptr<ShaderReference> ShaderContainer::getShader(std::vector<std::string> names, bool assignDefaultParams)
	{
		return getShader(names, defaultShaderLoaderName, assignDefaultParams);
	}

	std::shared_ptr<ShaderReference> ShaderContainer::getShader(std::vector<std::string> names, std::string loaderName, bool assignDefaultParams)
	{
		auto hasSeenShader = std::map<std::string, bool>();
		auto shadersToPack = std::vector<std::weak_ptr<Shader>>();

		for (const auto name : names)
		{
			if (shaderComps.count(name) == 1)
			{
				for (const auto compName : shaderComps.at(name))
				{
					if (shaders.count(compName) == 1 && hasSeenShader.count(compName) == 0)
					{
						shadersToPack.push_back(shaders.at(compName));
						hasSeenShader[compName] = true;
					}
				}
			}
			else if (shaders.count(name) == 1 && hasSeenShader.count(name) == 0)
			{
				shadersToPack.push_back(shaders.at(name));
				hasSeenShader[name] = true;
			}
		}

		const auto shaderLoader = loaderShaders[loaderName];
		auto shader = std::make_shared<ShaderReference>(shadersToPack, shaderLoader);

		if (assignDefaultParams)
		{
			shader->setFloatIUParam("frameStartTime", (GLfloat)time->getFrameStartTime());
			shader->setFloatIUParam("frameTime", -1.f);
		}

		shaderReferences.push_back(shader);

		return shader;
	}

	void ShaderContainer::releaseAllShaders()
	{
		Logger::instance()->log("ShaderContainer::Releasing " + std::to_string(shaders.size()) + " shaders");
		for (const auto nameToShader : shaders)
		{
			nameToShader.second->releaseShader();
		}

		for (const auto nameToShaderLoader : loaderShaders)
		{
			nameToShaderLoader.second->releaseShader();
		}

		for (auto it = shaderReferences.begin(); it != shaderReferences.end();)
		{
			const auto shaderReferencePtr = (*it).lock();

			if (shaderReferencePtr != nullptr)
			{
				shaderReferencePtr->resetCache();
				++it;
			}
			else
			{
				it = shaderReferences.erase(it);
			}
		}
	}

	void ShaderContainer::rebindAllShaders()
	{
		Logger::instance()->log("ShaderContainer::Rebinding " + std::to_string(shaders.size()) + " shaders");
		for (const auto nameToShader : shaders)
		{
			nameToShader.second->compileShader();
		}
	}

	void ShaderContainer::purgeWeakRefs()
	{
		for (auto it = shaderReferences.begin(); it != shaderReferences.end();)
		{
			const auto shaderReferencePtr = (*it).lock();
			if (shaderReferencePtr != nullptr)
			{
				++it;
			}
			else
			{
				it = shaderReferences.erase(it);
			}
		}

	}
}