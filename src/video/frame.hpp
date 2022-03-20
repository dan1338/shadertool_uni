#pragma once
#include <vector>
#include "script/register.hpp"

namespace video
{
	class FrameSource;

	struct FrameSpec
	{
		size_t width, height;
		unsigned nchannels;
		unsigned depth;
	};

	class Frame : public script::Register
	{
	public:
		friend class FrameSource;

		Frame(const FrameSpec &spec, const bool extern_storage = false);
		~Frame();
		auto size() const -> size_t { return spec.width * spec.height * spec.nchannels * spec.depth; }
		auto width() const -> size_t { return spec.width; }
		auto height() const -> size_t { return spec.height; }
		auto get_spec() const -> const FrameSpec& { return spec; }

		operator unsigned char*() const { return buf; }
		operator void*() const { return buf; }
	private:
		const bool extern_storage;
		const FrameSpec spec;
		unsigned char *buf;
	};
}

