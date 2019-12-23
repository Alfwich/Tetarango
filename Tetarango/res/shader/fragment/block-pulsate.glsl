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
	float borderSize = borderSize / tSize.x;

	bool isBorder = 
		tPos.x < (cRect.x + borderSize) ||
		tPos.x > (cRect.x + cRect.z - borderSize) ||
		tPos.y < (cRect.y + borderSize) ||
		tPos.y > (cRect.y + cRect.w - borderSize);

	if (!isBorder)
	{
		vec4 eColor = pColor;

		float xF = ((tPos.x - cRect.x) / cRect.z) + sin((frameTime - frameStartTime) * PI_2) * blockEffectPulsate;
		eColor.rgb *= xF;

		float yF = ((tPos.y - cRect.y) / cRect.w) + sin((frameTime - frameStartTime) * PI_2) * blockEffectPulsate;
		eColor.rgb *= yF;

		float p = blockEffect;
		pColor = (1 - p) * pColor + eColor * p;
	}
};
