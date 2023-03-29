#include "StdAfx.h"
#include "brunsliWrapper.h"
#include "brunsli/encode.h"

void* brunsliReaderWriter::ReadImage(int& width,
	int& height,
	int& nchannels,
	bool& outOfMemory,
	const void* buffer,
	int sizebytes)
{
	outOfMemory = false;
	return NULL;
}

void* brunsliReaderWriter::Compress(const void* source,
	int width,
	int height,
	int& len)
{
	int nchannels = 3;
	void* pOutput = NULL;
	return NULL;
}
