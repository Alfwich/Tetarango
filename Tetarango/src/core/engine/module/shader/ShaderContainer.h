#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include "Shader.h"
#include "engine/module/logger/Logger.h"
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

		int getShaderTypeFromPath(const std::string& path);

	public:
		void bindAsset(std::shared_ptr<Asset> asset);
		void bindTime(std::shared_ptr<Time> time);

		void loadShader(std::string path, std::string name);
		std::shared_ptr<ShaderReference> getShader(std::string name, bool assignDefaultParams = false);

		void releaseAllShaders();
		void rebindAllShaders();
	};

}