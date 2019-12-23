#version 330 core

uniform float frameTime;
uniform float frameStartTime;

uniform float blockEffect;
uniform float blockEffectP;
uniform float blockEffectG;
uniform float blockBorderSize;

const float PI = 3.1415926535897932384626433832795;
const float PI_2 = PI * 2.0;

vec4 pColor;
ivec2 tSize;
vec4 tPos;
vec4 cRect;

void main() 
{
	vec4 effect = vec4(pColor.rgba);

	float period = 10.0;
	float pX = clamp(pow((tPos.x - cRect.x) / cRect.z, 2.0), 0.0, 1.0);
	float pY = clamp(pow((tPos.y - cRect.y) / cRect.w, 2.0), 0.0, 1.0);
	float offsetX = cos((frameStartTime - frameTime * PI_2) / period) * blockEffectG * 6.0;
	float offsetY = sin((frameStartTime - frameTime * PI_2) / period) * blockEffectG;

	float x2 = blockEffectP + offsetX;
	float x3 = 1.0;

	float y2 = blockEffectP + offsetY + 0.65;
	float y3 = 1.0;

	if (pX < x2)
	{
		float p = (pX) / x2;
		effect /= vec4(p, p, p, 1.0);
	}
	else if (pX < x3)
	{
		float p = 1 - ((pX - x2) / (1 - x2));
		effect /= vec4(p, p, p, 1.0);
	}

	if (pY < y2)
	{
		float p = (pY) / y2;
		effect /= vec4(p, p, p, 1.0);
	}
	else if (pY < y3)
	{
		float p = 1 - ((pY - y2) / (1 - y2));
		effect /= vec4(p, p, p, 1.0);
	}

	float borderSize = blockBorderSize / tSize.x;
	bool isBorder = 
		tPos.x < (cRect.x + borderSize) ||
		tPos.x > (cRect.x + cRect.z - borderSize) ||
		tPos.y < (cRect.y + borderSize) ||
		tPos.y > (cRect.y + cRect.w - borderSize);

	float p = blockEffect;

	if (!isBorder)
	{
		vec4 pC = pColor * (1 - p) + effect * p;
		pColor *= pC;
	}
};
