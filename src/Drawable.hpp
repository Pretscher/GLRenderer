#pragma once
#include "Shader.hpp"

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
    void updateAndDraw();

    vec3 getPosition() {
        return position;
    }

    void setBaseColor(vec3 i_baseColor) {
        baseColor = i_baseColor;
    }
    
    void setAffectedByLighting(bool affected) {
        affectedByLighting = affected;
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
        scaleX *= x; scaleY *= y; scaleZ *= z;
    }
    
    void enableOutline(float width, vec4 color, shared_ptr<Shader> i_singleColorShader) {
        drawOutline = true;
        outlineWidth = width;
        outlineColor = color;
        singleColorShader = i_singleColorShader;
    }


    bool dontDraw = false;
    bool faceCulling = true;
protected:
    Drawable(shared_ptr<Shader> i_shader) {
        defaultShader = i_shader;
        specularSensitivity = 1.0f;
        diffuseSensitivity = 1.0f;
        shininess = 64;
    }

    Drawable(shared_ptr<Shader> i_shader, vector<unsigned int> i_textures, vector<unsigned int> i_specMaps) {
        defaultShader = i_shader;
        textures = i_textures;
        specMaps = i_specMaps;
        specularSensitivity = 1.0f;
        diffuseSensitivity = 1.0f;
        shininess = 64;
    }

    virtual void draw(shared_ptr<Shader> shader) = 0;//should only be called from updateAndDraw() func


    //tempModelMat is only used for printing


    vec3 position = vec3(0.0f);
    vector<unsigned int> textures;
    vector<unsigned int> specMaps;

private:
    //temporary transformation data, reset after drawing
    float transX, transY, transZ;
    float scaleX, scaleY, scaleZ;
    vector<Rotation> rotations;

    //lighting data set in update()
    float specularSensitivity, diffuseSensitivity, shininess;
    vec3 baseColor = vec3(0.0f);//default base color is black
    bool affectedByLighting = true;

    void update(shared_ptr<Shader> shader);
    //Call before drawing shape
    void applyTransformations(shared_ptr<Shader> shader);


    float outlineWidth = 0.0f;
    bool drawOutline = false;
    vec4 outlineColor;
    shared_ptr<Shader> singleColorShader;
    void drawColoredOutline();

    mat4 tempModelMat = mat4(1.0f), projection;
    shared_ptr<Shader> defaultShader;
};