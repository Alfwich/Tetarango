#version 330 core


uniform sampler2D textureSampler;

vec4 pColor;
vec4 tLoc;

void main() 
{
	pColor *= texture(textureSampler, tLoc.xy).rgba;
};
