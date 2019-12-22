#version 330 core

uniform float cornerSize;
uniform float targetWidth;
uniform float targetHeight;

vec4 texLoc;
ivec2 texSize;

void main() 
{
	float widthR = cornerSize * (1.0 / targetWidth);
	float heightR = cornerSize * (1.0 / targetHeight);

	float widthP = cornerSize * (1.0 / texSize.x);
	float heightP = cornerSize * (1.0 / texSize.y);

	if (texLoc.x < widthR)
	{
		texLoc.x = (1 - ((widthR - texLoc.x) / widthR)) * widthP;
	}
	else if (texLoc.x > 1 - widthR)
	{
		texLoc.x = (1.0 / texSize.x) * (texSize.x / 2.0);
	}
	else
	{
		texLoc.x = (1.0 / texSize.x) * (texSize.x / 2.0);
	}

	if (texLoc.y < heightR)
	{
		texLoc.y = (1 - ((heightR - texLoc.y) / heightR)) * heightP;
	}
	else if (texLoc.x > 1 - widthR)
	{
		texLoc.y = (1.0 / texSize.y) * (texSize.y / 2.0);
	}
	else
	{
		texLoc.y = (1.0 / texSize.y) * (texSize.y / 2.0);
	}
};
