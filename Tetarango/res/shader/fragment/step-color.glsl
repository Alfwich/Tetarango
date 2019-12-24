#version 330 core

uniform float fStepOldSpace;
uniform float fStepC;

vec4 pColor;

void main() 
{
	float oldSpace = clamp(fStepOldSpace, 0.0, 256.0);
	if (oldSpace == 0.0)
	{
		oldSpace = 256.0;
	}

	float conversionFactor = fStepC / oldSpace;

	float oldRedSpaceValue = pColor.r * oldSpace;
	float newRedSpaceValue = floor(oldRedSpaceValue * conversionFactor);
	float newRed = newRedSpaceValue / fStepC;

	float oldGreenSpaceValue = pColor.g * oldSpace;
	float newGreenSpaceValue = floor(oldGreenSpaceValue * conversionFactor);
	float newGreen = newGreenSpaceValue / fStepC;

	float oldBlueSpaceValue = pColor.b * oldSpace;
	float newBlueSpaceValue = floor(oldBlueSpaceValue * conversionFactor);
	float newBlue = newBlueSpaceValue / fStepC;

	pColor = vec4(newRed, newGreen, newBlue, pColor.a);
};

