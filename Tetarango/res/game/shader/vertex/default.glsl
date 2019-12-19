#version 330 core
layout(location = 0) in vec3 vp;
layout(location = 1) in vec2 vertexUV;
out vec2 UV;
out mat4 UVp;
out vec4 colorMod;
uniform mat4 mvp;
uniform mat4 UVproj;
uniform vec4 cMod;

void main()
{
	vec4 p = mvp * vec4(vp, 1);
	gl_Position = p;
	UV = vertexUV;
	UVp = UVproj;
	colorMod = cMod;
};
