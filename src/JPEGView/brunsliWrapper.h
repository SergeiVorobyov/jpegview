#pragma once

namespace brunsliWrapper
{
	class brunsliLibraryWraper
	{
		HMODULE brunsliDecoder;

	public:
		brunsliLibraryWraper();
		~brunsliLibraryWraper();

		bool is_decoder_load() const;
		bool load_decoder();
		void* ReadImage(int& width, int& height, int& bpp, bool& outOfMemory, const void* buffer, int sizebytes) const;
		void* Compress(const void* buffer, int width, int height, int& len) const;
	};

	// Check if data looks like Brunsli stream. Currently, only 6 byte signature is compared (i.e. if |len| < 6, result is always "false").
	bool IsBrunsli(const uint8_t* data, const size_t len);
};
