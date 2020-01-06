#pragma once

namespace AW
{
	enum ImageBundleType
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
		int width;
		int height;
		ImageBundleType type;
	};
}
