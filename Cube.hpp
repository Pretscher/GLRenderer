#pragma once
#include <vector>
#include "Material.hpp"//includes shader

class Cube {
public:
	Cube(std::vector<unsigned int> i_textures, std::vector<unsigned int> i_specMaps, std::vector<float> i_texWeights, std::shared_ptr<Shader> i_shader);
	void commonInit(std::vector<unsigned int>& i_textures, std::vector<unsigned int>& i_specMaps, std::vector<float>& i_texWeights, std::shared_ptr<Shader> i_shader);
	
	void draw();
	//rotates the axes by a degree. Input 1 for x, y or z for full rotation (based on degree input), 0 for no rotation,
	//can be something in between aswell
	void rotate(float degree, float x, float y, float z);
	void translate(float x, float y, float z);
	void scale(float x, float y, float z);
	void printModelMatrix();

	glm::vec3 position;

	void setMaterial(std::vector<float>  diffuseColor, std::vector<float> specularColor, float specularShininess) {
		//if (ambientColor.size() != 3 || diffuseColor.size() != 3 || specularColor.size() != 3) {
			//std::cout << "vector input for setMaterial function is only for easy and quick initialization"
	//	}
		material = Material(
			glm::vec3(diffuseColor[0], diffuseColor[1], diffuseColor[2]),
			glm::vec3(specularColor[0], specularColor[1], specularColor[1]),
			specularShininess);
	}

	bool isDrawable() {
		return drawable;
	}
protected:
	bool drawable = true;

private:
	Material material;//default material is assigned, see above class for default constructur
	void updateMaterial();
	std::vector<unsigned int> textures;
	std::vector<unsigned int> specMaps;
	std::vector<float> texWeights;
	glm::mat4 tempModelMat, view, projection;
	std::shared_ptr<Shader> shader;

	float transX, transY, transZ;
	float scaleX, scaleY, scaleZ;
	float rotDegree, rotX, rotY, rotZ;
};