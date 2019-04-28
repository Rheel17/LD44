/**
 * Copyright (c) 2019 Levi van Rheenen - All rights reserved
 */

#ifndef WINDOW_H_
#define WINDOW_H_

#include <atomic>
#include <memory>

#include "Renderer.h"

class Window {

public:
	class EventListener {

	public:
		virtual ~EventListener() = default;

		virtual void OnMouseMove(float x, float y) {}
		virtual void OnMouseButton(int button, int action, int mods) {}
		virtual void OnKey(int key, int scancode, int action, int mods) {}

	};

public:
	Window(EventListener *eventListener);

	void Show(const Renderer& renderer);
	bool HasClosed() const;

	unsigned Width() const { return _width; }
	unsigned Height() const { return _height; }

	// needed public because GLFW expects actual function pointers,
	// so capturing this, or anything really, is impossible.
	EventListener *event_listener;

private:
	unsigned _width;
	unsigned _height;

	std::atomic_bool _closed;

	// type void* to avoid having to include the GLFW headers
	// in this header, which would make Game.h depend on it as
	// well.
	std::shared_ptr<void> _window_handle = nullptr;

	// GLFW init/terminate class
	class __GLFW {
		friend class Window;

		// to initialize GLFW
		__GLFW();

		// to terminate GLFW
		~__GLFW();
	};

	static __GLFW _glfw;

};

#endif
