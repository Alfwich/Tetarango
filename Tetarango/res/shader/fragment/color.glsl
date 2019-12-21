#version 330 core

in vec4 colorMod;

vec4 _mainN(vec4 c) 
{
	return c * colorMod;
};

