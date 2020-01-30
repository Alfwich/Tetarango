#version 330 core

in vec4 colorMod;

uniform float fColorSaturationAmount;
uniform float fColorSaturationOffset;

vec4 pColor;

void main() 
{
	float p = ((pColor.r + pColor.g + pColor.b) / (3.0 - fColorSaturationOffset)) * fColorSaturationAmount;
	pColor = (pColor * colorMod * (1 - p)) + pColor * p;
};

