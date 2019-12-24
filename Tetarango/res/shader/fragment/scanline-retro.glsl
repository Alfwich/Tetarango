#version 330 core

uniform float fScanline;
uniform float fScanlineRetroAmount;

vec4 pColor;
vec4 tPos;
ivec2 tSize;

void main() 
{
	vec4 effect = vec4(pColor.rgb, pColor.a);
	int cp = int(floor(tPos.x * tSize.x));
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

	int yp = int(floor(tPos.y * tSize.y));
	if (yp % 2 == 0)
	{
		effect *= 0.75;
	}

	pColor = pColor * (1 - fScanlineRetroAmount) + effect * fScanlineRetroAmount;
};
