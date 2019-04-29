/**
 * Copyright (c) 2019 Levi van Rheenen - All rights reserved
 */

#include "Window.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Error.h"

Window::__GLFW Window::_glfw;

inline static GLFWwindow *getWindow(void *handle) {
	return static_cast<GLFWwindow *>(handle);
}

inline static GLFWwindow *getWindow(std::shared_ptr<void> handle) {
	return getWindow(handle.get());
}

Window::Window(EventListener *eventListener) :
		event_listener(eventListener),
		_closed(false) {

	// get the monitor for the game window
	GLFWmonitor *primaryMonitor = glfwGetPrimaryMonitor();

	// get the native resolution
	const GLFWvidmode *vidmode = glfwGetVideoMode(primaryMonitor);
	_width = vidmode->width * 0.75f;
	_height = vidmode->height * 0.75f;

	// set the window hints
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_SAMPLES, 9);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	// create the window
	GLFWwindow *window = glfwCreateWindow(_width, _height, "Stargem", nullptr, nullptr);
	if (!window) {
		throw showerror("Could not create window.");
	}

	glfwSetWindowPos(window, (int) (vidmode->width * 0.125f), (int) (vidmode->height * 0.125));

	// initialize the handle
	_window_handle = std::shared_ptr<void>(window, [](void *ptr) {
		glfwDestroyWindow(getWindow(ptr));
	});

	// install the event callbacks
	glfwSetWindowUserPointer(window, this);
	glfwSetCursorPosCallback(window, [](GLFWwindow *window, double x, double y) {
		Window *w = static_cast<Window *>(glfwGetWindowUserPointer(window));
		w->event_listener->OnMouseMove(float(x), float(y));
	});

	glfwSetMouseButtonCallback(window, [](GLFWwindow *window, int button, int action, int mods) {
		Window *w = static_cast<Window *>(glfwGetWindowUserPointer(window));
		w->event_listener->OnMouseButton(button, action, mods);
	});

	glfwSetKeyCallback(window, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
		Window *w = static_cast<Window *>(glfwGetWindowUserPointer(window));
		w->event_listener->OnKey(key, scancode, action, mods);
	});

	// initialize OpenGL
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	int error;
	if ((error = glewInit()) != GLEW_OK) {
		throw showerror("Failed to initialize GLEW: " + std::to_string(error));
	}

	glLineWidth(_width / 800.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Window::Show(const Renderer& renderer) {
	// get the window
	GLFWwindow *window = getWindow(_window_handle);

	// initialize time variables
	double lastTime = glfwGetTime(), time;
	float dt = 1.0 / 60.0;

	while (!glfwWindowShouldClose(window)) {
		// handle the input events
		glfwPollEvents();

		// clear the framebuffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// update the time delta
		time = glfwGetTime();
		dt = time - lastTime;
		lastTime = time;

		// render everything
		renderer.Render(dt);

		// swap the buffers
		glfwSwapBuffers(window);
	}

	_closed.store(true);
}

bool Window::HasClosed() const {
	return _closed.load();
}

Window::__GLFW::__GLFW() {
	// initialize GLFW
	if (!glfwInit()) {
		throw showerror("Failed to initialize GLFW.");
	}
}

Window::__GLFW::~__GLFW() {
	// terminate GLFW
	glfwTerminate();
}
