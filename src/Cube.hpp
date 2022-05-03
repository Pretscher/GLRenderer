#pragma once
#include <vector>
using namespace std;
#include "Shader.hpp"
#include "Drawable.hpp"

class Cube : public Drawable {
public:
	Cube(shared_ptr<Shader> i_shader);
	Cube(vector<unsigned int> i_textures, vector<unsigned int> i_specMaps, shared_ptr<Shader> i_shader);
	void draw(shared_ptr<Shader> shader);

	static shared_ptr<vector<float>> getCubeVertices();
	static unsigned int getCubeVAO();
};