#version 330 core

in vec2 UV;

uniform float clipX;
uniform float clipY;
uniform float clipWidth;
uniform float clipHeight;

vec4 tPos;
vec4 cRect;
ivec2 tSize;

void main() 
{
	if (clipWidth > 0.0 && clipHeight > 0.0)
	{
		tPos = vec4(UV.x * (clipWidth / tSize.x), UV.y * (clipHeight / tSize.y), 1, 1);
		tPos = vec4(tPos.x + clipX * (1.0 / tSize.x), tPos.y + clipY * (1.0 / tSize.y), 1, 1);
		cRect = vec4(
				clipX * (1.0 / tSize.x), 
				clipY * (1.0 / tSize.y), 
				clipWidth / tSize.x, 
				clipHeight / tSize.x
			);
	}
};
