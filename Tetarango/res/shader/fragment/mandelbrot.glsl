#version 400 core

uniform sampler2D textureSampler;
uniform float iter;

uniform vec3 fColor;

vec4 pColor;
vec4 tPos;
vec4 cRect;
ivec2 tSize;

void main() {
    dvec2 z, c;

    c.x = -2.0 + (tPos.x * 3.0);
    c.y = -1 + (tPos.y * 2.0);

    int i;
	int l = int(iter);
    z = c;
    for(i=0; i<l; i++) 
	{
        double x = (z.x * z.x - z.y * z.y) + c.x;
        double y = (z.y * z.x + z.x * z.y) + c.y;

        if((x * x + y * y) > 4.0) break;
        z.x = x;
        z.y = y;
    }

	vec4 col = vec4(fColor, 1.0);
	if (i == iter)
	{
		discard;
	}
	else
	{
		pColor *= col * (i / iter);
	}

}
