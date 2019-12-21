#version 330 core

uniform float alpha;

vec4 _mainN(vec4 c) 
{
	return vec4(alpha * c.xyz, alpha);
};
