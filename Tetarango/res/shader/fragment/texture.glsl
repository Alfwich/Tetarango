#version 330 core

uniform sampler2D textureSampler;

vec4 texLoc;

vec4 _mainN(vec4 c) 
{
	return c * texture(textureSampler, texLoc.xy).rgba;
};
