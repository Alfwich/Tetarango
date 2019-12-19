#version 330 core
in vec2 UV;
in vec4 colorMod;
in mat4 UVp;
out vec4 color;
uniform sampler2D textureSampler;
uniform float blurAmount;

void main() 
{
	vec4 loc = UVp * vec4(UV, 1, 1);
	ivec2 size = textureSize(textureSampler, 0);
	vec4 c;
	float f = 0;
	if (blurAmount <= 0)
	{
		c = texture(textureSampler, loc.xy).rgba;
		f = 1.0;
	}
	else
	{
		for (float x = -(blurAmount); x < blurAmount; x += 1.0)
		{
			for (float y = -blurAmount; y < blurAmount; y += 1.0)
			{
				c += texture(textureSampler, (loc + vec4(x / size.x, y / size.y, 0, 0)).xy).rgba;
				f += 1.0;
			}
		}
	}

	c /= vec4(f, f, f, f);

	vec4 fC = c * colorMod;
	if (fC.a == 0) discard;

	color = fC;
};
