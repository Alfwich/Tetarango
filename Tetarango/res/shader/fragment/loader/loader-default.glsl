#version 330 core
out vec4 color;

in vec2 UV;

uniform sampler2D textureSampler;

void mainN(); //#REPEAT

vec4 pColor;
vec4 tLoc;
vec4 cRect;
ivec2 tSize;

void main() 
{
	pColor = vec4(1.0);
	tLoc = vec4(UV, 1, 1);
	tSize = textureSize(textureSampler, 0);
	cRect = vec4(0, 0, 1.0, 1.0);

	mainN(); //#REPEAT

	if (pColor.a == 0) discard;

	color = pColor;
};
