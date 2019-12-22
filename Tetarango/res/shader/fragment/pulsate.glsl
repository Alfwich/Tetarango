#version 330 core


uniform float frameTime;
uniform float frameStartTime;
uniform float pulsateAmount;

vec4 pColor;

const float PI = 3.1415926535897932384626433832795;
const float PI_2 = PI * 2.0;

void main() 
{
	pColor = pColor * (1 - pulsateAmount) + (pColor * sin((frameTime - frameStartTime) * PI_2) * pulsateAmount);
};
