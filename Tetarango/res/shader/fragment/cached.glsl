#version 330 core

vec4 tPos;

void main() 
{
	tPos = vec4(tPos.x, 1 - tPos.y, 1, 1);
};
