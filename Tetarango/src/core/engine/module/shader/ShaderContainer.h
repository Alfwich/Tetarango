#pragma once

#include "Shader.h"
#include "engine/module/asset/Asset.h"
#include "engine/module/IBaseModule.h"
#include "ShaderReference.h"

namespace AW
{

	class ShaderContainer : public IBaseModule
	{
		std::shared_ptr<Asset> asset;
		std::shared_ptr<Time> time;

		std::unordered_map<std::string, std::shared_ptr<Shader>> shaders;
		std::string defaultShaderLoaderName;
		std::unordered_map<std::string, std::shared_ptr<Shader>> loaderShaders;

		std::unordered_map<std::string, std::vector<std::string>> shaderComps;

		ShaderType getShaderTypeFromPath(const std::string& path);

	public:
		void bindAsset(std::shared_ptr<Asset> asset);
		void bindTime(std::shared_ptr<Time> time);

		void loadShaderLoader(std::string path, std::string name);
		void loadShader(std::string path, std::string name);

		void registerShaderComposition(std::vector<std::string> shaderNames, std::string name);

		std::shared_ptr<ShaderReference> getShader(std::vector<std::string> names, bool assignDefaultParams = false);
		std::shared_ptr<ShaderReference> getShader(std::vector<std::string> names, std::string loaderName, bool assignDefaultParams = false);

		std::shared_ptr<ShaderReference> getLockedShader(std::vector<std::string> names, bool assignDefaultParams = false);

		void updateShaderFrameStartTime(const std::shared_ptr<ShaderReference>& shader);

		void onLoadResources();
	};
}