#version 330 core
out vec4 color;

vec4 _mainN(vec4 c); //#RE

void main() 
{
	vec4 c = vec4(1.0, 1.0, 1.0, 1.0);

	c = _mainN(c); //#RE

	if (c.a == 0) discard;

	color = c;
};
