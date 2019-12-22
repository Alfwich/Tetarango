#include "Shader.h"

#include <vector>
#include "util/StringHelper.h"
#include "util/NumberHelper.h"
#include "engine/module/logger/Logger.h"

namespace
{
	const auto replaceToken = "_main";
	const auto repeatToken = std::string("//#REPEAT");
}

namespace AW
{
	bool Shader::compileShader()
	{
		const auto shader1 = getShaderId(1);

		return shader1 != 0;
	}

	void Shader::genShaderForLoc(int locId)
	{
		GLint compileResult = GL_FALSE;
		int infoLength;

		const auto glShaderTypeForShaderType = shaderType == ShaderType::Vertex ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER;
		const auto shaderId = glCreateShader(glShaderTypeForShaderType);
		auto source = getUpdatedShaderProgramForLoc(data->data.get(), data->size, locId);
		const auto programStartOffset = AW::StringHelper::distanceToLeft(&source, "#");
		const GLint dataLength = (GLint)source.size() - programStartOffset;
		const auto sourceLoc = &source[programStartOffset];
		glShaderSource(shaderId, 1, &sourceLoc, &dataLength);
		glCompileShader(shaderId);

		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compileResult);
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLength);

		if (compileResult == GL_FALSE && infoLength > 0) {
			std::vector<char> infoText(infoLength + 1);
			glGetShaderInfoLog(shaderId, infoLength, NULL, &infoText[0]);
			Logger::instance()->logCritical("Shader::Failed to compile shader: " + std::string(infoText.begin(), infoText.end()));
		}

		locToShaderIds[locId] = shaderId;
	}

	std::string Shader::getUpdatedShaderProgramForLoc(char* data, int dataSize, int loc)
	{
		auto programData = std::string(data, dataSize);

		switch (shaderType)
		{
		case ShaderType::Fragment:
		{
			const auto mainPos = AW::StringHelper::distanceToLeft(&programData, replaceToken);
			if (mainPos < programData.size())
			{
				programData = programData.substr(0, mainPos + 5) + std::to_string(loc) + programData.substr(mainPos + 6);
			}
			break;
		}

		case ShaderType::Loader:
		{
			std::string processedData = std::string(), line = std::string();
			int dataPos = 0, lineStartPos = 0;
			while (dataPos < programData.size())
			{
				const auto c = programData[dataPos];
				if (c == '\n' || c == '\r')
				{
					lineStartPos = dataPos;
					if (!line.empty())
					{
						processedData += line + "\n";
						line = std::string();
					}
				}
				else if (dataPos + repeatToken.size()-1 < programData.size() && AW::StringHelper::startsWith(&programData.at(dataPos), repeatToken))
				{
					for (auto i = 1; i < loc + 1; ++i)
					{
						const auto tokenPos = AW::StringHelper::distanceToLeft(&line, replaceToken);
						auto lineC = line.substr(0, tokenPos + 5) + std::to_string(i) + line.substr(tokenPos + 6);
						processedData += lineC + "\n";
					}
					lineStartPos = dataPos + repeatToken.size();
					dataPos += repeatToken.size();
					line = std::string();
				}
				else
				{
					line += c;
				}

				++dataPos;
			}

			programData = processedData;
		}
		break;
		}

		return programData;
	}

	void Shader::releaseShader()
	{
		for (const auto locToShaderIds : locToShaderIds)
		{
			glDeleteShader(locToShaderIds.second);
		}
		locToShaderIds.clear();
	}

	GLuint Shader::getShaderId(unsigned int location)
	{
		const auto locationId = AW::NumberHelper::clamp<unsigned int>(location, 1, INT_MAX);
		if (locToShaderIds.count(locationId) == 0)
		{
			genShaderForLoc(locationId);
		}

		return locToShaderIds[locationId];
	}

	bool Shader::shaderIsCompiled()
	{
		return !locToShaderIds.empty();
	}
}