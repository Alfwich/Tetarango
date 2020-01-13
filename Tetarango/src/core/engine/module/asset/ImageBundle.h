#pragma once

namespace AW
{
	enum class ImageBundleType
	{
		Unspecificed,
		Png,
		Raw,
		Error
	};

	class ImageBundle
	{
	public:
		std::vector<unsigned char> data;
		int width = 0;
		int height = 0;
		ImageBundleType type = ImageBundleType::Unspecificed;
	};
}
