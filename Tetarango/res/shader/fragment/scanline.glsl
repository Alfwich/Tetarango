#version 330 core

uniform float fScanlineAmount;

const float PI = 3.1415926535897932384626433832795;
const float PI_2 = PI * 2.0;

vec4 pColor;
vec4 tPos;
ivec2 tSize;

void main() 
{
	int cp = int(floor(tPos.x * tSize.x));

	if (cp % int(fScanlineAmount) > 0)
	{
		pColor = vec4(0);
	}
};
