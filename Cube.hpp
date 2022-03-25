#pragma once
#include <vector>
#include "Shader.hpp"
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

	bool isDrawable() {
		return drawable;
	}
protected:
	bool drawable = true;
	glm::vec3 baseColor = glm::vec3(0.0f);
	float shininess = 32;
private:
	std::vector<unsigned int> textures;
	std::vector<unsigned int> specMaps;
	std::vector<float> texWeights;
	glm::mat4 tempModelMat, view, projection;
	std::shared_ptr<Shader> shader;

	float transX, transY, transZ;
	float scaleX, scaleY, scaleZ;
	float rotDegree, rotX, rotY, rotZ;
};