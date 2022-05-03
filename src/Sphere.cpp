#include "Sphere.hpp"

#include "glad/glad.h"//include before GLFW
#include <GLFW/glfw3.h>

#include "Shader.hpp"
#include <iostream>
#include <vector>

bool wasInitialized = false;
unsigned int sphereVAO, sphereVBO;
int vertexCount;
Sphere::Sphere(std::vector<unsigned int> i_textures, std::vector<float> i_texWeights, std::shared_ptr<Shader> i_shader) {
    if (wasInitialized == false) {//init vertex array only once
        wasInitialized = true;
        this->init();
    }

    shader = i_shader;
    this->textures = i_textures;
    this->texWeights = i_texWeights;
    tempModelMat = glm::mat4(1.0f);
    view = glm::mat4(1.0f);

    shader->use();
    bool useTextures = true;
    if (textures.size() == 0) useTextures = false;
    shader->setBool("useTexture", useTextures);
    for (int i = 0; i < textures.size(); i++) {
        std::string num = std::to_string(i);
        shader->setInt("texture" + num, i);
        shader->setFloat("weight" + num, i_texWeights[i]);
    }
}

void Sphere::init() {
    // One vertex at every latitude-longitude intersection,
    // plus one for the north pole and one for the south.
    // One meridian serves as a UV seam, so we double the vertices there.
    vertexCount = numLatitudeLines * (numLongitudeLines + 1) + 2;
    glm::vec3* positions = new  glm::vec3[vertexCount];
    glm::vec2* texcoords = new  glm::vec2[vertexCount];

    // North pole.
    positions[0] = glm::vec3(0, radius, 0);
    texcoords[0] = glm::vec2(0, 1);

    // South pole.
    positions[vertexCount - 1] = glm::vec3(0, -radius, 0);
    texcoords[vertexCount - 1] = glm::vec2(0, 0);

    // +1.0f because there's a gap between the poles and the first parallel.
    float latitudeSpacing = 1.0f / (numLatitudeLines + 1.0f);
    float longitudeSpacing = 1.0f / (numLongitudeLines);

    // start writing new vertices at position 1
    int v = 1;
    for (int latitude = 0; latitude < numLatitudeLines; latitude++) {
        for (int longitude = 0; longitude <= numLongitudeLines; longitude++) {

            // Scale coordinates into the 0...1 texture coordinate range,
            // with north at the top (y = 1).
            texcoords[v] = glm::vec2(longitude * longitudeSpacing, 1.0f - (latitude + 1) * latitudeSpacing);

            // Convert to spherical coordinates:
            // theta is a longitude angle (around the equator) in radians.
            // phi is a latitude angle (north or south of the equator).
            float theta = texcoords[v].x * 2.0f * glm::pi<float>();
            float phi = (texcoords[v].y - 0.5f) * glm::pi<float>();

            // This determines the radius of the ring of this line of latitude.
            // It's widest at the equator, and narrows as phi increases/decreases.
            float c = cos(phi);

            // Usual formula for a vector in spherical coordinates.
            // You can exchange x & z to wind the opposite way around the sphere.
            positions[v] = glm::vec3(c * cos(theta), sin(phi), c * sin(theta)) * radius;

            // Proceed to the next vertex.
            v++;
        }
    }

    vertexData = new float[vertexCount * 5];
    for (int i = 0; i < vertexCount; i++) {
        int index = i * 5;
        vertexData[index] = positions[i].x;
        vertexData[index + 1] = positions[i].y;
        vertexData[index + 2] = positions[i].z;
        vertexData[index + 3] = texcoords[i].x;
        vertexData[index + 4] = texcoords[i].y;
    }
    delete[] positions; delete[] texcoords;

    glGenVertexArrays(1, &sphereVAO);
    glGenBuffers(1, &sphereVBO);

    glBindVertexArray(sphereVAO);

    glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);

    int vertexAttributes = 5;//3 coords, 2 tex coords (no color till now)

    //cube has 36 vertices
    glBufferData(GL_ARRAY_BUFFER, vertexAttributes * vertexCount * sizeof(float), vertexData, GL_STATIC_DRAW);

    // position attribute (layout = 0 in shader)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexAttributes * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // there is no color attribute (layout = 1 in shader) for this function (yet), so skip it
    // texture coord attribute (layout = 2 in shader)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertexAttributes * sizeof(float), (void*)(3 * sizeof(float)));//offset 3
    glEnableVertexAttribArray(2);

    //normal vectors
    //glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, vertexAttributes * sizeof(float), (void*)(5 * sizeof(float)));//offset 5
   // glEnableVertexAttribArray(3);
}

void Sphere::draw() {
    shader->use();
    /*//view and model matrix application
    float radius = 10.0f;
    float camX = static_cast<float>(sin(glfwGetTime()) * radius);
    float camZ = static_cast<float>(cos(glfwGetTime()) * radius);
   // view = glm::lookAt(glm::vec3(camX, 0.0f, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    shader->setMat4("view", view);

    model = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));//set to coordinates (not move from previous coordinates)
    shader->setMat4("model", model);
    */

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
        else if (i == 5) {
            glActiveTexture(GL_TEXTURE5);
        }
        glBindTexture(GL_TEXTURE_2D, textures[i]);
    }
    //drawing
    glBindVertexArray(sphereVAO);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

void Sphere::rotate(float degree, float x, float y, float z) {
    rotDegree = degree;  rotX = x; rotY = y; rotZ = z;

}

void Sphere::translate(float x, float y, float z) {
    transX = x; transY = y; transZ = z;
    position[0] = x; position[1] = y; position[2] = z;
}

void Sphere::scale(float x, float y, float z) {
    scaleX = x; scaleY = y; scaleZ = z;
}
