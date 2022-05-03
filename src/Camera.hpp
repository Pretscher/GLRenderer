#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;
#include "glad/glad.h"//include before GLFW
#include <GLFW/glfw3.h>
#include <memory>
using namespace std;
class Camera {
public:
	Camera(vec3 startPos, vec3 startCameraFront);
	//if you want to customize up vector at the start
	Camera(vec3 startPos, vec3 startCameraFront, vec3 startUp);
	void processInput(GLFWwindow* window, float deltaTime);

	void setCameraSpeed(float i_camSpeed);
	vec3 getPos();
	vec3 getDirection();
	shared_ptr<mat4> getViewMatrix() {
		return viewMat;
	}
	shared_ptr<mat4> getProjectionMatrix() {
		return projectionMat;
	}


private:
	shared_ptr<mat4> viewMat, projectionMat;
	void init();
	void update();


};