#include "material.h"

Material::Material(char *DiffPath, char*SpectPath, float Shine){
	diffusePath = DiffPath;
	specularPath = SpectPath;
	Shininess = Shine;
}


Material::~Material(){
}

void Material::SetMaterial(Shader *shad) {
	//We bind the first texture to the uniform variable "Texture1" in the fragment shader
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TextDiff);
	glUniform1i(glGetUniformLocation(shad->Program, "material.diffuse"), 0);

	//We do the same as before but using another buffer and the second texture
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, TextSpec);
	glUniform1i(glGetUniformLocation(shad->Program, "material.specular"), 1);
}

void Material::SetShininess(Shader *shad) {
	GLuint materialShine = glGetUniformLocation(shad->Program, "material.shininess");
	glUniform1f(materialShine, Shininess);
}

void Material::ActivateTextures() {
	int width, height;
	unsigned char* loadedTexture;

	//Diffuse
	glGenTextures(1, &TextDiff); // We generate the actual texture
	glBindTexture(GL_TEXTURE_2D, TextDiff); // We bind it to the graphics card
											// Set the texture wrapping/filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load and generate the texture
	loadedTexture = SOIL_load_image(diffusePath, &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, loadedTexture); // We generate the texture
	SOIL_free_image_data(loadedTexture); // We free the image since we have stored it
	glBindTexture(GL_TEXTURE_2D, 0); // We bind the texture to the graphics card

	//Specular
	glGenTextures(1, &TextSpec); // We generate the actual texture
	glBindTexture(GL_TEXTURE_2D, TextSpec); // WE bind it to the graphics card
											// Set the texture wrapping/filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load and generate the texture
	loadedTexture = SOIL_load_image(specularPath, &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, loadedTexture); // We generate the texture
	SOIL_free_image_data(loadedTexture); // We free the image since we have stored it
	glBindTexture(GL_TEXTURE_2D, 0); // We bind the texture to the graphics card
}
