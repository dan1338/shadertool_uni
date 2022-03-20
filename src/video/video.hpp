#pragma once
#include <string>

extern "C" {
#include <libavutil/pixdesc.h>
}

namespace video
{
	static AVPixelFormat resolve_pixfmt(const std::string &name)
	{
		return av_get_pix_fmt(name.c_str());
	}
}

