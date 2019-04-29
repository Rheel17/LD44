/**
 * Copyright (c) 2019 Levi van Rheenen - All rights reserved
 */

#ifndef GL_H_
#define GL_H_

#include <GL/glew.h>
#include <memory>
#include <functional>
#include <cassert>
#include <iostream>

class GLhandle {
	// only the GL class can actually create handles.
	friend class GL;

	typedef std::function<void(GLsizei, GLuint *)> _Generator;
	typedef std::function<void(GLsizei, GLuint *)> _Deleter;

	GLhandle(_Generator generator, _Deleter deleter, bool debug) :
			_valid(true) {

		// create the OpenGL object
		GLuint *id = new GLuint;
		generator(1, id);

		if (debug) {
			std::cout << "Generated: " << *id << std::endl;
		}

		// create the handle, with a custom deleter when all instances
		// of this object go out of scope
		_handle = std::shared_ptr<GLuint>(id, [deleter, debug](GLuint *id) {
			if (debug) {
				std::cout << "Deleting: " << *id << std::endl;
			}

			deleter(1, id);
		});
	}

	bool _valid;
	std::shared_ptr<GLuint> _handle;

public:
	// default constructor to create a invalid GLhandle
	GLhandle() : _valid(false), _handle(nullptr) {}

	/*
	 * When the handle is valid, it returns the OpenGL id of the object. If
	 * the handle is not valid, it always returns 0.
	 */
	inline operator GLuint() const { return _valid ? *_handle : 0; }

	/*
	 * Returns whether the handle is valid.
	 */
	inline operator bool() const { return _valid; }

};

class GL {

	// Delete all the default con/de-structors, copy/assignment, etc.
private:
	GL() = delete;
	~GL() = delete;
	GL(const GL&) = delete;
	GL operator=(const GL&) = delete;

public:
	inline static GLhandle GenBuffer()       { return GLhandle(glGenBuffers,       glDeleteBuffers      , false); }
	inline static GLhandle GenFramebuffer()  { return GLhandle(glGenFramebuffers,  glDeleteFramebuffers , false); }
	inline static GLhandle GenRenderbuffer() { return GLhandle(glGenRenderbuffers, glDeleteRenderbuffers, false); }
	inline static GLhandle GenTexture()      { return GLhandle(glGenTextures,      glDeleteTextures     , false); }
	inline static GLhandle GenVertexArray()  { return GLhandle(glGenVertexArrays,  glDeleteVertexArrays , false); }

	inline static GLhandle CreateProgram() {
		return GLhandle([](GLsizei count, GLuint *ids) {
			for (GLsizei i = 0; i < count; i++) {
				ids[i] = glCreateProgram();
			}
		}, [](GLsizei count, GLuint *ids) {
			for (GLsizei i = 0; i < count; i++) {
				glDeleteProgram(ids[i]);
			}
		}, false);
	}

private:

};

#endif
