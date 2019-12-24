#include "ShaderContainer.h"

#include "GL/glew.h"
#include <map>

#include "util/NumberHelper.h"

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
		auto shadersToPack = std::vector<std::shared_ptr<Shader>>();

		for (const auto name : names)
		{
			if (shaderComps.count(name) == 1)
			{
				for (const auto compName : shaderComps.at(name))
				{
					if (shaders.count(compName) == 1)
					{
						shadersToPack.push_back(shaders.at(compName));
					}
					else
					{
						Logger::instance()->logCritical("ShaderContainer::Failed to get shader with name=" + compName + ", from comp, as it does not exist");
					}
				}
			}
			else if (shaders.count(name) == 1)
			{
				shadersToPack.push_back(shaders.at(name));
			}
			else
			{
				Logger::instance()->logCritical("ShaderContainer::Failed to get shader with name=" + name + " as it does not exist");
			}
		}

		if (shadersToPack.empty())
		{
			Logger::instance()->logCritical("ShaderContainer::Attempted to create ShaderReference with no resolved Shader units");
			return nullptr;
		}

		const auto shaderLoader = loaderShaders[loaderName];
		auto shader = std::make_shared<ShaderReference>(shadersToPack, shaderLoader);

		if (assignDefaultParams)
		{
			shader->setFloatIUParam("frameStartTime", (GLfloat)time->getFrameStartTime());
			shader->setFloatIUParam("random", (GLfloat)AW::NumberHelper::random());
		}

		shaderReferences.push_back(shader);

		return shader;
	}

	std::shared_ptr<ShaderReference> ShaderContainer::getLockedShader(std::vector<std::string> names, bool assignDefaultParams)
	{
		const auto result = getShader(names, assignDefaultParams);
		result->lock();
		return result;
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

	void ShaderContainer::onLoadResources()
	{
		loadShaderLoader("res/shader/fragment/loader/loader-default.glsl", "default");

		loadShader("res/shader/vertex/default.glsl", "v-default");
		loadShader("res/shader/fragment/color.glsl", "f-default");

		loadShader("res/shader/fragment/texture.glsl", "f-texture");
		loadShader("res/shader/fragment/color.glsl", "f-color");
		loadShader("res/shader/fragment/solid.glsl", "f-cliprect");
		loadShader("res/shader/fragment/blur.glsl", "f-blur");
		loadShader("res/shader/fragment/negate.glsl", "f-negate");
		loadShader("res/shader/fragment/alpha.glsl", "f-alpha");
		loadShader("res/shader/fragment/pulsate.glsl", "f-pulsate");
		loadShader("res/shader/fragment/clip-texture.glsl", "f-clip-texture");
		loadShader("res/shader/fragment/9slice.glsl", "f-9slice");
		loadShader("res/shader/fragment/step-color.glsl", "f-step-color");
		loadShader("res/shader/fragment/step-texture.glsl", "f-step-texture");
		loadShader("res/shader/fragment/scanline.glsl", "f-scanline");
		loadShader("res/shader/fragment/scanline-retro.glsl", "f-scanline-retro");
	}
}