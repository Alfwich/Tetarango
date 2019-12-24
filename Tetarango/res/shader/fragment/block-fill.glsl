#version 330 core

uniform float frameTime;
uniform float frameStartTime;
uniform float random;

uniform float blockBorderSize;
uniform float blockCenterFill;

const float PI = 3.1415926535897932384626433832795;
const float PI_2 = PI * 2.0;

vec4 pColor;
ivec2 tSize;
vec4 tPos;
vec4 cRect;

void main() 
{
	float pY = (tPos.y - cRect.y) / cRect.w;
	float borderSize = blockBorderSize / tSize.x;
	bool isBorder = 
		tPos.x < (cRect.x + borderSize) ||
		tPos.x > (cRect.x + cRect.z - borderSize) ||
		tPos.y < (cRect.y + borderSize) ||
		tPos.y > (cRect.y + cRect.w - borderSize);

	float yOffset = sin((((tPos.x - cRect.x) / cRect.z + frameTime - frameStartTime) * 2) * PI_2 / 3.0 + random * 4.0) * 0.0125;
	float cutoffY = (1 - blockCenterFill) + yOffset;
	float d = clamp((cutoffY - pY) * 8.0, 0.0, 1.0);

	if (!isBorder && blockCenterFill < 1.0 && pY < cutoffY)
	{
		vec4 dC = vec4(pColor.rgb * (1 - d), 1.0);
		vec4 bC = vec4(min(pColor.rgb, 0.25), 1.0);

		pColor = dC * (1 - d) + d * bC;
	}

	if (pY > cutoffY && !isBorder)
	{
		pColor *=  2;
	}
};
