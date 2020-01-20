#version 330 core

uniform vec4 fColorA;
uniform vec4 fColorB;

vec4 pColor;
ivec2 tSize;
vec4 tPos;
vec4 cRect;

void main() 
{
	float p = tPos.y / tPos.w;
	pColor *= fColorA * (1 - p) + fColorB * p;
};

