#pragma once
#include <vector>
#include "Shader.hpp"
#include "Drawable.hpp"
class Cube : public Drawable {
public:
	Cube(std::vector<unsigned int> i_textures, std::vector<unsigned int> i_specMaps, std::shared_ptr<Shader> i_shader);
	void commonInit(std::vector<unsigned int>& i_textures, std::vector<unsigned int>& i_specMaps, std::shared_ptr<Shader> i_shader);
	
	void draw();
	//rotates the axes by a degree. Input 1 for x, y or z for full rotation (based on degree input), 0 for no rotation,
	//can be something in between aswell
	void rotate(float degree, float x, float y, float z);
	void translate(float x, float y, float z);
	void scale(float x, float y, float z);
	void printModelMatrix();

	glm::vec3 position;
protected:
	glm::vec3 baseColor = glm::vec3(0.0f);
	float shininess = 32;
};