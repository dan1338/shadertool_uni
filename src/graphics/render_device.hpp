#pragma once
#include <stdexcept>
#include <vector>
#include <array>
#include <map>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "render_target.hpp"
#include "video/frame.hpp"
#include "graphics/shader_program.hpp"

namespace graphics
{
	class RenderDevice
	{
	public:
		RenderDevice(const size_t default_width = 800, const size_t default_height = 600);
		~RenderDevice();
		auto use_shader(const ShaderProgram &shader) -> void;
		auto set_target(const RenderTarget &target) -> void;
		auto reset_target() -> void;
		auto get_frame_spec() const -> video::FrameSpec;
		auto load_texture(const std::string &name, const video::Frame &frame) -> void;
		auto read_frame(const video::Frame &frame) -> void;
		auto clear() -> void;
		auto draw() -> void;
		auto swap_buffers() -> void;
	private:
		const size_t width, height;
		unsigned vao;
		unsigned vbo;
		const ShaderProgram *current_shader;
		const RenderTarget *current_target;
		GLFWwindow *window;

		// texmap associates name to texture unit
		std::map<const std::string, unsigned> texmap;
		// texids stores texture ids and is indexed by texture unit
		std::array<unsigned, 32> texids;

		auto find_texunit(const std::string &name) -> unsigned;
	};
}

