#pragma once
#include "Renderer.hpp"
#include "Camera.hpp"

class EventManager {
public:
	EventManager(shared_ptr<Renderer> io_renderer, shared_ptr<Camera> io_cam) {
		renderer = io_renderer;
		cam = io_cam;
		initEvents();
	}

	void eventloop();
private:
	void initEvents();
	shared_ptr<Renderer> renderer;
	shared_ptr<Camera> cam;

	void initRotatingLights();
	void drawCubeOfCubes();
	void updateRotatingLights();
	void drawLights();
	void initLights();
};