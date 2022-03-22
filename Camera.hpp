#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glad/glad.h"//include before GLFW
#include <GLFW/glfw3.h>
#include <memory>

class Camera {
public:
	Camera(glm::vec3 startPos, glm::vec3 startCameraFront);
	//if you want to customize up vector at the start
	Camera(glm::vec3 startPos, glm::vec3 startCameraFront, glm::vec3 startUp);
	void processInput(GLFWwindow* window, float deltaTime);

	void setCameraSpeed(float i_camSpeed);
	glm::vec3 getPos();
	glm::vec3 getDirection();
	std::shared_ptr<glm::mat4> getViewMatrix() {
		return viewMat;
	}
	std::shared_ptr<glm::mat4> getProjectionMatrix() {
		return projectionMat;
	}


private:
	std::shared_ptr<glm::mat4> viewMat, projectionMat;
	void init();
	void update();


};