#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>

const std::string assetStart = "##ASSET_START##";
const std::string assetEnd = "##ASSET_END##";
const std::string binaryDelimiter = "#B";

std::string assetNameFromPath(std::string path)
{
	std::string result;
	unsigned int i = 0;
	bool hitRes = false;

	while (i < path.size())
	{
		if (!hitRes && path[i] == '/' && path[i + 1] == 'r' && path[i + 2] == 'e' && path[i + 3] == 's' && path[i + 4] == '/')
		{
			hitRes = true;
			++i;
			continue;
		}

		if (hitRes)
		{
			if (path[i] == '/' || path[i] == '\\')
			{
				result += '_';
			}
			else
			{
				result += path[i];
			}
		}
		++i;
	}

	return result;
}

int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		std::cout << "Expected useage: asset-pack <asset-path-to-pack>... <asset-package-path>" << std::endl;
		return -1;
	}

	std::vector<char> data;
	const auto packLocation = std::string(argv[argc - 1]);
	for (int i = 1; i < argc - 1; ++i)
	{
		const auto path = std::string(argv[i]);

		std::cout << "path= " << path << std::endl;
		std::cout << "packLocation= " << packLocation << std::endl;

		std::ifstream inputFile;
		inputFile.open(path, std::ios::binary);
		inputFile.unsetf(std::ios::skipws);

		std::streampos fileSize;
		inputFile.seekg(0, std::ios::end);
		fileSize = inputFile.tellg();
		inputFile.seekg(std::ios::beg);

		data.reserve(fileSize);

		char* buf = new char[fileSize];
		inputFile.read(buf, fileSize);

		for (const auto c : assetStart)
		{
			data.push_back(c);
		}

		const auto assetName = assetNameFromPath(path);
		for (const auto c : assetName)
		{
			data.push_back(c);
		}

		for (const auto c : binaryDelimiter)
		{
			data.push_back(c);
		}

		for (unsigned int i = 0; i < fileSize; ++i)
		{
			data.push_back(buf[i]);
		}

		for (const auto c : assetEnd)
		{
			data.push_back(c);
		}

		inputFile.close();

		delete[] buf;
	}

	std::ofstream outputFile;
	outputFile.open(packLocation, std::ios::app | std::ios::binary);
	outputFile.unsetf(std::ios::skipws);

	unsigned int i = 0, p = 0;
	std::string key = "sd6tSaIECaZqB2ZzpQIABjs4LnDrT89mzKq9S92jZbWQYsr8ZXNSXtsLphWSSgzx";
	if (outputFile.is_open())
	{
		char* buf = new char[data.size()];
		for (auto byte : data)
		{
			buf[i] = byte ^ key[p++ % key.size()];
			++i;
		}

		outputFile.write(buf, data.size());
		outputFile.close();

		delete[] buf;
	}

	return 0;
}

