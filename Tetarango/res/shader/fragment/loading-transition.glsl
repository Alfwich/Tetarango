#version 330 core

in vec2 UV;

uniform float fLoadingP;

vec4 tPos;
vec4 pColor;

void main() 
{
	vec4 black = vec4(0, 0, 0, 1.0);
	float bP = fLoadingP;
	pColor = pColor * (1 - bP) + black * bP;
}
