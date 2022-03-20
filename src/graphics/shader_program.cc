#include <stdexcept>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shader_program.hpp"

namespace graphics
{

ShaderProgram::ShaderProgram(const Shader &fragment):
	ShaderProgram({ Shader::VERTEX, "default_vertex", default_vertex_src }, fragment)
{
}

ShaderProgram::ShaderProgram(const Shader &vertex, const Shader &fragment)
{
	int ok;
	char glsl_err[256];

	id = glCreateProgram();
	glAttachShader(id, vertex.id);
	glAttachShader(id, fragment.id);
	glLinkProgram(id);

	glGetProgramiv(id, GL_LINK_STATUS, &ok);

	if (!ok)
	{
		glGetProgramInfoLog(id, sizeof glsl_err, 0, glsl_err);
		throw std::runtime_error(glsl_err);
	}
}

int ShaderProgram::operator[](const std::string &name) const
{
	const int loc = glGetUniformLocation(id, name.c_str());

	if (loc == -1)
		throw std::runtime_error("Unable to find shader uniform");

	return loc;
}

auto ShaderProgram::set(const int loc, const int val) const -> void
{
	glUniform1i(loc, val);
}

auto ShaderProgram::set(const int loc, const float val) const -> void
{
	glUniform1f(loc, val);
}

}

