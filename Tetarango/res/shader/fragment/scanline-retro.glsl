#version 330 core

uniform float fScanline = 0.0;
uniform float fScanlineY = 0.0;
uniform float fScanlineRetroAmount = 0.0;

vec4 pColor;
vec4 tPos;
ivec2 tSize;

void main() 
{
	vec4 effect = vec4(pColor.rgb, pColor.a);
	int cp = int(floor(tPos.x * tSize.x + (tPos.y * tSize.y * fScanlineY)));
	int cmpL = cp % 3;

	switch(cmpL)
	{
	case 0:
		effect = vec4(effect.r, 0, 0, effect.a);
		break;

	case 1:
		effect = vec4(0, effect.g, 0, effect.a);
		break;

	case 2:
		effect = vec4(0, 0, effect.b, effect.a);
		break;

	default:
		effect = vec4(effect.rgb * 0.25, effect.a);
		break;
	}

	pColor = pColor * (1 - fScanlineRetroAmount) + effect * fScanlineRetroAmount;
};
