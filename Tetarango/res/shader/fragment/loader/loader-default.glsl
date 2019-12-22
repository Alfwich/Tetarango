#version 330 core
out vec4 color;

in vec2 UV;
in mat4 UVp;

uniform sampler2D textureSampler;

vec4 _mainN(vec4 c); //#REPEAT

vec4 texLoc;
ivec2 texSize;

void main() 
{
	texLoc = UVp * vec4(UV, 1, 1);
	texSize = textureSize(textureSampler, 0);

	vec4 c = vec4(1.0);

	c = _mainN(c); //#REPEAT

	if (c.a == 0) discard;

	color = c;
};
