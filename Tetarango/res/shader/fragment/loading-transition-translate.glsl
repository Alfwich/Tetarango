#version 330 core

in vec2 UV;

vec4 tPos;

uniform float fLoadingP;

float detRP(vec2 a, vec2 b, vec2 c) {
     return ((b.x - a.x)*(c.y - a.y) - (b.y - a.y)*(c.x - a.x));
}

void main() 
{
	if (detRP(vec2(fLoadingP, 0.0), vec2(fLoadingP + 1.0 * fLoadingP, 1.0), UV) < 0) 
	{
		discard;
	}

	tPos.x += fLoadingP * 10.0;
	tPos.y += fLoadingP * 10.0;
}
