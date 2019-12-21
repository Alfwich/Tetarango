#version 330 core

in vec2 UV;
in mat4 UVp;
uniform sampler2D textureSampler;

uniform float effectAmount;
uniform float frameTime;
uniform float frameStartTime;

const float PI = 3.1415926535897932384626433832795;
const float PI_2 = PI * 2.0;
const float effectOffset = -10.0;

vec4 _mainN(vec4 c) 
{
	ivec2 size = textureSize(textureSampler, 0);
	vec4 loc = UVp * vec4(UV, 1, 1);

	float xPos = loc.x * size.x;
	float yPos = loc.y * size.y;
	if (xPos > 2 && xPos < 30 && yPos > 2 && yPos < 30 && (c.r > 0.0 || c.g > 0.0 || c.b > 0.0)) 
	{
		float mag = (loc.x + loc.y) * (effectAmount + effectOffset) * sin((frameTime - frameStartTime) * PI_2);
		c.r = c.r + (c.r * mag);
		c.g = c.g + (c.g * mag);
		c.b = c.b + (c.b * mag);
	}

	return c;
};
