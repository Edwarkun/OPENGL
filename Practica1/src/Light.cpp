#include "Light.h"

Light::Light(vec3 pos, vec3 dir, float ambient, float diffuse, float specular, LType lType, int number, vec3 color) {
	switch (lType) {
	case DIRECTIONAL:
		LightType = DIRECTIONAL;
		break;
	case POINT:
		LightType = POINT;
		break;
	case SPOT:
		LightType = SPOT;
		MinAperture = cos(radians(10.f));
		MaxAperture = cos(radians(20.f));
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
	//default 50 units light
	c1 = 1.0f;
	c2 = 0.09f;
	c3 = 0.0032f;
}

Light::~Light() {

}
void Light::SetAtt(float constant, float lineal, float quadratic) {
	c1 = constant;
	c2 = lineal;
	c3 = quadratic;
}

void Light::SetAperture(float min, float max) {
	MinAperture = cos(radians(min));
	MaxAperture = cos(radians(max));
}

void Light::SetLight(Shader *shad, vec3 CamPos) {

	std::string variable;
	glUniform3f(glGetUniformLocation(shad->Program, "cameraPosition"), CamPos.x, CamPos.y, CamPos.z);

	switch (LightType){
		case DIRECTIONAL:
			glUniform3f(glGetUniformLocation(shad->Program, "dlight.direction"), Ldirection.x, Ldirection.y, Ldirection.z);
			glUniform3f(glGetUniformLocation(shad->Program, "dlight.ambientFactor"), Lambient, Lambient, Lambient);
			glUniform3f(glGetUniformLocation(shad->Program, "dlight.diffuseFactor"), Ldiffuse, Ldiffuse, Ldiffuse);
			glUniform3f(glGetUniformLocation(shad->Program, "dlight.specularFactor"), Lspecular, Lspecular, Lspecular);
			glUniform3f(glGetUniformLocation(shad->Program, "dlight.color"), Lcolor.x, Lcolor.y, Lcolor.z);
			break;

		case POINT:		
			variable = "plight[" + std::to_string(lightNumber) + "]";		
			glUniform3f(glGetUniformLocation(shad->Program, (variable + ".position").c_str()), Lpos.x, Lpos.y, Lpos.z);
			glUniform3f(glGetUniformLocation(shad->Program, (variable + ".ambientFactor").c_str()), Lambient, Lambient, Lambient);
			glUniform3f(glGetUniformLocation(shad->Program, (variable + ".diffuseFactor").c_str()), Ldiffuse, Ldiffuse, Ldiffuse);
			glUniform3f(glGetUniformLocation(shad->Program, (variable + ".specularFactor").c_str()), Lspecular, Lspecular, Lspecular);
			glUniform3f(glGetUniformLocation(shad->Program, (variable + ".color").c_str()), Lcolor.x, Lcolor.y, Lcolor.z);

			glUniform1f(glGetUniformLocation(shad->Program, (variable + ".constant").c_str()), c1);
			glUniform1f(glGetUniformLocation(shad->Program, (variable + ".linear").c_str()), c2);
			glUniform1f(glGetUniformLocation(shad->Program, (variable + ".quadratic").c_str()), c3);
			break;

		case SPOT:
			variable = "slight[" + std::to_string(lightNumber) + "]";
			glUniform3f(glGetUniformLocation(shad->Program, (variable + ".position").c_str()), Lpos.x, Lpos.y, Lpos.z);
			glUniform3f(glGetUniformLocation(shad->Program, (variable + ".direction").c_str()), Ldirection.x, Ldirection.y, Ldirection.z);
			glUniform3f(glGetUniformLocation(shad->Program, (variable + ".ambientFactor").c_str()), Lambient, Lambient, Lambient);
			glUniform3f(glGetUniformLocation(shad->Program, (variable + ".diffuseFactor").c_str()), Ldiffuse, Ldiffuse, Ldiffuse);
			glUniform3f(glGetUniformLocation(shad->Program, (variable + ".specularFactor").c_str()), Lspecular, Lspecular, Lspecular);
			glUniform3f(glGetUniformLocation(shad->Program, (variable + ".color").c_str()), Lcolor.x, Lcolor.y, Lcolor.z);

			glUniform1f(glGetUniformLocation(shad->Program, (variable + ".constant").c_str()), c1);
			glUniform1f(glGetUniformLocation(shad->Program, (variable + ".linear").c_str()), c2);
			glUniform1f(glGetUniformLocation(shad->Program, (variable + ".quadratic").c_str()), c3);

			glUniform1f(glGetUniformLocation(shad->Program, (variable + ".innerCone").c_str()), MinAperture);
			glUniform1f(glGetUniformLocation(shad->Program, (variable + ".outerCone").c_str()), MaxAperture);
			break;
		default:
			std::cout << "U WOT M8?" << std::endl;
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
vec3 Light::GetPosition() {
	return Lpos;
}