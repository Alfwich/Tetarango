﻿#version 330 core

uniform float fStepT;

vec4 pColor;

ivec2 tSize;
vec4 tPos;
vec4 cRect;

void main() 
{
	if (fStepT > 1.0)
	{
		float xP = tPos.x * tSize.x;
		float nXP = floor(xP / fStepT);
		tPos.x = nXP * (fStepT / tSize.x);

		float yP = tPos.y * tSize.y;
		float nYP = floor(yP / fStepT);
		tPos.y = nYP * (fStepT / tSize.y);
	}
};

