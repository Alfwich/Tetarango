#version 330 core

uniform float frameTime;
uniform float frameStartTime;

uniform float pulsateMin;
uniform float pulsateMax;

vec4 pColor;

const float PI = 3.1415926535897932384626433832795;
const float PI_2 = PI * 2.0;

void main() 
{
	pColor = vec4(pColor.rgb * (pulsateMin + (pulsateMax - pulsateMin) * abs(sin((frameTime - frameStartTime) * PI))), pColor.a);
};
