#pragma once
#include "Shader.hpp"
#include <iostream>
#include <vector>

struct Rotation {
    Rotation(float i_angle, float i_x, float i_y, float i_z) {
        angle = glm::radians(i_angle);
        x = i_x;
        y = i_y;
        z = i_z;
    }
    float x, y, z;
    float angle;
};

class Drawable {
public:
    void updateAndDraw() {
        applyTransformations();//managed by drawable
        if (dontDraw == false) {
            draw();
        }
    }

    vec3 getPosition() {
        return position;
    }

    void setBaseColor(vec3 i_baseColor) {
        baseColor = i_baseColor;
    }

    void setLightingSensitivity(float i_diffuse, float i_specular) {
        diffuseSensitivity = i_diffuse;
        specularSensitivity = i_specular;
    }

    void setShininess(float i_shininess) {
        shininess = i_shininess;
    }

    //set rotation from last rotation state, reusing the current model matrix
    void rotate(float degree, float x, float y, float z) {
        rotations.push_back(Rotation(degree, x, y, z));
    }

    //transalte from last coords, reusing the current model matrix
    void translate(float x, float y, float z) {
        transX += x; transY += y; transZ += z;
        position = { transX, transY, transZ };
    }
    
    //transalte from last coords, reusing the current model matrix
    void translate(vec3 newPos) {
        transX += newPos.x; transY += newPos.y; transZ += newPos.z;
        position = { transX, transY, transZ };
    }

    void scale(float x, float y, float z) {
        scaleX = x; scaleY = y; scaleZ = z;
    }

    void printModelMatrix() {
        for (int i = 0; i < 4; i++) {
            cout << "\nrow " << i << ": ";
            for (int j = 0; j < 4; j++) {
                cout << tempModelMat[i][j] << ", ";
            }
        }
    }

    bool influencedByLighting = true;
    bool dontDraw = false;
protected:
    float specularSensitivity, diffuseSensitivity, shininess;

    vec3 baseColor = vec3(0.0f);//default base color is black
    Drawable(shared_ptr<Shader> i_shader) {
        shader = i_shader;
        specularSensitivity = 1.0f;
        diffuseSensitivity = 1.0f;
        shininess = 64;
    }
    virtual void draw() = 0;//should only be called from updateAndDraw() func

    void applyTransformations() {
        shader->use();
        //THOSE ARE NOT TRANSFORMATIONS, but here for convenience ;)
        shader->setVec3("baseColor", baseColor);//if 0.0 (default) nothing happens.
        shader->setBool("influencedByLighting", influencedByLighting);

        shader->setFloat("materialDissuseSensitivity", diffuseSensitivity);
        shader->setFloat("materialSpecularSensitivity", specularSensitivity);
        shader->setFloat("materialShininess", shininess);
        //\THOSE ARE NOT TRANSFORMATIONS
        
        //model matrix
        mat4 model(1.0f);
        //first scale
        model[0][0] = scaleX;
        model[1][1] = scaleY;
        model[2][2] = scaleZ;
        scaleX = 1.0f; scaleY = 1.0f; scaleZ = 1.0f;
        //then rotate (if you translate before that it will rotate in a circle around the origin from its coordinates)
        for(int i = 0; i < rotations.size(); i++) {
            model = glm::rotate(model, rotations[i].angle, vec3(rotations[i].x, rotations[i].y, rotations[i].z));//rotate
        }
        rotations.clear();
        //then translate
        model[3][0] = transX;
        model[3][1] = transY;
        model[3][2] = transZ;
        transX = 0.0f; transY = 0.0f; transZ = 0.0f;
        shader->setMat4("model", model);
    }

    //tempModelMat is only used for printing
    mat4 tempModelMat = mat4(1.0f), projection;
    shared_ptr<Shader> shader;

    float transX, transY, transZ;
    float scaleX, scaleY, scaleZ;
    vector<Rotation> rotations;

    vec3 position = vec3(0.0f);

    vector<unsigned int> textures;
    vector<unsigned int> specMaps;
};