#pragma once
#define GLEW_STATIC
#include <GL\glew.h>
#include <glm.hpp>
#include "Shader.h"
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>

using namespace glm;

class Light {
public:
	enum LType {
		DIRECTIONAL = 0,
		POINT = 1,
		SPOT = 2
	};
	Light(vec3 pos, vec3 dir, float ambient, float diffuse, float specular, LType lType, int number, vec3 color);
	~Light();
	void SetAtt(float constant, float lineal, float quadratic);
	void SetLight(Shader *shad, vec3 CamPos);
	void Rotate(vec3 rotation);
	void SetDirection(vec3 dir);
	void SetAperture(float min, float max);
	void SetPosition(vec3 pos);
	vec3 GetColor();
	vec3 GetPosition();

private:
	vec3 Ldirection;
	vec3 Lpos;
	float Lambient;
	float Ldiffuse;
	float Lspecular;
	vec3 Lcolor;
	LType LightType;
	float MinAperture;
	float MaxAperture;
	float c1, c2, c3;
	int lightNumber;
};

