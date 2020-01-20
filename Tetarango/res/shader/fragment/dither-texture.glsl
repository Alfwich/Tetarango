#version 330 core

uniform sampler2D textureSampler;

uniform float fDitherTextureAmount;

vec4 pColor;
vec4 tPos;

void main() 
{
	pColor = pColor + texture(textureSampler, tPos.xy).rgba * fDitherTextureAmount;
};
