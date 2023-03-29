#pragma once

class brunsliReaderWriter
{
public:
	static void* ReadImage(int& width, int& height, int& bpp, bool& outOfMemory, const void* buffer, int sizebytes);
	static void* Compress(const void* buffer, int width, int height, int& len);
};