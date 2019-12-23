#version 330 core

uniform float cornerSize;
uniform float targetWidth;
uniform float targetHeight;

vec4 tLoc;
vec4 cRect;
ivec2 tSize;

void main() 
{
	float renderSpaceCornerSizeWidth = (cornerSize / targetWidth) * cRect.z;
	float textureSpaceCornerSizeWidth = cornerSize / tSize.x;

	float x = tLoc.x;

	float x1 = cRect.x;
	float x2 = x1 + renderSpaceCornerSizeWidth;
	float x4 = cRect.x + cRect.z;
	float x3 = x4 - renderSpaceCornerSizeWidth;

	if (x <= x2)
	{
		float d = (x - x1) / renderSpaceCornerSizeWidth;
		tLoc.x = d * textureSpaceCornerSizeWidth;
	}
	else if (x > x2 && x < x3)
	{
		tLoc.x = cRect.z / 2.0;
	}
	else if (x >= x3)
	{
		float d = (x - x3) / renderSpaceCornerSizeWidth;
		tLoc.x = (d * textureSpaceCornerSizeWidth) + (cRect.z - textureSpaceCornerSizeWidth);
	}

	float renderSpaceCornerSizeHeight = (cornerSize / targetHeight) * cRect.z;
	float textureSpaceCornerSizeHeight = cornerSize / tSize.x;

	float y = tLoc.y;

	float y1 = cRect.y;
	float y2 = y1 + renderSpaceCornerSizeHeight;
	float y4 = cRect.y + cRect.w;
	float y3 = y4 - renderSpaceCornerSizeHeight;

	if (y <= y2)
	{
		float d = (y - y1) / renderSpaceCornerSizeHeight;
		tLoc.y = d * textureSpaceCornerSizeHeight;
	}
	else if (y > y2 && y < y3)
	{
		tLoc.y = cRect.w / 2.0;
	}
	else if (y >= y3)
	{
		float d = (y - y3) / renderSpaceCornerSizeHeight;
		tLoc.y = (d * textureSpaceCornerSizeHeight) + (cRect.w - textureSpaceCornerSizeHeight);
	}

	tLoc.x += cRect.x;
	tLoc.y += cRect.y;
};
