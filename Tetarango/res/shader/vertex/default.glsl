#version 330 core
layout(location = 0) in vec3 vp;
layout(location = 1) in vec2 vertexUV;

out vec2 UV;
out vec4 colorMod;

uniform mat4 mvp;
uniform vec2 vTranslate;
uniform float vScale = 1.0;
uniform vec4 cMod;

void main()
{
	UV = vertexUV;
	colorMod = cMod;

	vec4 p = mvp * vec4(vp, 1);
	p += vec4(vTranslate, 0.0, 0.0);
	p *= vec4(1, 1, 1, 1.0 / float(vScale));
	gl_Position = p;
};
