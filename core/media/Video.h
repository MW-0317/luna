#pragma once
#include <iostream>
#include <thirdparty/ffmpeg.h>

namespace luna
{
	class Video
	{
	protected:
		int frameNumber, outSize, size, x, y, bitrate, outbufSize;
		AVCodec* codec;
		AVCodecContext* c = NULL;
		AVStream* stream = NULL;
		AVFormatContext* ofc = NULL;
		AVOutputFormat* oformat = NULL;
		SwsContext* swsCtx = NULL;
		FILE* f;
		AVFrame* picture;
		uint8_t* outbuf, * pictureBuf;
	public:
		const char* filename;
		float fps;
		float seconds;

		int width, height;

		Video(const char* filename, float fps, float seconds, int width, int height);
		~Video();

		// Encodes frame from given buffer.
		void encodeFrame(uint8_t* pixels, int pixelSize);
		void save();
		void free();

		bool isFull();
	};
}
