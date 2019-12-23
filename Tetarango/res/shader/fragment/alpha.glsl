#version 330 core

uniform float alpha;

vec4 pColor;

void main() 
{
	pColor = vec4(alpha * pColor.rgb, alpha);
};
