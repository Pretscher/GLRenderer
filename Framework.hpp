#pragma once
#include <iostream>
#include <vector>
#include "Renderer.hpp"
#include "Cube.hpp"
#include "Camera.hpp"
#include "Light.hpp"
#include <cstdlib>//random numbers

class FrameWork {
public:
    FrameWork() {
        renderer = new Renderer();//inits opengl aswell
        cam = new Camera(glm::vec3(0.0f, 0.0f, -20.0f), glm::vec3(5.0f, 5.0f, 20.0f));
        //viewmatrix of camera will be automatically applied to shaders passed to the renderer
        renderer->bindViewMatrix(cam->getViewMatrix());
        //projectionmatrix of camera will be automatically applied to shaders passed to the renderer
        renderer->bindProjectionMatrix(cam->getProjectionMatrix());
	}

    Renderer* renderer;
    Camera* cam;

    void startDrawingLoop(void(*callback)()) {
        while (!glfwWindowShouldClose(renderer->window))
        {
            closeOnEscape(renderer->window);//close on escape
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//clear color buffer and depth buffer
            //draw here------------------------------------------------------------------------------------------
            updateDeltaTime();
            cameraManagement();
            callback();
            //\draw here------------------------------------------------------------------------------------------
            glfwSwapBuffers(renderer->window);//DOUBLE BUFFERING
            glfwPollEvents();//resizing, minimizing etc.
        }
        glfwTerminate();
    }

    float getTimeSinceLastFrame() {
        return deltaTime;
    }
private:


    //not really needed because we move the camera manually, but good to know
    void rotateCamAroundOrigin(float radius, float speed, float x, float y, float z) {
        float camX = (float)sin((float)glfwGetTime() * speed) * radius * x;
        float camY = (float)cos((float)glfwGetTime() * speed) * radius * y;
        float camZ = (float)cos((float)glfwGetTime() * speed) * radius * z;
        renderer->setViewMatrix(glm::vec3(camX, camY, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0f));
    }


    void closeOnEscape(GLFWwindow* window) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }
    }

    void cameraManagement() {
        cam->processInput(renderer->window, deltaTime);
    }

    float deltaTime = 0.0f;	// Time between current frame and last frame
    float lastFrame = 0.0f; // Time of last frame
    void updateDeltaTime() {
        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
    }
};