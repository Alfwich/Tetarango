#version 330 core

in vec4 colorMod;

vec4 pColor;

void main() 
{
	pColor *= colorMod;
};

