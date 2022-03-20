#include "frame_sink.hpp"
#include "video.hpp"
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <stdexcept>

namespace video
{

FrameSink::FrameSink(const std::string &path, const FrameSource &source):
	format_ctx(0),
	encoder_ctx(0),
	path(path)
{
	avformat_alloc_output_context2(&format_ctx, 0, 0, path.c_str());

	if (!format_ctx)
		throw std::runtime_error("Unable to create output format context");

	stream = avformat_new_stream(format_ctx, 0);

	int ret = avcodec_parameters_copy(stream->codecpar, source.stream->codecpar);

	const AVCodec *encoder = avcodec_find_encoder(source.decoder_ctx->codec_id);

	if (!encoder)
		throw std::runtime_error("Unable to find suitable encoder");

	if (ret < 0)
		throw std::runtime_error("Unable to copy codec parameters");

	encoder_ctx = avcodec_alloc_context3(encoder);
	ret = avcodec_parameters_to_context(encoder_ctx, source.stream->codecpar);

	int fps_num = source.stream->r_frame_rate.num;
	int fps_den = source.stream->r_frame_rate.den;

	stream->avg_frame_rate = stream->r_frame_rate = { fps_num, fps_den };
	stream->time_base = { 1, 1000 };
	// Reduce time base by half as a "temporary" fix for h264 pts scaling
	encoder_ctx->time_base = { fps_den, 1*fps_num };
	encoder_ctx->bit_rate = source.decoder_ctx->bit_rate;
	encoder_ctx->gop_size = source.decoder_ctx->gop_size;
	
	if (!encoder_ctx)
		throw std::runtime_error("Unable to alloc video encoder");

	if (ret < 0)
		throw std::runtime_error("Unable to init video encoder");

	ret = avcodec_open2(encoder_ctx, encoder, 0);

	if (ret < 0)
		throw std::runtime_error("Unable to open video encoder");

	ret = avio_open(&format_ctx->pb, path.c_str(), AVIO_FLAG_WRITE);

	if (ret < 0)
		throw std::runtime_error("Unable open output stream");

	ret = avformat_write_header(format_ctx, 0);

	if (ret < 0)
		throw std::runtime_error("Unable to write output header");

	target_pixfmt = AV_PIX_FMT_RGB24;

	const size_t w = encoder_ctx->width;
	const size_t h = encoder_ctx->height;
	const auto srcfmt = target_pixfmt;
	const auto dstfmt = encoder_ctx->pix_fmt;
	sws_ctx = sws_getContext(w, h, srcfmt, w, h, dstfmt, SWS_BILINEAR, 0, 0, 0);
	inframe = av_frame_alloc();
	inframe->width = w;
	inframe->height = h;
	inframe->format = srcfmt;
	outframe = av_frame_alloc();
	outframe->width = w;
	outframe->height = h;
	outframe->format = dstfmt;

	if (av_frame_get_buffer(inframe, 0) < 0)
		throw std::runtime_error("Unable to alloc outframe buffer");

	if (av_frame_get_buffer(outframe, 0) < 0)
		throw std::runtime_error("Unable to alloc outframe buffer");

	pkt = av_packet_alloc();

	if (pkt == 0)
		throw std::runtime_error("Unable to alloc video packet");
}

FrameSink::~FrameSink()
{
	drain_pipeline();

	av_write_trailer(format_ctx);
	av_frame_free(&inframe);
	av_frame_free(&outframe);
	av_packet_free(&pkt);
	avio_closep(&format_ctx->pb);
}

auto FrameSink::operator<<(const video::Frame &frame) -> bool
{
	int ret;

	do 
	{
		memcpy(inframe->data[0], frame, frame.size());
		inframe->linesize[0] = frame.width() * 3;

		ret = sws_scale(sws_ctx, inframe->data, inframe->linesize, 0, inframe->height,
				outframe->data, outframe->linesize);

		outframe->pts = encoder_ctx->frame_number;

		if (ret < 0)
			return false;

		ret = avcodec_send_frame(encoder_ctx, outframe);

		if (ret < 0)
			return false;

		while ((ret = avcodec_receive_packet(encoder_ctx, pkt)) >= 0)
		{
			pkt->pts = av_rescale_q(pkt->pts, encoder_ctx->time_base, stream->time_base);
			pkt->dts = av_rescale_q(pkt->dts, encoder_ctx->time_base, stream->time_base);

			ret = av_write_frame(format_ctx, pkt);

			if (ret < 0)
				return false;

			av_packet_unref(pkt);
		}

	}
	while (ret >= 0);

	return true;
}

auto FrameSink::drain_pipeline() -> void
{
	// Sending nullptr frame sets drain flag
	avcodec_send_frame(encoder_ctx, 0);

	while (avcodec_receive_packet(encoder_ctx, pkt) >= 0)
	{
		pkt->pts = av_rescale_q(pkt->pts, encoder_ctx->time_base, stream->time_base);
		pkt->dts = av_rescale_q(pkt->dts, encoder_ctx->time_base, stream->time_base);

		av_write_frame(format_ctx, pkt);
		av_packet_unref(pkt);
	}
}

}

