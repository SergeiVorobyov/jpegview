#include <brunsli/jpeg_data.h>
#include <brunsli/types.h>
#include <brunsli/status.h>

extern "C"
{
    // Check if data looks like Brunsli stream. Currently, only 6 byte signature is compared (i.e. if |len| < 6, result is always "false").
    bool IsBrunsli(const uint8_t* data, size_t len);

    // Parses the brunsli byte stream contained in data[0 ... len) and fills in *jpg
    // with the parsed information.
    // The *jpg object is valid only as long as the input data is valid.
    // Returns BRUNSLI_OK, unless the data is not valid brunsli byte stream, or is
    // truncated.
    brunsli::BrunsliStatus BrunsliDecodeJpeg(const uint8_t* data, size_t len, brunsli::JPEGData* jpg);

    // Returns the estimated peak memory usage (in bytes) of the BrunsliDecodeJpeg
    // function. If parsing is failed, then result is 0.
    size_t BrunsliEstimateDecoderPeakMemoryUsage(const uint8_t* data, size_t len);

    // Returns an upper bound on the size of the buffer needed to encode the given
    // jpg data in brunsli format.
    size_t GetMaximumBrunsliEncodedSize(const brunsli::JPEGData& jpg);

    // Returns the estimated peak memory usage (in bytes) of BrunsliEncodeJpeg().
    size_t EstimateBrunsliEncodePeakMemoryUsage(size_t jpg_size, const brunsli::JPEGData& jpg);

    // Encodes the given jpg to the buffer data[0 ... *len) in brunsli format and
    // sets *len to the encoded size. Returns false on buffer overflow or invalid
    // jpg data.
    bool BrunsliEncodeJpeg(const brunsli::JPEGData& jpg, uint8_t* data, size_t* len);

    // Return the storage size needed to store raw jpg data in bypass mode.
    size_t GetBrunsliBypassSize(size_t jpg_size);

    // Bypass mode: store the JPEG data directly into brunsli format. *len should
    // contain the maximum storage space available. Upon return, *len is updated to
    // the actual size used in 'data'.
    bool BrunsliEncodeJpegBypass(const uint8_t* jpg_data, size_t jpg_data_len, uint8_t* data, size_t* len);
}
