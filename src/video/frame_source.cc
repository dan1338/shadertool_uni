#include "frame_source.hpp"
#include "video.hpp"
#include <string>
#include <vector>
#include <map>
#include <stdexcept>

namespace video
{

FrameSource::FrameSource(const std::string &path, const std::string &pixfmt):
	format_ctx(0),
	decoder_ctx(0),
	stream(0),
	inframe(0),
	outframe(0),
	pkt(0),
	sws_ctx(0),
	path(path)
{
	target_pixfmt = video::resolve_pixfmt(pixfmt);

	if (target_pixfmt == AV_PIX_FMT_NONE)
		throw std::invalid_argument(std::string("Invalid frame pixel format ") + pixfmt);

	int ret = avformat_open_input(&format_ctx, path.c_str(), 0, 0);

	if (ret != 0)
		throw std::runtime_error(std::string("Unable to open frame source ") + path);

	avformat_find_stream_info(format_ctx, 0);

	AVCodec *decoder = 0;
	ret = av_find_best_stream(format_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, &decoder, 0);

	if (ret < 0)
		throw std::runtime_error("Unable to find video stream");

	stream = format_ctx->streams[ret];
	decoder_ctx = avcodec_alloc_context3(decoder);

	if (!decoder_ctx)
		throw std::runtime_error("Unable to alloc video decoder");

	ret = avcodec_parameters_to_context(decoder_ctx, stream->codecpar);

	if (ret < 0)
		throw std::runtime_error("Unable to init video decoder");

	ret = avcodec_open2(decoder_ctx, decoder, 0);

	if (ret < 0)
		throw std::runtime_error("Unable to open video decoder");

	if (decoder_ctx->width == 0 || decoder_ctx->height == 0)
		throw std::runtime_error("Unable to infer video resolution");

	if (decoder_ctx->pix_fmt != target_pixfmt)
	{
		const size_t w = decoder_ctx->width;
		const size_t h = decoder_ctx->height;
		const auto srcfmt = decoder_ctx->pix_fmt;
		const auto dstfmt = target_pixfmt;
		sws_ctx = sws_getContext(w, h, srcfmt, w, h, dstfmt, SWS_BILINEAR, 0, 0, 0);
		inframe = av_frame_alloc();
		outframe = av_frame_alloc();
		outframe->width = w;
		outframe->height = h;
		outframe->format = dstfmt;
		
		if (av_frame_get_buffer(outframe, 0) < 0)
			throw std::runtime_error("Unable to alloc outframe buffer");
	}
	else
	{
		inframe = av_frame_alloc();
		outframe = inframe;
	}

	if (inframe == 0 || outframe == 0)
		throw std::runtime_error("Unable to alloc video frame");

	pkt = av_packet_alloc();

	if (pkt == 0)
		throw std::runtime_error("Unable to alloc video packet");
}

FrameSource::~FrameSource()
{
	av_packet_free(&pkt);

	if (inframe == outframe)
	{
		av_frame_free(&inframe);
	}
	else
	{
		av_frame_free(&inframe);
		av_frame_free(&outframe);
	}

	avcodec_close(decoder_ctx);
}

auto FrameSource::operator>>(video::Frame &frame) -> bool
{
	int ret;

	do
	{
		while (1)
		{
			ret = av_read_frame(format_ctx, pkt);

			if (ret < 0)
				return false;

			if (format_ctx->streams[pkt->stream_index] == stream)
				break;
		}
		
		ret = avcodec_send_packet(decoder_ctx, pkt);

		if (ret < 0)
			return false;

		ret = avcodec_receive_frame(decoder_ctx, inframe);
		av_packet_unref(pkt);
	}
	while (ret == AVERROR(EAGAIN));

	if (ret == 0)
	{
 		// The decoded pixel format has to be converted
		if (sws_ctx)
		{
			ret = sws_scale(sws_ctx, inframe->data, inframe->linesize, 0, inframe->height,
					outframe->data, outframe->linesize);

			if (ret < 0)
				return false;
		}

		// Copy each row manually because in AVFrame they are aligned and linesize[0] is the actual row size
		for (size_t y = 0; y < frame.height(); ++y)
		{
			unsigned char *dst = frame.buf + (y * frame.width() * 3);
			const unsigned char *src = outframe->data[0] + (y * outframe->linesize[0]);
			memcpy(dst, src, frame.width() * 3);
		}

		return true;
	}

	return false;
}

}

