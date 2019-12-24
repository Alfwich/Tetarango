#version 330 core

uniform sampler2D textureSampler;

vec4 pColor;
vec4 tPos;

void main() 
{
	pColor *= texture(textureSampler, tPos.xy).rgba;
};
