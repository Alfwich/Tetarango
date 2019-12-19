#version 330 core
in vec2 UV;
in vec4 colorMod;
in mat4 UVp;
out vec4 color;
uniform sampler2D textureSampler;
uniform float effectAmount;

void main() 
{
	vec4 loc = UVp * vec4(UV, 1, 1);
	ivec2 size = textureSize(textureSampler, 0);
	vec4 c = texture(textureSampler, loc.xy).rgba;
	vec4 fC = c * colorMod;

	if (fC.a == 0) discard;

	float xPos = loc.x * size.x;
	float yPos = loc.y * size.y;
	if (xPos > 2 && xPos < 30 && yPos > 2 && yPos < 30 && (fC.r > 0.0 || fC.g > 0.0 || fC.b > 0.0)) 
	{
		float mag = (loc.x + loc.y);
		fC.r += mag * effectAmount;
		fC.g += mag * effectAmount;
		fC.b += mag * effectAmount;
	}

	color = fC;
};
