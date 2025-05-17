#include "StdAfx.h"
#include "brunsliWrapper.h"
#include <brunsli/decode.h>

//using namespace brunsli;
namespace brunsliWrapper
{
	bool IsBrunsli(const uint8_t* data, const size_t len)
	{
		static const uint8_t kSignature[6] = {
			/* marker */ 0x0A,
			/* length */ 0x04, 0x42, 0xD2, 0xD5, 0x4E };
		static const size_t kSignatureLen = sizeof(kSignature);
		return (len < kSignatureLen) ? false : (memcmp(kSignature, data, kSignatureLen) == 0);
	}

	size_t DecodeBrunsliSink2(void* out_data, const uint8_t* buf, size_t size)
	{
		return 0;
	}

	void* brunsliLibraryWraper::ReadImage(int& width,
		int& height,
		int& nchannels,
		bool& outOfMemory,
		const void* buffer,
		int sizebytes) const
	{
		outOfMemory = false;

		typedef int(STDAPICALLTYPE* DecodeBrunsliFn)(size_t, const uint8_t*, void*, DecodeBrunsliSink);

		//DecodeBrunsliFn decodeBrunsli = (DecodeBrunsliFn)
			::GetProcAddress(brunsliDecoder, "DecodeBrunsli");
		if (decodeBrunsli == NULL)
			return NULL;

		decodeBrunsli(sizebytes, (const uint8_t*)buffer, NULL, &DecodeBrunsliSink2);

		return NULL;
	}

	void* brunsliLibraryWraper::Compress(const void* source,
		int width,
		int height,
		int& len) const
	{
		int nchannels = 3;
		void* pOutput = NULL;
		return NULL;
	}

	brunsliLibraryWraper::brunsliLibraryWraper()
		: brunsliDecoder(NULL)
	{
	}

	brunsliLibraryWraper::~brunsliLibraryWraper()
	{
		if (brunsliDecoder != NULL)
			::FreeLibrary(brunsliDecoder);
	}

	bool brunsliLibraryWraper::is_decoder_load() const
	{
		return brunsliDecoder != NULL;
	}

	bool brunsliLibraryWraper::load_decoder()
	{
		ATLASSERT(brunsliDecoder == NULL);
		brunsliDecoder = ::LoadLibrary(_T("brunslidec-c.dll"));
		return is_decoder_load();
	}
}
