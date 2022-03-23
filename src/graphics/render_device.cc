#include "render_device.hpp"

namespace graphics
{

RenderDevice::RenderDevice(const size_t default_width, const size_t default_height):
	current_shader(0), width(width), height(height)
{
	if (glfwInit() == GLFW_FALSE)
		throw std::runtime_error("Unable to initialize glfw");

	glfwWindowHint(GLFW_RESIZABLE, 0);
	glfwWindowHint(GLFW_VISIBLE, 0);
	glfwWindowHint(GLFW_DECORATED, 0);

	window = glfwCreateWindow(default_width, default_height, "shadertool", 0, 0);

	if (!window)
		throw std::runtime_error("Unable to create glfw window");

	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
		throw std::runtime_error("Unable to initialize glew");

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	static float fullrect_verts[] = {
		//   pos         uv
		-1.0f,  1.0f, 0.0f, 0.0f,
		-1.0f, -1.0f, 0.0f, 1.0f,
		1.0f, -1.0f, 1.0f, 1.0f,

		-1.0f,  1.0f, 0.0f, 0.0f,
		1.0f,  1.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 1.0f, 1.0f,
	};

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fullrect_verts), fullrect_verts, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_TRUE, 4 * sizeof(float), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_TRUE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glDisable(GL_DEPTH_TEST);

	texids.fill(-1);
}

RenderDevice::~RenderDevice()
{
	glfwDestroyWindow(window);
}

auto RenderDevice::use_shader(const ShaderProgram &shader) -> void
{
	current_shader = &shader;
	glUseProgram(shader.id);

	for (const auto pair : texmap)
	{
		const auto id = texids[pair.second];
		glDeleteTextures(1, &id);
	}
	
	texmap.clear();
	texids.fill(-1);
}

auto RenderDevice::set_target(const RenderTarget &target) -> void
{
	glBindFramebuffer(GL_FRAMEBUFFER, target.fbo);
	glViewport(0, 0, target.width, target.height);
	current_target = &target;
}

auto RenderDevice::reset_target() -> void
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, width, height);
	current_target = 0;
}

auto RenderDevice::get_frame_spec() const -> video::FrameSpec
{
	if (!current_target)
		throw std::runtime_error("No bound rendering target");

	return { current_target->width, current_target->height, 3, 1 };
}

auto RenderDevice::load_texture(const std::string &name, const video::Frame &frame) -> void
{
	const auto unit = find_texunit(name);

	if (texids[unit] == -1)
		glGenTextures(1, &texids[unit]);

	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, texids[unit]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame.width(), frame.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, frame);
	glGenerateMipmap(GL_TEXTURE_2D);
	
	if (current_shader)
	{
		current_shader->set((*current_shader)[name], static_cast<int>(unit));
	}
}

auto RenderDevice::read_frame(const video::Frame &frame) -> void
{
	glReadPixels(0, 0, frame.width(), frame.height(), GL_RGB, GL_UNSIGNED_BYTE, frame);
}

auto RenderDevice::clear() -> void
{
	glClear(GL_COLOR_BUFFER_BIT);
}

auto RenderDevice::draw() -> void
{
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

auto RenderDevice::swap_buffers() -> void
{
	glfwSwapBuffers(window);
}


auto RenderDevice::find_texunit(const std::string &name) -> unsigned
{
	const auto unit = texmap.find(name);

	if (unit != texmap.end())
		return unit->second;

	for (size_t i = 0; i < texids.size(); ++i)
	{
		if (texids[i] == -1)
		{
			texmap[name] = i;
			return i;
		}
	}

	throw std::runtime_error("Exceeded the maximum number of textures");
}

}

