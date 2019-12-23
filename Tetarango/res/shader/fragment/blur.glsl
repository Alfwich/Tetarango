#version 330 core

uniform sampler2D textureSampler;
uniform float blurAmount;

vec4 pColor;
vec4 tPos;
ivec2 tSize;

const float PI = 3.1415926535897932384626433832795;
const float PI_2 = PI * 2.0;

void main() 
{
	vec4 blurred = vec4(0);
	int samplePasses = 10;
	vec2 offsetAmount = vec2(1.0 / tSize.x, 1.0 / tSize.y);
	vec2 offset = offsetAmount.xy;

	for (int i = 0; i < samplePasses; ++i)
	{
		blurred += texture(textureSampler, vec2(tPos.x + offset.x, tPos.y + offset.y).xy).rgba;
		blurred += texture(textureSampler, vec2(tPos.x + offset.x, tPos.y - offset.y).xy).rgba;
		blurred += texture(textureSampler, vec2(tPos.x - offset.x, tPos.y + offset.y).xy).rgba;
		blurred += texture(textureSampler, vec2(tPos.x - offset.x, tPos.y - offset.y).xy).rgba;

		offset += offsetAmount;
	}

	blurred /= (4 * samplePasses);

	pColor = pColor * (1 - blurAmount) + blurred * blurAmount;
};
