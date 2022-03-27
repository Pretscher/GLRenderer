#pragma once
#include <memory>
#include <string>
using namespace std;
//math includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;
#include "glad/glad.h"//include before GLFW
#include <GLFW/glfw3.h>
class Shader
{
public:
    // the program ID
    unsigned int ID;

    // constructor reads and builds the shader
    Shader(const char* vertexPath, const char* fragmentPath);
    // use/activate the shader
    void use();
    // utility uniform functions
    void setBool(const string& name, bool value) const;
    void setInt(const string& name, int value) const;
    void setFloat(const string& name, float value) const;
    void setFloatArr(const string& name, int size, GLfloat value[]) const;
    void setVector(const string& name, int dimension, float values[]);
    //function not finished, only takes some possibilities as rows/cols
    void setVec2(const string& name, const vec2& value) const;
    void setVec3(const string& name, const vec3& value) const;
    void setVec4(const string& name, const vec4& value) const;
    void setMat2(const string& name, const mat2& mat) const;
    void setMat3(const string& name, const mat3& mat) const;
    void setMat4(const string& name, const mat4& mat) const;

    void setVec3Arr(const string& name, int size, float* start) const;
    void setVec4Arr(const string& name, int size, float* start) const;

    void setView(shared_ptr<mat4> i_view);
    void setProjection(shared_ptr<mat4> i_projection);
    mat4  getProjection();
private:
    shared_ptr<mat4> projectionMat;
    shared_ptr<mat4> viewMat;
    void checkCompileErrors(unsigned int shader, string type);
};
