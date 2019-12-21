#version 330 core

vec4 _mainN(vec4 c) 
{
	return vec4(1 - c.r, 1 - c.g, 1 - c.b, c.a);
};
