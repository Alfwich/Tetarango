#version 330 core

uniform float fCircleEdge;

vec4 pColor;
vec4 cRect;
vec4 tPos;

void main() 
{
	vec2 center = cRect.xy  + (cRect.zw / 2.0);
	float x = (center.x - cRect.x) - (tPos.x - cRect.x);
	float y = (center.y - cRect.y) - (tPos.y - cRect.y);
	float r = sqrt(x * x + y * y);

	float f = min(cRect.z, cRect.w) / (2.0 + fCircleEdge);
	float t = min(cRect.z, cRect.w) / 2.0;
	if (r > f)
	{
		float p = (r - f) / (t - f);
		pColor.a *= 1 - p;
	}
};

