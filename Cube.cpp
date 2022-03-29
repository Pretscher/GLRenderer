#include "Cube.hpp"
#include <iostream>

//math libs through cube.hpp -> shader.hpp included

unsigned int cubeVAO, cubeVBO;
bool buffersInitialized = false;
void initBuffers() {
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);

    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);


    float cubeVerts[]{
        //coords                 //texture coords  //normal vectors 
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,       0.0f,  0.0f, -1.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 0.0f,       0.0f,  0.0f, -1.0f,
            0.5f, 0.5f, -0.5f,   1.0f, 1.0f,       0.0f,  0.0f, -1.0f,
            0.5f, 0.5f, -0.5f,   1.0f, 1.0f,       0.0f,  0.0f, -1.0f,
            -0.5f, 0.5f, -0.5f,  0.0f, 1.0f,       0.0f,  0.0f, -1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,       0.0f,  0.0f, -1.0f,

            -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,       0.0f,  0.0f, 1.0f,
            0.5f, -0.5f, 0.5f,   1.0f, 0.0f,       0.0f,  0.0f, 1.0f,
            0.5f, 0.5f, 0.5f,    1.0f, 1.0f,       0.0f,  0.0f, 1.0f,
            0.5f, 0.5f, 0.5f,    1.0f, 1.0f,       0.0f,  0.0f, 1.0f,
            -0.5f, 0.5f, 0.5f,   0.0f, 1.0f,       0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,       0.0f,  0.0f, 1.0f,

            -0.5f, 0.5f, 0.5f,   1.0f, 0.0f,       -1.0f,  0.0f,  0.0f,
            -0.5f, 0.5f, -0.5f,  1.0f, 1.0f,       -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,       -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,       -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,       -1.0f,  0.0f,  0.0f,
            -0.5f, 0.5f, 0.5f,   1.0f, 0.0f,       -1.0f,  0.0f,  0.0f,

            0.5f, 0.5f, 0.5f,    1.0f, 0.0f,       1.0f,  0.0f,  0.0f,
            0.5f, 0.5f, -0.5f,   1.0f, 1.0f,       1.0f,  0.0f,  0.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,       1.0f,  0.0f,  0.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,       1.0f,  0.0f,  0.0f,
            0.5f, -0.5f, 0.5f,   0.0f, 0.0f,       1.0f,  0.0f,  0.0f,
            0.5f, 0.5f, 0.5f,    1.0f, 0.0f,       1.0f,  0.0f,  0.0f,

            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,       0.0f, -1.0f,  0.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 1.0f,       0.0f, -1.0f,  0.0f,
            0.5f, -0.5f, 0.5f,   1.0f, 0.0f,       0.0f, -1.0f,  0.0f,
            0.5f, -0.5f, 0.5f,   1.0f, 0.0f,       0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,       0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,       0.0f, -1.0f,  0.0f,

            -0.5f, 0.5f, -0.5f,  0.0f, 1.0f,       0.0f,  1.0f,  0.0f,
            0.5f, 0.5f, -0.5f,   1.0f, 1.0f,       0.0f,  1.0f,  0.0f,
            0.5f, 0.5f, 0.5f,    1.0f, 0.0f,       0.0f,  1.0f,  0.0f,
            0.5f, 0.5f, 0.5f,    1.0f, 0.0f,       0.0f,  1.0f,  0.0f,
            -0.5f, 0.5f, 0.5f,   0.0f, 0.0f,       0.0f,  1.0f,  0.0f,
            -0.5f, 0.5f, -0.5f,  0.0f, 1.0f,       0.0f,  1.0f,  0.0f,
    };

    int vertexAttributes = 8;//3 pos, 2 texture, 3 normals
    int vertexCount = 36;

    //cube has 36 vertices
    glBufferData(GL_ARRAY_BUFFER, vertexAttributes * (vertexCount * sizeof(float)), cubeVerts, GL_STATIC_DRAW);

    // position attribute (layout = 0 in shader)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexAttributes * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexAttributes * sizeof(float), (void*)(5 * sizeof(float)));//offset 3
    glEnableVertexAttribArray(1);
    // texture coord attribute (layout = 2 in shader)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertexAttributes * sizeof(float), (void*)(3 * sizeof(float)));//offset 6
    glEnableVertexAttribArray(2);

    // there is no color attribute for this function (yet), so skip it
}

//if you dont want to pass a shader (thus not make the object drawable) pass nullptr
void Cube::commonInit(vector<unsigned int>& i_textures, vector<unsigned int>& i_specMaps, shared_ptr<Shader> i_shader) {
    if (buffersInitialized == false) {
        buffersInitialized = true;
        initBuffers();
    }
    shader = i_shader;
    this->textures = i_textures;
    this->specMaps = i_specMaps;

    tempModelMat = mat4(1.0f);
}

//if you dont want to pass a shader (thus not make the object drawable) pass nullptr
Cube::Cube(vector<unsigned int> i_textures, vector<unsigned int> i_specMaps, shared_ptr<Shader> i_shader) : Drawable(i_shader) {
    this->commonInit(i_textures, i_specMaps, i_shader);
}

void Cube::draw() {
    shader->use();
    //shader->setMat4("projection", projection);
    //textures
    shader->setInt("material.diffuseMapCount", textures.size());
    for (int i = 0; i < textures.size(); i++) {
        string index = "[" + to_string(i) + "]";
        shader->setInt("material.diffuse" + index, i);

        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, textures[i]);
    }
    shader->setInt("material.specMapCount", specMaps.size());
    //from the end index of textures array, add specular maps
    int startIndex = textures.size();
    for (int i = startIndex; i - startIndex < specMaps.size(); i++) {
        string index = "[" + to_string(i - startIndex) + "]";//index in material array has to count from 0, so i - startIndex
        this->shader->setInt("material.specular" + index, i);//index for shader should be unique, so i.
        glActiveTexture(GL_TEXTURE0 + i);//index for opengl should also be unique, so we use i
        glBindTexture(GL_TEXTURE_2D, specMaps[i - startIndex]);//index in specular array has to count from 0, so i - startIndex
    }

    //drawing
    glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}