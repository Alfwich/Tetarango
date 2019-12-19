#version 330 core
in vec4 colorMod;
out vec4 color;

void main() 
{
	if (colorMod.a == 0) discard;
	color = colorMod;
};

