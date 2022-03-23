#pragma once 
#include <string>
#include "frame.hpp"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavutil/pixdesc.h>
#include <libswscale/swscale.h>
}

namespace video
{
	class FrameSink;

	class FrameSource
	{
	public:
		friend class FrameSink;
		FrameSource(const std::string &path, const std::string &fmt = "rgb24");
		~FrameSource();
		auto operator>>(video::Frame &frame) -> bool;
		auto get_frame_spec() const -> FrameSpec {
			return { (size_t)decoder_ctx->width, (size_t)decoder_ctx->height, 3, 1 };
		}
		auto get_time_step() -> float {
			return stream->r_frame_rate.den / (float)stream->r_frame_rate.num;
		}
	private:
		AVFormatContext *format_ctx;
		AVCodecContext *decoder_ctx;
		AVStream *stream;
		AVPacket *pkt;
		AVFrame *inframe, *outframe;
		SwsContext *sws_ctx;
		AVPixelFormat target_pixfmt;
		const std::string &path;
	};
}

