#pragma once
#include <vector>
#include "Shader.hpp"
#include "Drawable.hpp"
class Cube : public Drawable {
public:
	Cube(std::vector<unsigned int> i_textures, std::vector<unsigned int> i_specMaps, std::shared_ptr<Shader> i_shader);
	void draw();
private:
	void commonInit(std::vector<unsigned int>& i_textures, std::vector<unsigned int>& i_specMaps, std::shared_ptr<Shader> i_shader);
};