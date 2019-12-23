#version 330 core

uniform float frameTime;
uniform float frameStartTime;

uniform float blockEffect;
uniform float blockEffectPulsate;
uniform float borderSize;

const float PI = 3.1415926535897932384626433832795;
const float PI_2 = PI * 2.0;

vec4 pColor;
ivec2 tSize;
vec4 tPos;
vec4 cRect;

void main() 
{
	float effectTime = 1.0;
	float offset = 3.0;
	float amplitude = 2.0;

	float maxCmp = max(pColor.r, max(pColor.g, pColor.b));
	vec3 factors = vec3(maxCmp / 1.0);
	vec3 eColor = vec3(pColor);
	float pulsateOffset = sin(((frameTime - frameStartTime) * PI_2) / effectTime) * blockEffectPulsate;

	float xF = (((tPos.x - cRect.x) / cRect.z) + offset + pulsateOffset) / amplitude;
	eColor *= xF * factors;

	float yF = (((tPos.y - cRect.y) / cRect.w) + offset + pulsateOffset) / amplitude;
	eColor *= yF * factors * 0.5;

	float borderSize = borderSize / tSize.x;
	bool isBorder = 
		tPos.x < (cRect.x + borderSize) ||
		tPos.x > (cRect.x + cRect.z - borderSize) ||
		tPos.y < (cRect.y + borderSize) ||
		tPos.y > (cRect.y + cRect.w - borderSize);

	float p = blockEffect;

	if (isBorder)
	{
		p *= 0.5;
	}

	pColor = (1 - p) * pColor + vec4(eColor, pColor.a) * p;
};
