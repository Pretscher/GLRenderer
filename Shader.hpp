#pragma once
#include <memory>
#include <string>
//math includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
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
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setFloatArr(const std::string& name, int size, GLfloat value[]) const;
    void setVector(const std::string& name, int dimension, float values[]);
    //function not finished, only takes some possibilities as rows/cols
    void setVec2(const std::string& name, const glm::vec2& value) const;
    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setVec4(const std::string& name, const glm::vec4& value) const;
    void setMat2(const std::string& name, const glm::mat2& mat) const;
    void setMat3(const std::string& name, const glm::mat3& mat) const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;

    void setVec3Arr(const std::string& name, int size, float* start) const;
    void setVec4Arr(const std::string& name, int size, float* start) const;

    void setView(std::shared_ptr<glm::mat4> i_view);
    void setProjection(std::shared_ptr<glm::mat4> i_projection);
    glm::mat4  getProjection();
private:
    std::shared_ptr<glm::mat4> projectionMat;
    std::shared_ptr<glm::mat4> viewMat;
    void checkCompileErrors(unsigned int shader, std::string type);
};
