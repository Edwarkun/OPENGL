#include "Light.h"

Light::Light(vec3 pos, vec3 dir, vec3 ambient, vec3 diffuse, vec3 specular, LType lType, int number, vec3 color) {
	switch (lType) {
	case DIRECTIONAL:
		LightType = DIRECTIONAL;
		break;
	case POINT:
		LightType = POINT;
		break;
	case SPOT:
		LightType = SPOT;
		MinAperture = 10.f;
		MaxAperture = 20.f;
		break;
	default:
		break;
	}
	Lpos = pos;
	Ldirection = dir;
	Lambient = ambient;
	Ldiffuse = diffuse;
	Lspecular = specular;
	lightNumber = number;
	Lcolor = color;
}


void Light::SetAtt(float constant, float lineal, float quadratic) {
	c1 = constant;
	c2 = lineal;
	c3 = quadratic;
}

void Light::SetAperture(float min, float max) {
	MinAperture = min;
	MaxAperture = max;
}

void Light::SetLight(Shader *shad, vec3 CamPos) {
	std::string variable;

	glUniform3f(glGetUniformLocation(shad->Program, "viewPos"), CamPos.x, CamPos.y, CamPos.z);
	switch (LightType){
	case DIRECTIONAL:

		break;
	case POINT:		
		variable = "plight[" + std::to_string(lightNumber) + "]";		

		break;
	case SPOT:
		variable = "slight[" + std::to_string(lightNumber) + "]";

		break;
	default:
		break;
	}	
}

void Light::Rotate(vec3 rotation) {	
//opcional
}

void Light::SetDirection(vec3 dir) {
	Ldirection = dir;
}

void Light::SetPosition(vec3 pos) {
	Lpos = pos;
}

vec3 Light::GetColor() {
	return Lcolor;
}