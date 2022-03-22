#pragma once
#include "Shader.hpp"

class Material {
public:
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
	Material() {
		diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
		specular = glm::vec3(1.0f, 1.0f, 1.0f);
		shininess = 32;
	}
	Material(glm::vec3 diffuseColor, glm::vec3 specularColor, float specularShininess) {
		diffuse = diffuseColor;
		specular = specularColor;
		shininess = specularShininess;
	}

	void setShaderUniforms(const Shader* shader) {
		shader->setVec3("material.diffuse", this->diffuse);
		shader->setVec3("material.specular", this->specular);
		shader->setFloat("material.shininess", this->shininess);
	}
};