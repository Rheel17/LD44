/**
 * Copyright (c) 2019 Levi van Rheenen - All rights reserved
 */

#include "ShaderProgram.h"

#include <exception>
#include <iostream>
#include <fstream>
#include <streambuf>

GLuint ShaderProgramBindings::programInUse = 0;

void ShaderProgram::Link() {
	// create the OpenGL program
	_program_id = GL::CreateProgram();

	// actually adds the shaders to the program
	std::vector<GLuint> shaders;

	for (const auto& addIterator : _shaders_to_add) {
		const auto& toAdd = addIterator.second;

		if (toAdd.first) { // toAdd.first is true if the shader is a file
			_AddShaderFromFile(addIterator.first, std::move(toAdd.second));
		} else { // else, it is the complete source
			_AddShaderFromSource(addIterator.first, std::move(toAdd.second));
		}
	}

	// remove the to-add shaders, since now they only take up memory
	_shaders_to_add.clear();

	// link the program
	glLinkProgram(_program_id);

	// check link status
	GLint linked;
	glGetProgramiv(_program_id, GL_LINK_STATUS, &linked);

	if (!linked) {
		// get compile log (errors)
		GLint logSize;
		glGetProgramiv(_program_id, GL_INFO_LOG_LENGTH, &logSize);

		GLchar log[logSize];
		glGetProgramInfoLog(_program_id, logSize, &logSize, log);

		// throw with compile errors
		throw std::runtime_error("Failed to link shader program: " + std::string(log));
	}

	// remove shaders
	for (const GLuint& shader : shaders) {
		glDetachShader(_program_id, shader);
		glDeleteShader(shader);
	}
}

GLuint ShaderProgram::_AddShaderFromSource(GLuint type, std::string source) {
	// create the shader object
	GLuint shader = glCreateShader(type);
	if (shader == 0) {
		throw std::runtime_error("Failed to create shader");
	}

	// add the shader source and compile
	const GLchar *str = source.c_str();
	const GLint len = source.size();

	glShaderSource(shader, 1, &str, &len);
	glCompileShader(shader);

	// check compile status
	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

	if (!compiled) {
		// get compile log (errors)
		GLint logSize;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);

		GLchar log[logSize];
		glGetShaderInfoLog(shader, logSize, &logSize, log);

		// throw with compile errors
		throw std::runtime_error("Failed to compile shader: " + std::string(log));
	}

	// add the shader to the program
	glAttachShader(_program_id, shader);
	return shader;
}

GLuint ShaderProgram::_AddShaderFromFile(GLuint type, std::string file) {
	std::ifstream f(file);
	std::string source((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());

	return _AddShaderFromSource(type, std::move(source));
}

void ShaderProgram::_EnsureUniformInstance(const std::string& name) const {
	if (_uniforms.find(name) == _uniforms.end()) {
		_uniforms.emplace(name, Uniform(_program_id, _GetUniformLocation(name)));
	}
}

GLint ShaderProgram::_GetUniformLocation(const std::string& name) const {
	GLint location = glGetUniformLocation(_program_id, name.c_str());

	if (location < 0) {
		std::cerr << "shader doesn't have uniform " << name << std::endl;
	}

	return location;
}
