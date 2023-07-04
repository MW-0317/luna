#include "Video.h"
namespace luna
{
	Video::Video(const char* filename, float fps, float seconds, int width, int height)
	{
		// TODO: Follow https://github.com/shi-yan/videosamples/blob/master/libavmp4encoding/main.cpp
		// main function
		oformat = (AVOutputFormat*) av_guess_format(NULL, filename, NULL);
		if (!oformat)
		{
			std::cout << "VIDEO::CANNOT_CREATE_OUTPUT_FORMAT" << std::endl;
			return;
		}

		int err = avformat_alloc_output_context2(&ofc, oformat, NULL, filename);

		if (err)
		{
			std::cout << "VIDEO::CANNOT_CREATE_OUTPUT_CONTEXT";
			return;
		}
				
		// "resolution must be a multiple of two"
		width = width + width % 2;
		height = height + height % 2;
		frameNumber = 0;
		bitrate = 80000000;
		this->filename = filename;
		this->fps = fps;
		this->seconds = seconds;
		this->width = width;
		this->height = height;
		
		//codec = (AVCodec*) avcodec_find_encoder(oformat->video_codec);
		codec = (AVCodec*)avcodec_find_encoder(AV_CODEC_ID_H264);
		if (!codec)
		{
			std::cout << "VIDEO::CODEC_NOT_FOUND" << std::endl;
		}

		stream = avformat_new_stream(ofc, codec);
		if (!stream)
		{
			std::cout << "VIDEO::CANNOT_FIND_FORMAT" << std::endl;
			return;
		}

		c = avcodec_alloc_context3(codec);

		if (!c)
		{
			std::cout << "VIDEO::CANNOT_CREATE_CODEC_CONTEXT" << std::endl;
			return;
		}

		//stream->codecpar->codec_id		= oformat->video_codec;
		stream->codecpar->codec_id		= AV_CODEC_ID_H264;
		stream->codecpar->codec_type	= AVMEDIA_TYPE_VIDEO;
		stream->codecpar->width			= width;
		stream->codecpar->height		= height;
		stream->codecpar->format		= AV_PIX_FMT_YUV420P;
		stream->codecpar->bit_rate		= bitrate;
		stream->avg_frame_rate			= av_make_q(fps, 1);

		avcodec_parameters_to_context(c, stream->codecpar);

		c->bit_rate = bitrate;
		c->width	= width;
		c->height	= height;

		c->time_base	= av_make_q(1, 1);
		c->framerate	= av_make_q(fps, 1);
		c->gop_size		= 60;
		c->max_b_frames	= 1;
		c->pix_fmt		= AV_PIX_FMT_YUV420P;
		
		if (stream->codecpar->codec_id == AV_CODEC_ID_H264)
		{
			av_opt_set(c, "preset", "ultrafast", 0);
			av_opt_set(c->priv_data, "profile", "high", 0);
		}
		else if (stream->codecpar->codec_id == AV_CODEC_ID_H265)
			av_opt_set(c, "preset", "ultrafast", 0);
		else
			av_opt_set_int(c, "lossless", 1, 0);

		avcodec_parameters_from_context(stream->codecpar, c);

		if (avcodec_open2(c, codec, 0) < 0)
		{
			std::cout << "VIDEO::COULD_NOT_OPEN_CODEC" << std::endl;
		}

		if (!(oformat->flags & AVFMT_NOFILE))
		{
			if (avio_open(&ofc->pb, filename, AVIO_FLAG_WRITE) < 0)
			{
				std::cout << "VIDEO::FAILED_TO_OPEN_FILE" << std::endl;
				return;
			}
		}

		if (avformat_write_header(ofc, NULL) < 0)
		{
			std::cout << "VIDEO::FAILED_TO_WRITE_HEADER" << std::endl;
			return;
		}

		av_dump_format(ofc, 0, filename, 1);

		/*
		f = fopen(filename, "wb");
		if (!f)
		{
			std::cout << "VIDEO::COULD_NOT_OPEN" << std::endl;
		}

		outbufSize = 100000;
		outbuf = (uint8_t*)malloc(outbufSize);
		size = width * height;
		pictureBuf = (uint8_t*)malloc((size * 3) / 2);
		
		picture->data[0] = pictureBuf;
		picture->data[1] = picture->data[0] + size;
		picture->data[2] = picture->data[1] + size / 4;
		picture->linesize[0] = c->width;
		picture->linesize[1] = c->width / 2;
		picture->linesize[2] = c->width / 2;
		*/
		
	}

	void Video::encodeFrame(uint8_t* pixels, int pixelSize)
	{
		int err;
		if (!picture)
		{
			picture = av_frame_alloc();
			picture->format = AV_PIX_FMT_YUV420P;
			picture->width = c->width;
			picture->height = c->height;

			err = av_frame_get_buffer(picture, 32);
			if (err < 0)
			{
				std::cout << "VIDEO::FAILED_TO_ALLOCATE_PICTURE" << std::endl;
				return;
			}
		}

		if (!swsCtx)
		{
			swsCtx = sws_getContext(c->width, c->height, AV_PIX_FMT_RGBA,
				c->width, c->height, AV_PIX_FMT_YUV420P, SWS_BICUBIC, 0, 0, 0);
		}

		AVFrame* rgbaFrame = av_frame_alloc();
		int size = c->width * c->height * 4;
		uint8_t* data = new uint8_t[size];
		int j = 0;
		for (int y = c->height - 1; y >= 0; y--)
		{
			for (int x = 0; x < c->width; x++)
			{
				for (int v = 4 - 1; v >= 0; v--)
				{
					data[j] = pixels[y * c->width * 4 + x * 4 + v];
					j++;
				}
			}
		}
		rgbaFrame->data[0] = data;

		int inLinesize[] = { 4 * c->width };

		sws_scale(swsCtx, rgbaFrame->data, inLinesize, 0, c->height,
			picture->data, picture->linesize);

		picture->pts = (frameNumber++) * stream->time_base.den / (stream->time_base.num * fps);

		err = avcodec_send_frame(c, picture);
		if (err < 0)
		{
			std::cout << "VIDEO::FRAME_COULDNT_SEND::" << err << std::endl;
			exit(1);
		}

		AVPacket* pkt = av_packet_alloc();
		pkt->data = NULL;
		pkt->size = 0;
		pkt->flags |= AV_PKT_FLAG_KEY;
		int ret = avcodec_receive_packet(c, pkt);
		if (ret == 0)
		{
			av_interleaved_write_frame(ofc, pkt);
		}
		av_packet_unref(pkt);
	}

	void Video::save()
	{
		AVPacket* pkt = av_packet_alloc();
		pkt->data = NULL;
		pkt->size = 0;

		for (;;)
		{
			avcodec_send_frame(c, NULL);
			if (avcodec_receive_packet(c, pkt) == 0)
			{
				av_interleaved_write_frame(ofc, pkt);
			}
			else
				break;
		}
		av_packet_unref(pkt);

		av_write_trailer(ofc);
		if (!(oformat->flags & AVFMT_NOFILE))
		{
			int err = avio_close(ofc->pb);
			if (err < 0)
				std::cout << "VIDEO::CLOSE_FILE_FAILED::" << err << std::endl;
		}
	}

	void Video::free()
	{
		if (picture)
			av_frame_free(&picture);

		if (c)
			avcodec_free_context(&c);

		if (ofc)
			avformat_free_context(ofc);

		if (swsCtx)
			sws_freeContext(swsCtx);
	}

	bool Video::isFull()
	{
		return (frameNumber / fps > seconds);
	}
}