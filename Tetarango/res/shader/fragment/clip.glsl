#version 330 core

in vec2 UV;

uniform float clipX;
uniform float clipY;
uniform float clipWidth;
uniform float clipHeight;

vec4 tLoc;
vec4 cRect;
ivec2 tSize;

void main() 
{
	tLoc = vec4(UV.x * (clipWidth / tSize.x), UV.y * (clipHeight / tSize.y), 1, 1);
	tLoc = vec4(tLoc.x + clipX * (1.0 / tSize.x), tLoc.y + clipY * (1.0 / tSize.y), 1, 1);
	cRect = vec4(
			cRect.x + clipX * (cRect.z / tSize.x), 
			cRect.y + clipY * (cRect.w / tSize.y), 
			(clipWidth / cRect.z) / tSize.x, 
			(clipHeight / cRect.w) / tSize.x
		);
};
