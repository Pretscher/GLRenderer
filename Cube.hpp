#pragma once
#include <vector>
#include "Shader.hpp"
#include "Drawable.hpp"
class Cube : public Drawable {
public:
	Cube(vector<unsigned int> i_textures, vector<unsigned int> i_specMaps, shared_ptr<Shader> i_shader);
	void draw();
private:
	void commonInit(vector<unsigned int>& i_textures, vector<unsigned int>& i_specMaps, shared_ptr<Shader> i_shader);
};