#version 330 core

uniform float fScanline = 0.0;
uniform float fScanlineY = 0.0;
uniform float fScanlineAmount = 0.0;

vec4 pColor;
vec4 tPos;
ivec2 tSize;

void main() 
{
	int cp = int(floor(tPos.x * tSize.x + (tPos.y * tSize.y * fScanlineY)));

	if (cp % int(fScanline) > 0)
	{
		vec4 effect = vec4(0);
		pColor = pColor * (1 - fScanlineAmount) + effect * fScanlineAmount;
	}
};
