#version 330 core

uniform float frameTime;
uniform float frameStartTime;

uniform float pulsateAmount;

const float PI = 3.1415926535897932384626433832795;
const float PI_2 = PI * 2.0;

vec4 _mainN(vec4 c) 
{
	return c * (1 - pulsateAmount) + (c * sin((frameTime - frameStartTime) * PI_2) * pulsateAmount);
};
