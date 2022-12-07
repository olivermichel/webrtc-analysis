#ifndef WEBRTC_ANALYSIS_STUN_H
#define WEBRTC_ANALYSIS_STUN_H

#include <cstdint>

namespace stun {

    static bool contains_stun(const unsigned char* buf, std::size_t len) {

        return len >= 20 && (buf[0] >> 6) == 0
               && buf[4] == 0x21 && buf[5] == 0x12 && buf[6] == 0xa4 && buf[7] == 0x42; // cookie
    }

    struct hdr {
        std::uint16_t type         = 0;
        std::uint16_t len          = 0;
        std::uint32_t cookie       = 0;
        unsigned char trans_id[12] = {0};
    };

    static_assert(sizeof(struct hdr) == 20);
}

#endif
