/*
 * SPDX-FileCopyrightText: 2023-2023 CSSlayer <wengxt@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#ifndef LIBIME_ZSTDFILTER_H
#define LIBIME_ZSTDFILTER_H

#include <boost/iostreams/filter/symmetric.hpp>
#include <boost/iostreams/pipeline.hpp>
#include <fcitx-utils/log.h>
#include <fcitx-utils/misc.h>
#include <zstd.h>

namespace libime {

class ZSTDError : public BOOST_IOSTREAMS_FAILURE {
public:
    explicit ZSTDError(size_t error)
        : BOOST_IOSTREAMS_FAILURE(ZSTD_getErrorName(error)), error_(error) {}

    size_t error() const { return error_; }
    static void check(size_t error) {
        if (ZSTD_isError(error)) {
            boost::throw_exception(ZSTDError(error));
        }
    }

private:
    size_t error_;
};

namespace details {

enum class ZSTDResult {
    StreamEnd,
    Okay,
};

class ZSTDFilterBase {
public:
    typedef char char_type;

protected:
    ZSTDFilterBase() = default;
    ZSTDFilterBase(const ZSTDFilterBase &) = delete;
    ~ZSTDFilterBase() {}
    void before(const char *&src_begin, const char *src_end, char *&dest_begin,
                char *dest_end) {
        in_.src = src_begin;
        in_.size = static_cast<size_t>(src_end - src_begin);
        in_.pos = 0;
        out_.dst = dest_begin;
        out_.size = static_cast<size_t>(dest_end - dest_begin);
        out_.pos = 0;
    }
    void after(const char *&src_begin, char *&dest_begin, bool) {
        src_begin = reinterpret_cast<const char *>(in_.src) + in_.pos;
        dest_begin = reinterpret_cast<char *>(out_.dst) + out_.pos;
    }
    void reset() {
        memset(&in_, 0, sizeof(in_));
        memset(&out_, 0, sizeof(out_));
        eof_ = 0;
    }

    ZSTD_inBuffer in_;
    ZSTD_outBuffer out_;
    int eof_ = 0;
};

class ZSTDCompressorImpl : public ZSTDFilterBase {
public:
    ZSTDCompressorImpl() : cstream_(ZSTD_createCStream()) { reset(); }
    bool filter(const char *&src_begin, const char *src_end, char *&dest_begin,
                char *dest_end, bool flush) {
        before(src_begin, src_end, dest_begin, dest_end);
        auto result = deflate(flush);
        after(src_begin, dest_begin, true);
        return result != ZSTDResult::StreamEnd;
    }
    void close() { reset(); }

private:
    void reset() {
        ZSTDFilterBase::reset();
        ZSTDError::check(ZSTD_initCStream(cstream_.get(), 0));
        // Enable checksum.
        ZSTDError::check(
            ZSTD_CCtx_setParameter(cstream_.get(), ZSTD_c_checksumFlag, 1));
    }

    ZSTDResult deflate(bool finish) {
        // Ignore spurious extra calls.
        // Note size > 0 will trigger an error in this case.
        if (eof_ && in_.size == 0)
            return ZSTDResult::StreamEnd;
        size_t result = ZSTD_compressStream(cstream_.get(), &out_, &in_);
        ZSTDError::check(result);
        if (finish) {
            result = ZSTD_endStream(cstream_.get(), &out_);
            ZSTDError::check(result);
            eof_ = result == 0;
            return eof_ ? ZSTDResult::StreamEnd : ZSTDResult::Okay;
        }
        return ZSTDResult::Okay;
    }

    fcitx::UniqueCPtr<ZSTD_CStream, &ZSTD_freeCStream> cstream_;
};

class ZSTDDecompressorImpl : public ZSTDFilterBase {
public:
    ZSTDDecompressorImpl() : dstream_(ZSTD_createDStream()) { reset(); }
    bool filter(const char *&src_begin, const char *src_end, char *&dest_begin,
                char *dest_end, bool flush) {
        before(src_begin, src_end, dest_begin, dest_end);
        auto result = inflate(flush);
        after(src_begin, dest_begin, false);
        return result != ZSTDResult::StreamEnd;
    }
    void close() { reset(); }

private:
    void reset() {
        ZSTDFilterBase::reset();
        ZSTDError::check(ZSTD_initDStream(dstream_.get()));
    }

    ZSTDResult inflate(bool finish) {
        // need loop since iostream code cannot handle short reads
        do {
            size_t result = ZSTD_decompressStream(dstream_.get(), &out_, &in_);
            ZSTDError::check(result);
        } while (in_.pos < in_.size && out_.pos < out_.size);
        return finish && in_.size == 0 && out_.pos == 0 ? ZSTDResult::StreamEnd
                                                        : ZSTDResult::Okay;
    }

    fcitx::UniqueCPtr<ZSTD_DStream, &ZSTD_freeDStream> dstream_;
};

} // namespace details

struct ZSTDCompressor
    : boost::iostreams::symmetric_filter<details::ZSTDCompressorImpl> {
private:
    typedef details::ZSTDCompressorImpl impl_type;
    typedef symmetric_filter<impl_type> base_type;

public:
    typedef typename base_type::char_type char_type;
    typedef typename base_type::category category;
    ZSTDCompressor(std::streamsize buffer_size =
                       boost::iostreams::default_device_buffer_size)
        : base_type(buffer_size) {}
};
BOOST_IOSTREAMS_PIPABLE(ZSTDCompressor, 0)

struct ZSTDDecompressor
    : boost::iostreams::symmetric_filter<details::ZSTDDecompressorImpl> {
private:
    typedef details::ZSTDDecompressorImpl impl_type;
    typedef symmetric_filter<impl_type> base_type;

public:
    typedef typename base_type::char_type char_type;
    typedef typename base_type::category category;
    ZSTDDecompressor(std::streamsize buffer_size =
                         boost::iostreams::default_device_buffer_size)
        : base_type(buffer_size) {}
};
BOOST_IOSTREAMS_PIPABLE(ZSTDDecompressor, 0)

} // namespace libime

#endif
