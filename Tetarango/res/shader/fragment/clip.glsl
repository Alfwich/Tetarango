#version 330 core

in vec2 UV;

uniform float clipX;
uniform float clipY;
uniform float clipWidth;
uniform float clipHeight;

vec4 texLoc;
ivec2 texSize;

void main() 
{
	texLoc = vec4(UV.x * (clipWidth / texSize.x), UV.y * (clipHeight / texSize.y), 1, 1);
	texLoc = vec4(texLoc.x + clipX * (1.0 / texSize.x), texLoc.y + clipY * (1.0 / texSize.y), 1, 1);
	texSize = ivec2(clipWidth, clipHeight);
};
