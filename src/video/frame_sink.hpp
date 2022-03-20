#pragma once 
#include <string>
#include "frame.hpp"
#include "frame_source.hpp"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavutil/pixdesc.h>
#include <libswscale/swscale.h>
}

namespace video
{
	class FrameSink
	{
	public:
		FrameSink(const std::string &path, const FrameSource &source);
		~FrameSink();
		auto operator<<(const video::Frame &frame) -> bool;
	private:
		AVFormatContext *format_ctx;
		AVCodecContext *encoder_ctx;
		AVStream *stream;
		AVPacket *pkt;
		AVFrame *inframe, *outframe;
		SwsContext *sws_ctx;
		AVPixelFormat target_pixfmt;
		const std::string &path;

		auto drain_pipeline() -> void;
	};
}

