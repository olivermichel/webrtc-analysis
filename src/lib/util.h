
#ifndef WEBRTC_ANALYSIS_UTIL_H
#define WEBRTC_ANALYSIS_UTIL_H

#include <vector>

namespace util {

    static std::uint32_t extract_bits(const std::vector<bool>& bits, unsigned from, unsigned len = 1) {

        if (from + len > bits.size())
            throw std::invalid_argument("util:extract_bits: requested range outside of bit vector");

        if (len > 32 || len < 1)
            throw std::invalid_argument("util:extract_bits: len must be > 0 and <= 32");

        std::uint32_t a = 0;

        for (auto i = from, j = len - 1; i < from + len; i++, j--)
            a |= (bits[i] << j);

        return a;
    }
}




#endif
