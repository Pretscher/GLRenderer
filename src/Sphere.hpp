#pragma once
#include "Shader.hpp"
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Sphere {
public:
    Sphere(std::vector<unsigned int> i_textures, std::vector<float> i_texWeights, std::shared_ptr<Shader> i_shader);
    void init();
    void draw();

    void rotate(float degree, float x, float y, float z);
    void translate(float x, float y, float z);
    void scale(float x, float y, float z);
    glm::vec3 position;
private:
    std::vector<unsigned int> textures;
    std::vector<float> texWeights;
    glm::mat4 tempModelMat, view, projection;
    std::shared_ptr<Shader> shader;
    float numLatitudeLines = 500, numLongitudeLines = 500;
    float radius = 5.0f;
    float* vertexData;

    float transX, transY, transZ;
    float scaleX, scaleY, scaleZ;
    float rotDegree, rotX, rotY, rotZ;
};