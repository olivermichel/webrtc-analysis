#ifndef WEBRTC_ANALYSIS_STUN_H
#define WEBRTC_ANALYSIS_STUN_H

#include <cstdint>

namespace stun {

    static bool contains_stun(const unsigned char* buf, std::size_t len) {

        return len >= 20 && (buf[0] >> 6) == 0
               && buf[4] == 0x21 && buf[5] == 0x12 && buf[6] == 0xa4 && buf[7] == 0x42; // cookie
    }
}

#endif
