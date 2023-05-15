#version 440 core

uniform float time;

in vec2 texCoord0;
in vec3 FragPosition;

out vec4 FragColor;

float maxiterations = 128;

float mandelbrot(vec2 uv)
{
	vec2 c = 2 * uv - vec2(0.7, 0.7);
	c = c / pow(time, 1) - vec2(0.65, 0.45);
	vec2 z = vec2(0.0);
	float iter = 0.0;
	for (float i; i < maxiterations; i++)
	{
		z = vec2(z.x * z.x - z.y * z.y, 2 * z.x * z.y) + c;
		if(dot(z, z) > 2) return iter / maxiterations; 
		iter++;
	}
	return 0.0;
}

vec3 hashfunc(float m)
{
	float x = fract(sin(m) * 56255.246);
	float y = fract(sin(m + x) * 2218.486);
	float z = fract(sin(x + y) * 8653.352);

	return vec3(x, z, y);
}
void main() 
{
	vec3 col = vec3(0.0);

	float m = mandelbrot(texCoord0);
	col.rb +=  hashfunc(m).rb;
	col = pow(col, vec3(0.45));

	FragColor = vec4(col, 1.0);
}
