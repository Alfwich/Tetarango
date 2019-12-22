#version 330 core
out vec4 color;

in vec2 UV;

uniform sampler2D textureSampler;

void mainN(); //#REPEAT

vec4 pColor;
vec4 texLoc;
ivec2 texSize;

void main() 
{
	pColor = vec4(1.0);
	texLoc = vec4(UV, 1, 1);
	texSize = textureSize(textureSampler, 0);

	mainN(); //#REPEAT

	if (pColor.a == 0) discard;

	color = pColor;
};
