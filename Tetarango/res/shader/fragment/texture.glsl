#version 330 core

in vec2 UV;
in mat4 UVp;
uniform sampler2D textureSampler;

vec4 _mainN(vec4 c) 
{
	vec4 loc = UVp * vec4(UV, 1, 1);
	return c * texture(textureSampler, loc.xy).rgba;
};
