#version 330 core

uniform float fRepeat;

vec4 tPos;

void main() 
{
	tPos = vec4(tPos.xy * fRepeat, tPos.z, tPos.w);
};
