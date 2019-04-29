/**
 * Copyright (c) 2019 Levi van Rheenen - All rights reserved
 */

#ifndef SHADERPROGRAM_H_
#define SHADERPROGRAM_H_

#include "GL.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

#include <glm/glm.hpp>

class ShaderProgramBindings {
	friend class Uniform;
	friend class ShaderProgram;

	ShaderProgramBindings() = delete;

private:
	inline static void Use(GLuint program) {
		if (programInUse != program) {
			glUseProgram(program);
			programInUse = program;
		}
	}

	static GLuint programInUse;

};

class Uniform {

public:
	Uniform(GLuint program = 0, GLuint location = -1) : _program(program), _location(location) {}

	inline void operator=(GLfloat value)          { _Use(); glUniform1f(_location, value); }
	inline void operator=(const glm::vec2& value) { _Use(); glUniform2f(_location, value.x, value.y); }
	inline void operator=(const glm::vec3& value) { _Use(); glUniform3f(_location, value.x, value.y, value.z); }
	inline void operator=(const glm::vec4& value) { _Use(); glUniform4f(_location, value.x, value.y, value.z, value.w); }

	inline void operator=(GLint value)             { _Use(); glUniform1i(_location, value); }
	inline void operator=(const glm::ivec2& value) { _Use(); glUniform2i(_location, value.x, value.y); }
	inline void operator=(const glm::ivec3& value) { _Use(); glUniform3i(_location, value.x, value.y, value.z); }
	inline void operator=(const glm::ivec4& value) { _Use(); glUniform4i(_location, value.x, value.y, value.z, value.w); }

	inline void operator=(GLuint value)            { _Use(); glUniform1ui(_location, value); }
	inline void operator=(const glm::uvec2& value) { _Use(); glUniform2ui(_location, value.x, value.y); }
	inline void operator=(const glm::uvec3& value) { _Use(); glUniform3ui(_location, value.x, value.y, value.z); }
	inline void operator=(const glm::uvec4& value) { _Use(); glUniform4ui(_location, value.x, value.y, value.z, value.w); }

	inline void operator=(const std::vector<GLfloat>& values)   { _Use(); glUniform1fv(_location, values.size(), values.data()); }
	inline void operator=(const std::vector<glm::vec2>& values) { _Use(); glUniform2fv(_location, values.size(), reinterpret_cast<const GLfloat *>(values.data())); }
	inline void operator=(const std::vector<glm::vec3>& values) { _Use(); glUniform3fv(_location, values.size(), reinterpret_cast<const GLfloat *>(values.data())); }
	inline void operator=(const std::vector<glm::vec4>& values) { _Use(); glUniform4fv(_location, values.size(), reinterpret_cast<const GLfloat *>(values.data())); }

	inline void operator=(const std::vector<GLint>& values)      { _Use(); glUniform1iv(_location, values.size(), values.data()); }
	inline void operator=(const std::vector<glm::ivec2>& values) { _Use(); glUniform2iv(_location, values.size(), reinterpret_cast<const GLint *>(values.data())); }
	inline void operator=(const std::vector<glm::ivec3>& values) { _Use(); glUniform3iv(_location, values.size(), reinterpret_cast<const GLint *>(values.data())); }
	inline void operator=(const std::vector<glm::ivec4>& values) { _Use(); glUniform4iv(_location, values.size(), reinterpret_cast<const GLint *>(values.data())); }

	inline void operator=(const std::vector<GLuint>& values)     { _Use(); glUniform1uiv(_location, values.size(), values.data()); }
	inline void operator=(const std::vector<glm::uvec2>& values) { _Use(); glUniform2uiv(_location, values.size(), reinterpret_cast<const GLuint *>(values.data())); }
	inline void operator=(const std::vector<glm::uvec3>& values) { _Use(); glUniform3uiv(_location, values.size(), reinterpret_cast<const GLuint *>(values.data())); }
	inline void operator=(const std::vector<glm::uvec4>& values) { _Use(); glUniform4uiv(_location, values.size(), reinterpret_cast<const GLuint *>(values.data())); }

	inline void operator=(const glm::mat2& value) { _Use(); glUniformMatrix2fv(_location, 1, GL_FALSE, reinterpret_cast<const GLfloat *>(&value)); }
	inline void operator=(const glm::mat3& value) { _Use(); glUniformMatrix3fv(_location, 1, GL_FALSE, reinterpret_cast<const GLfloat *>(&value)); }
	inline void operator=(const glm::mat4& value) { _Use(); glUniformMatrix4fv(_location, 1, GL_FALSE, reinterpret_cast<const GLfloat *>(&value)); }
	inline void operator=(const glm::mat2x3& value) { _Use(); glUniformMatrix2x3fv(_location, 1, GL_FALSE, reinterpret_cast<const GLfloat *>(&value)); }
	inline void operator=(const glm::mat3x2& value) { _Use(); glUniformMatrix3x2fv(_location, 1, GL_FALSE, reinterpret_cast<const GLfloat *>(&value)); }
	inline void operator=(const glm::mat2x4& value) { _Use(); glUniformMatrix2x4fv(_location, 1, GL_FALSE, reinterpret_cast<const GLfloat *>(&value)); }
	inline void operator=(const glm::mat4x2& value) { _Use(); glUniformMatrix4x2fv(_location, 1, GL_FALSE, reinterpret_cast<const GLfloat *>(&value)); }
	inline void operator=(const glm::mat3x4& value) { _Use(); glUniformMatrix3x4fv(_location, 1, GL_FALSE, reinterpret_cast<const GLfloat *>(&value)); }
	inline void operator=(const glm::mat4x3& value) { _Use(); glUniformMatrix4x3fv(_location, 1, GL_FALSE, reinterpret_cast<const GLfloat *>(&value)); }

	inline void operator=(const std::vector<glm::mat2>& value) { _Use(); glUniformMatrix2fv(_location, value.size(), GL_FALSE, reinterpret_cast<const GLfloat *>(value.data())); }
	inline void operator=(const std::vector<glm::mat3>& value) { _Use(); glUniformMatrix3fv(_location, value.size(), GL_FALSE, reinterpret_cast<const GLfloat *>(value.data())); }
	inline void operator=(const std::vector<glm::mat4>& value) { _Use(); glUniformMatrix4fv(_location, value.size(), GL_FALSE, reinterpret_cast<const GLfloat *>(value.data())); }
	inline void operator=(const std::vector<glm::mat2x3>& value) { _Use(); glUniformMatrix2x3fv(_location, value.size(), GL_FALSE, reinterpret_cast<const GLfloat *>(value.data())); }
	inline void operator=(const std::vector<glm::mat3x2>& value) { _Use(); glUniformMatrix3x2fv(_location, value.size(), GL_FALSE, reinterpret_cast<const GLfloat *>(value.data())); }
	inline void operator=(const std::vector<glm::mat2x4>& value) { _Use(); glUniformMatrix2x4fv(_location, value.size(), GL_FALSE, reinterpret_cast<const GLfloat *>(value.data())); }
	inline void operator=(const std::vector<glm::mat4x2>& value) { _Use(); glUniformMatrix4x2fv(_location, value.size(), GL_FALSE, reinterpret_cast<const GLfloat *>(value.data())); }
	inline void operator=(const std::vector<glm::mat3x4>& value) { _Use(); glUniformMatrix3x4fv(_location, value.size(), GL_FALSE, reinterpret_cast<const GLfloat *>(value.data())); }
	inline void operator=(const std::vector<glm::mat4x3>& value) { _Use(); glUniformMatrix4x3fv(_location, value.size(), GL_FALSE, reinterpret_cast<const GLfloat *>(value.data())); }

private:
	GLuint _program;
	GLint _location;

	inline void _Use() { ShaderProgramBindings::Use(_program); }

};

class ShaderProgram {

public:
	inline void AddShaderFromSource(const GLuint type, std::string source) { _shaders_to_add[type] = { false, std::move(source) }; }
	inline void AddShaderFromFile  (const GLuint type, std::string file  ) { _shaders_to_add[type] = { true,  std::move(file)   }; }

	void Link();
	inline void Use() const { ShaderProgramBindings::Use((GLuint) _program_id); }

	inline Uniform& operator[](const std::string& name) const { _EnsureUniformInstance(name); return _uniforms[name]; }

private:
	GLuint _AddShaderFromSource(GLuint type, std::string source);
	GLuint _AddShaderFromFile(GLuint type, std::string file);

	void _EnsureUniformInstance(const std::string& name) const;
	GLint _GetUniformLocation(const std::string& name) const;

	GLhandle _program_id;
	std::unordered_map<GLuint, std::pair<bool, std::string>> _shaders_to_add;

	mutable std::unordered_map<std::string, Uniform> _uniforms;

};

#endif
