#pragma once

#include <iostream>
#include <vector>
using namespace std;
#include "Shader.hpp"
#include "Cube.hpp"
#include "Light.hpp"

//used by OpenGL if window is resized, not in class because function pointers in classes are tricky

class Renderer {
public:
    GLFWwindow* window;
    static int windowW, windowH;
    //init glfw, glad and window
    Renderer();

    void setBackgroundColor(float r, float g, float b, float a);
    static unsigned int loadTexture(string path, bool flipVertically, bool alphaValue);

    shared_ptr<Shader> createShader(string vertexPath, string fragmentPath, bool perspective, bool updateAutomatically);
    shared_ptr<Shader> getDefaultShader();

    void updateLights(vector<PointLight>& pLights, vector<DirectionalLight>& dLights, vector<SpotLight>& sLights, vec3 camPos) {
        for (int shaderI = 0; shaderI < shaders.size(); shaderI++) {
            for (int i = 0; i < pLights.size(); i ++) {
                pLights[i].setShaderLightObject(shaders[shaderI], i);
            }
            for (int i = 0; i < dLights.size(); i ++) {
                dLights[i].setShaderLightObject(shaders[shaderI], i);
            }
            for (int i = 0; i < sLights.size(); i ++) {
                sLights[i].setShaderLightObject(shaders[shaderI], i);
            }
            shaders[shaderI]->use();
            shaders[shaderI]->setVec3("viewPos", camPos);
        }
    }
    //Set view matrix for all shaders created by renderer
    void setViewMatrix(vec3 camPos, vec3 camLookAt, vec3 up) {
        *globalView = mat4(glm::lookAt(camPos, camLookAt, up));
    }

    void bindViewMatrix(shared_ptr<mat4> i_viewMat) {
        globalView = i_viewMat;
        //now globalview is a different pointer, so shader-viewmatrices with old pointer wont be updated
        for (int i = 0; i < shaders.size(); i++) {
            shaders[i]->setView(globalView);
        }
    }

    //Set projection matrix for all shaders created by renderer
    void setProjectionMatrix(float FOVDegree, float minVisibleDist, float maxVisibleDist, bool perspective) {
        if (perspective == true) {
            *globalProjection = glm::perspective(glm::radians(FOVDegree), (float)Renderer::windowW / (float)Renderer::windowH, minVisibleDist, maxVisibleDist);
        } else {
            *globalProjection = glm::ortho(glm::radians(FOVDegree), (float)Renderer::windowW / (float)Renderer::windowH, minVisibleDist, maxVisibleDist);
        }
    }

    void bindProjectionMatrix(shared_ptr<mat4> i_projectionMat) {
        globalProjection = i_projectionMat;
        for (int i = 0; i < shaders.size(); i++) {
            shaders[i]->setProjection(globalProjection);
        }
    }

    void setDefaultShader(shared_ptr<Shader> shader) {
        defaultShader = shader;
    }
    
    unsigned int loadCubeMap(vector<string> faces);
    shared_ptr<mat4> globalView, globalProjection;
private:
    vector< shared_ptr<Shader>> shaders;//used to change the view matrix for all shaders passed in functions
    shared_ptr<Shader> defaultShader;
    void initDrawingStuff() {
        setBackgroundColor(0.0f, 0.0f, 0.0f, 1.0f);
        
        globalView = shared_ptr<mat4>(new mat4());
        setViewMatrix(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -10.0f), vec3(0.0f, 0.0f, 0.0f));
        globalProjection = shared_ptr<mat4>(new mat4());
        setProjectionMatrix(45.0f, 0.1f, 100.0f, true);

        defaultShader = createShader("VertexShader.vert", "FragmentShader.frag", true, true);
        shaders.push_back(defaultShader);
    }
};