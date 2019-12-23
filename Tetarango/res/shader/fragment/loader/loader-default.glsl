#version 330 core
out vec4 color;

in vec2 UV;

uniform sampler2D textureSampler;

void mainN(); //#REPEAT

vec4 pColor;
vec4 tPos;
ivec2 tSize;
vec4 cRect;

void main() 
{
	pColor = vec4(1.0);
	tPos = vec4(UV, 1, 1);
	tSize = textureSize(textureSampler, 0);
	cRect = vec4(0, 0, 1.0, 1.0);

	mainN(); //#REPEAT

	if (pColor.a == 0) discard;

	color = pColor;
};
