#version 330 core

vec4 pColor;

void main() 
{
	pColor = vec4(1 - pColor.r, 1 - pColor.g, 1 - pColor.b, pColor.a);
};
