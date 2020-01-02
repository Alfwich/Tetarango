#version 330 core

uniform float frameTime;
uniform float frameStartTime;

uniform float fPulsateMin;
uniform float fPulsateMax;

vec4 pColor;

const float PI = 3.1415926535897932384626433832795;
const float PI_2 = PI * 2.0;

void main() 
{
	pColor = vec4(pColor.rgb * (fPulsateMin + (fPulsateMax - fPulsateMin) * abs(sin((frameTime - frameStartTime) * PI))), pColor.a);
};
