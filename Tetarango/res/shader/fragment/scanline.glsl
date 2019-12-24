#version 330 core

uniform float fScanline;
uniform float fScanlineOX;
uniform float fScanlineOY;
uniform float fScanlineX;
uniform float fScanlineY;
uniform float fScanlineW;
uniform float fScanlineH;
uniform float fScanlineAmount;

uniform float frameTime;
uniform float frameStartTime;

vec4 pColor;
vec4 tPos;
ivec2 tSize;

void main() 
{
	vec4 scanlineSource = vec4(0, 0, 0, 1);
	vec2 offsetFactors = vec2(fScanlineOX, fScanlineOY) / tSize;
	vec4 effect = vec4(pColor);
	int sp = int(fScanline);

	int x = int(round(clamp(fScanlineX, 0.0, 1.0)));
	int cX = int(floor((tPos.x + offsetFactors.x) * tSize.x) * x);
	int sPX = sp + int(clamp(fScanlineW, 1, tSize.x));

	if (cX % sPX >= sp)
	{
		effect = scanlineSource;
	}

	int y = int(round(clamp(fScanlineY, 0.0, 1.0)));
	int cY = int(floor((tPos.y + offsetFactors.y) * tSize.y) * y);
	int sPY = sp + int(fScanlineH);

	if (cY % sPY >= sp)
	{
		effect = scanlineSource;
	}

	pColor = pColor * (1 - fScanlineAmount) + effect * fScanlineAmount;
};
