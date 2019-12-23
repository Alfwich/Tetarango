#version 330 core

vec4 pColor;

void main() 
{
	pColor = vec4(1 - pColor.rgb, pColor.a);
};
