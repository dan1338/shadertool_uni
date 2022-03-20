#include <string>
#include <vector>
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shader.hpp"
#include "fs/read.hpp"

namespace graphics
{

static std::string path_to_name(const std::string &path)
{
	std::string s = path;
	size_t dir = s.rfind("/");

	if (dir != std::string::npos)
		s = s.substr(dir + 1, s.size());

	size_t ext = s.rfind(".");

	if (ext != std::string::npos)
		s = s.substr(ext + 1, s.size());

	return s;
}

Shader::Shader(const Shader::Type type, const std::string &path):
	Shader(type, path_to_name(path), fs::read_string(path))
{
}

Shader::Shader(const Shader::Type type, const std::string &name, const std::string &src):
	id(-1), type(type), name(name), src(src)
{
	int ok;
	char glsl_err[256];

	switch (type)
	{
	case Shader::VERTEX:
		id = glCreateShader(GL_VERTEX_SHADER);
		break;
	case Shader::FRAGMENT:
		id = glCreateShader(GL_FRAGMENT_SHADER);
		break;
	}

	const char *p = src.c_str();
	glShaderSource(id, 1, &p, 0);
	glCompileShader(id);
	glGetShaderiv(id, GL_COMPILE_STATUS, &ok);

	if (!ok)
	{
		glGetShaderInfoLog(id, sizeof glsl_err, 0, glsl_err);
		throw std::runtime_error(glsl_err);
	}

	if (0) // DEBUG
	{
		std::cout << "shader (";
		switch (type) {
			case VERTEX: std::cout << "vertex"; break;
			case FRAGMENT: std::cout << "fragment"; break;
		}
		std::cout << ")\n";
		std::cout << src;
	}
}

Shader::~Shader()
{
	if (id != -1)
		glDeleteShader(id);
}

}

