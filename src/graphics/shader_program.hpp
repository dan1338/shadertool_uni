#pragma once
#include <string>
#include "shader.hpp"

namespace graphics
{
	class RenderDevice;

	class ShaderProgram
	{
	public:
		friend class RenderDevice;
		ShaderProgram(const Shader &fragment);
		ShaderProgram(const Shader &vertex, const Shader &fragment);
		int operator[](const std::string &name) const;
		auto set(const int loc, const int val) const -> void;
		auto set(const int loc, const float val) const -> void;
	private:
		int id;
	};

	constexpr auto default_vertex_src =
		"#version 140\n"\
		"in vec2 in_pos;\n"\
		"in vec2 in_uv;\n"\
		"out vec2 UV;\n"\
		"void main() {\n"\
		"UV = in_uv;\n"\
		"gl_Position = vec4(in_pos.xy, 0.0, 1.0);\n"\
		"}\n";
	
	constexpr auto default_fragment_src =
		"#version 140\n"\
		"in vec2 UV;\n"\
		"out vec4 COLOR;\n"\
		"uniform sampler2D frame;\n"\
		"void main() {\n"\
		"COLOR = vec4(texture(frame, UV).rgb, 1.0);\n"\
		"}\n";
}

