#version 330 core
in vec2 UV;
in vec4 colorMod;
in mat4 UVp;
out vec4 color;
uniform sampler2D textureSampler;

void main() 
{
	vec4 loc = UVp * vec4(UV, 1, 1);
	vec4 c = texture(textureSampler, loc.xy).rgba;
	vec4 fC = c * colorMod;
	if (fC.a == 0) discard;
	color = fC;
};
