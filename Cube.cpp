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
    // there is no color attribute (layout = 1 in shader) for this function (yet), so skip it
    // texture coord attribute (layout = 2 in shader)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertexAttributes * sizeof(float), (void*)(3 * sizeof(float)));//offset 3
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, vertexAttributes * sizeof(float), (void*)(5 * sizeof(float)));//offset 5
    glEnableVertexAttribArray(3);

}

//if you dont want to pass a shader (thus not make the object drawable) pass nullptr
void Cube::commonInit(std::vector<unsigned int>& i_textures, std::vector<float>& i_texWeights, std::shared_ptr<Shader> i_shader) {
    if (buffersInitialized == false) {
        buffersInitialized = true;
        initBuffers();
    }
    shader = i_shader;
    this->textures = i_textures;
    this->texWeights = i_texWeights;
    tempModelMat = glm::mat4(1.0f);
    view = glm::mat4(1.0f);
    if (shader != nullptr) {//if you dont want to pass a shader (thus not make the object drawable) pass nullptr
        shader->use();
        for (int i = 0; i < textures.size(); i++) {
            std::string num = std::to_string(i);
            shader->setInt("texture" + num, i);
            shader->setFloat("weight" + num, i_texWeights[i]);
        }
        if (i_textures.size() > 0) {
            this->shader->setInt("material.diffuse", 0);
        }
    }
}

//if you dont want to pass a shader (thus not make the object drawable) pass nullptr
Cube::Cube(std::vector<unsigned int> i_textures, std::vector<float> i_texWeights, std::shared_ptr<Shader> i_shader) {
    this->commonInit(i_textures, i_texWeights, i_shader);
}
//if you dont want to pass a shader (thus not make the object drawable) pass nullptr
Cube::Cube(std::vector<unsigned int> i_textures, unsigned int i_specularMap, std::vector<float> i_texWeights, std::shared_ptr<Shader> i_shader) {
    this->commonInit(i_textures, i_texWeights, i_shader);
    this->specularMap = i_specularMap;
}

void Cube::draw() {
    if (drawable == false) {
        std::cout << "Error in Light drawing function: Light has been initialized without a shader."
                  << "To make the light drawable, init with the constructor that has a shader input variable.";
        std::exit(0);
    }
	shader->use();
    updateMaterial();

    //model matrix
    glm::mat4 model(1.0f);
    //first scale
    model[0][0] = scaleX;
    model[1][1] = scaleY;
    model[2][2] = scaleZ;
    scaleX = 1.0f; scaleY = 1.0f; scaleZ = 1.0f;
    //then rotate (if you translate before that it will rotate in a circle around the origin from its coordinates)
    if (rotDegree != 0.0f) {
        model = glm::rotate(model, glm::radians(rotDegree), glm::vec3(rotX, rotY, rotZ));//rotate
        rotDegree = 0.0f; rotX = 0.0f; rotY = 0.0f; rotZ = 0.0f;
    }
    //then translate
    model[3][0] = transX;
    model[3][1] = transY;
    model[3][2] = transZ;
    transX = 0.0f; transY = 0.0f; transZ = 0.0f;

    shader->setMat4("model", model);
    // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
    tempModelMat = model;

    //shader->setMat4("projection", projection);
    //textures
    for (int i = 0; i < textures.size(); i++) {
        if (i == 0) {
            glActiveTexture(GL_TEXTURE0);
        }
        else if (i == 1) {
            glActiveTexture(GL_TEXTURE1);
        }
        else if (i == 2) {
            glActiveTexture(GL_TEXTURE2);
        }
        else if (i == 3) {
            glActiveTexture(GL_TEXTURE3);
        }
        else if (i == 4) {
            glActiveTexture(GL_TEXTURE4);
        }
        glBindTexture(GL_TEXTURE_2D, textures[i]);
    }
    //this texture is just bound to index 5 for now
    this->shader->setInt("material.specular", 5);
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, this->specularMap);


    //drawing
    glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Cube::updateMaterial() {
    material.setShaderUniforms(shader.get());
}

//set rotation from last rotation state, reusing the current model matrix
void Cube::rotate(float degree, float x, float y, float z) {
    rotDegree = degree;  rotX = x; rotY = y; rotZ = z;

}

//transalte from last coords, reusing the current model matrix
void Cube::translate(float x, float y, float z) {
    transX = x; transY = y; transZ = z;
    position[0] = x; position[1] = y; position[2] = z;
}

void Cube::scale(float x, float y, float z) {
    scaleX = x; scaleY = y; scaleZ = z;
}

void Cube::printModelMatrix() {
    for (int i = 0; i < 4; i++) {
        std::cout << "\nrow " << i << ": ";
        for (int j = 0; j < 4; j++) {
            std::cout << tempModelMat[i][j] << ", ";
        }
    }
}