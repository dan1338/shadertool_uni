#pragma once
#include <GL/glew.h>

namespace graphics
{
	class RenderDevice;

	class RenderTarget
	{
	public:
		friend class RenderDevice;

		RenderTarget(const size_t width, const size_t height):
			width(width), height(height)
		{
			glGenFramebuffers(1, &fbo);
			glBindFramebuffer(GL_FRAMEBUFFER, fbo);

			glGenRenderbuffers(1, &rbo);
			glBindRenderbuffer(GL_RENDERBUFFER, rbo);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_RGB8, width, height);
			
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rbo);
		}

		~RenderTarget()
		{
			glDeleteFramebuffers(1, &fbo);
			glDeleteRenderbuffers(1, &rbo);
		}

	private:
		unsigned fbo;
		unsigned rbo;
		const size_t width, height;
	};
}

