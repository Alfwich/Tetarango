#version 330 core

in vec2 UV;
in mat4 UVp;
uniform sampler2D textureSampler;
uniform float blurAmount;

vec4 blur(sampler2D image, vec2 uv, vec2 resolution, vec2 direction) 
{
	vec4 color = vec4(0.0);
	vec2 o1 = vec2(1.411764705882353) * direction;
	vec2 o2 = vec2(3.2941176470588234) * direction;
	vec2 o3 = vec2(5.176470588235294) * direction;

	color += texture2D(image, uv) * 0.1964825501511404;
	color += texture2D(image, uv + (o1 / resolution)) * 0.2969069646728344;
	color += texture2D(image, uv - (o1 / resolution)) * 0.2969069646728344;
	color += texture2D(image, uv + (o2 / resolution)) * 0.09447039785044732;
	color += texture2D(image, uv - (o2 / resolution)) * 0.09447039785044732;
	color += texture2D(image, uv + (o3 / resolution)) * 0.010381362401148057;
	color += texture2D(image, uv - (o3 / resolution)) * 0.010381362401148057;

	return color;
}

vec4 _mainN(vec4 c) 
{
 	vec4 loc = UVp * vec4(UV, 1, 1);
	ivec2 size = textureSize(textureSampler, 0);
	vec4 tC = texture(textureSampler, loc.xy).rgba;
	if (blurAmount > 0.0)
	{
		float p = clamp(blurAmount, 0.0, 1.0);
		vec4 blured = blur(textureSampler, loc.xy, size, vec2(5, 5));
		c *= blured * p + tC * (1.0 - p);
	}
	else
	{
		c *= tC;
	}

	return c;
};
