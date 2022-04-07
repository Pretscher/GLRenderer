#include "Cube.hpp"
#include <iostream>

//math libs through cube.hpp -> shader.hpp included

unsigned int cubeVAO, cubeVBO;
bool buffersInitialized = false;
shared_ptr<vector<float>> cubeVerts;



void initBuffers() {
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);

    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);


    cubeVerts = shared_ptr<vector<float>>(new vector<float>({
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
    }));

    int vertexAttributes = 8;//3 pos, 2 texture, 3 normals
    int vertexCount = 36;

    //cube has 36 vertices
    glBufferData(GL_ARRAY_BUFFER, vertexAttributes * (vertexCount * sizeof(float)), cubeVerts->data(), GL_STATIC_DRAW);

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

shared_ptr<vector<float>> Cube::getCubeVertices() {
    if (cubeVerts == nullptr) {//this is a static method and can be called without the cubeverts being initialized.
        initBuffers();
    }
    return cubeVerts;
}

unsigned int Cube::getCubeVAO() {
    if (cubeVerts == nullptr) {//this is a static method and can be called without the cubeVAO being initialized.
        initBuffers();
    }
    return cubeVAO;
}

//if you dont want to pass a shader (thus not make the object drawable) pass nullptr
Cube::Cube(shared_ptr<Shader> i_shader) : Drawable(i_shader) {
    if (buffersInitialized == false) {
        buffersInitialized = true;
        initBuffers();
    }
}

Cube::Cube(vector<unsigned int> i_textures, vector<unsigned int> i_specMaps, shared_ptr<Shader> i_shader) : Drawable(i_shader, i_textures, i_specMaps) {
    if (buffersInitialized == false) {
        buffersInitialized = true;
        initBuffers();
    }
}

void Cube::draw(shared_ptr<Shader> shader) {
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
        shader->setInt("material.specular" + index, i);//index for shader should be unique, so i.
        glActiveTexture(GL_TEXTURE0 + i);//index for opengl should also be unique, so we use i
        glBindTexture(GL_TEXTURE_2D, specMaps[i - startIndex]);//index in specular array has to count from 0, so i - startIndex
    }

    //drawing
    glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}