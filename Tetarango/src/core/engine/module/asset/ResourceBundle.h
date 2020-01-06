#pragma once

namespace AW
{
	class ResourceBundle
	{
	public:
		ResourceBundle(std::unique_ptr<char[]> data, unsigned int size) : size(size), data(std::move(data)) {}
		ResourceBundle(const std::string& data) : size((unsigned int)data.size()), data(std::make_unique<char[]>(data.size()))
		{
			for (unsigned int i = 0; i < size; ++i) this->data.get()[i] = data[i];
		}
		const unsigned int size;
		const std::unique_ptr<char[]> data;
	};
}
