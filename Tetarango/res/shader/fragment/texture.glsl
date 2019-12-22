#version 330 core


uniform sampler2D textureSampler;

vec4 pColor;
vec4 texLoc;

void main() 
{
	pColor *= texture(textureSampler, texLoc.xy).rgba;
};
