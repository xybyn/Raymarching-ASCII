#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <cmath>

#define MAX_STEPS 50
#define MAX_DIST 100.
#define ACCURACY 0.01

float min(float a, float b)
{
	return a < b ? a : b;
}

struct vec3
{
	float x, y, z;

	vec3 operator+(const vec3 &other)
	{
		return vec3{x + other.x, y + other.y, z + other.z};
	}

	vec3 operator-(const vec3 &other)
	{
		return vec3{x - other.x, y - other.y, z - other.z};
	}

	vec3 &operator*(float scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		return *this;
	}
};

float dot(const vec3 &a, const vec3 &b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

float length(const vec3 &a)
{
	return sqrtf(dot(a, a));
}
vec3 normalize(vec3 a)
{
	float coef = (1.0f / length(a));
	return a * coef;
}



float saturate(float v)
{
	if (v > 1.0f)
		return 1.0f;
	if (v < 0.0f)
		return 0.0f;

	return v;
}

char shades[]
{
	' ',
	'.',
	'-',
	'~',
	'*',
	'=',
	'?',
	'#',
	'&',
	'@'
};
char get_symbol(float shade)
{
	int shades_num = sizeof(shades) / sizeof(shades[0]);

	shade = saturate(shade);
	//printf("shade %f\n", shade);
	float step = 1.0f / shades_num;
	for (int i = 0; i < shades_num; i++)
	{
		if (shade < (i + 1) * step)
			return shades[i];
	}
}

float get_distance_to_sphere(vec3 p)
{
	float r = 0.3f;
	vec3 pos{0.0, 1.f, 6.f};
	return length(p - pos) - r;
}

float get_distance_to_plane(vec3 p)
{
	return p.y;
}
float get_distance(vec3 p)
{

	float sphere = get_distance_to_sphere(p);
	float plane = get_distance_to_plane(p);
	return plane;
}
float ray_march(vec3 ro, vec3 rd)
{
	float dO = 0.0;

	for (int i = 0; i < MAX_STEPS; i++)
	{
		vec3 p = ro + rd * dO;
		float d = get_distance(p);
		dO += d;
		if (d < ACCURACY || dO > MAX_DIST)
			break;
	}
	return dO;

}

int main()
{
	constexpr int width = 80;
	constexpr int height = 80;

	char image[width * height];


	float norm_coeff = 60.0f;

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			int y = i - width / 2;
			int x = -j + height / 2;

			vec3 ro = {0, 1, 0};
			vec3 rd = {x, y, 0};
			rd = normalize(rd);

			float t = ray_march(ro, rd);

			vec3 p = ro + rd * t;

			float result_color = get_distance(p);

			image[i * width + j] = get_symbol(result_color / norm_coeff);
		}
	}

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			printf("%c ", image[i * width + j]);
		}
		putchar('\n');
	}

}

