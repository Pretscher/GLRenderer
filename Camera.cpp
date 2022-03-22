#include "Camera.hpp"
#include "Renderer.hpp"
//has to be outside of class because of mousecallback, thus this class cannot be instantiated multiple times
bool firstMouse = true;
float cameraSpeed = 10.0f, cameraSensitivity = 0.05f; // adjust accordingly
float baseZoomSpeed = 100.0f, currentZoomSpeed;
float yaw = 90.0f, pitch = 0.0f;
float lastX = (float)Renderer::windowW / 2, lastY = (float)Renderer::windowH / 2;
glm::vec3 pos, cameraFront, up;

Camera::Camera(glm::vec3 startPos, glm::vec3 startCameraFront) {
	pos = startPos; cameraFront = startCameraFront; up = glm::vec3(0.0f, 1.0f, 0.0f);
	init();
}

Camera::Camera(glm::vec3 startPos, glm::vec3 startCameraFront, glm::vec3 startUp) {
	pos = startPos; cameraFront = startCameraFront; up = startUp;
	init();
}

float calculateAngle(glm::vec3 a, glm::vec3 b) {
	//cos^-1 ((a [dot product] b) / (|a| * |b|))
	float dotProduct = (a[0] * b[0]) + (a[1] * b[1]) + (a[2] * b[2]);
	float distanceProduct = sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]) * sqrt(b[0] * b[0] + b[1] * b[1] + b[2] * b[2]);
	float angleRad = acos((dotProduct / distanceProduct));
	return  glm::degrees(angleRad);
}

float fov = 45.0f;
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse == true) {
		lastX = (float)xpos;
		lastY = (float)ypos;
		//calculate initial jaw and pitch from cameraFront matrix that was passed through constructor
		yaw = calculateAngle(cameraFront, glm::vec3(1.0f, 0.0f, 0.0f));
		pitch = calculateAngle(cameraFront, glm::vec3(0.0f, -1.0f, 0.0f)) - 90.0f;
		firstMouse = false;
	}

	float xoffset = (float)xpos - lastX;
	float yoffset = lastY - (float)ypos;
	lastX = (float)xpos;
	lastY = (float)ypos;

	xoffset *= cameraSensitivity;
	yoffset *= cameraSensitivity;

	yaw += xoffset;
	pitch += yoffset;


	if (pitch >= 90.0f)
		pitch = 90.0f;
	if (pitch <= -90.0f)
		pitch = -90.0f;

	glm::vec3 direction(cos(glm::radians(yaw)) * cos(glm::radians(pitch)), sin(glm::radians(pitch)),
						sin(glm::radians(yaw)) * cos(glm::radians(pitch)));
	cameraFront = glm::normalize(direction);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	fov -= (float)yoffset * currentZoomSpeed;
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 45.0f)
		fov = 45.0f;
}


void Camera::update() {
	*viewMat = glm::lookAt(pos, pos + cameraFront, up);
	//fov set in scrollcallback func => ZOOM with crolling
	*projectionMat = glm::perspective(glm::radians(fov), (float)Renderer::windowW / Renderer::windowH, 0.1f, 200.0f);
}

std::shared_ptr<glm::mat4> viewMat, projectionMat;

void Camera::init() {
	viewMat = std::shared_ptr<glm::mat4>(new glm::mat4(glm::lookAt(pos, pos + cameraFront, up)));
	projectionMat = std::shared_ptr<glm::mat4>(new glm::mat4(glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f)));
}


void Camera::processInput(GLFWwindow* window, float deltaTime) {
	float currentCamSpeed = cameraSpeed * deltaTime;
	currentZoomSpeed = baseZoomSpeed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		pos += currentCamSpeed * cameraFront;//move to camera front
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		pos -= currentCamSpeed * cameraFront;//move away from camera front
	//cross product of front and up gives right vector, so move along right vector
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		//- rightvector => move left
		pos -= glm::normalize(glm::cross(cameraFront, up)) * currentCamSpeed;//normalize so we can keep the speed
	//+ rightvector => move right
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		pos += glm::normalize(glm::cross(cameraFront, up)) * currentCamSpeed;//normalize so we can keep the speed

	//as long as camera input is requested the mouse is not visible.
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);//TODO: move (no window passed to other functions)
	glfwSetScrollCallback(window, scroll_callback);
	update();
}

glm::vec3 Camera::getPos() {
	return pos;
}

glm::vec3 Camera::getDirection() {
	return cameraFront;
}

void Camera::setCameraSpeed(float i_camSpeed) {
	cameraSpeed = i_camSpeed;
}