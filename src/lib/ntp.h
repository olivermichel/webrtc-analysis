#ifndef WEBRTC_ANALYSIS_NTP_H
#define WEBRTC_ANALYSIS_NTP_H

#include <cstdint>
#include <sys/time.h>

namespace ntp {

    static const unsigned NTP_UNIX_OFFSET = 2208988800;

    std::uint64_t ntp_from_timeval(timeval tv) {

        auto ntp_sec = (std::uint64_t) tv.tv_sec + NTP_UNIX_OFFSET;
        auto ntp_frac =  (std::uint64_t) (((double) tv.tv_usec / 1000000) * 0xffffffff);
        return ntp_sec << 32 | ntp_frac;
    }

    std::uint32_t middle_bits_from_ntp(const std::uint64_t t) {

        return (t >> 16);
    }
}

#endif

