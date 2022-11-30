#ifndef WEBRTC_ANALYSIS_RTP_H
#define WEBRTC_ANALYSIS_RTP_H

#include <cstdint>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <optional>
#include <ostream>
#include <sstream>

namespace rtp {

    static const unsigned HDR_LEN = 12;

    static bool contains_rtp_or_rtcp(const unsigned char* buf, std::size_t len) {

        return len >= 12 && ((buf[0] >> 6) & 0x03) == 0x02;
    }

    struct hdr {

        // https://datatracker.ietf.org/doc/html/rfc3550#section-5.1

        std::uint8_t  v_p_x_cc = 0;
        std::uint8_t  m_pt     = 0;
        std::uint16_t seq      = 0;
        std::uint32_t ts       = 0;
        std::uint32_t ssrc     = 0;

        [[nodiscard]] unsigned version() const {
            return (v_p_x_cc >> 6) & 0x03;
        }

        [[nodiscard]] unsigned padding() const {
            return (v_p_x_cc >> 5) & 0x01;
        }

        [[nodiscard]] unsigned extension() const {
            return (v_p_x_cc >> 4) & 0x01;
        }

        [[nodiscard]] unsigned csrc_count() const {
            return v_p_x_cc & 0x0f;
        }

        [[nodiscard]] unsigned marker() const {
            return m_pt >> 7 & 0x01;
        }

        [[nodiscard]] unsigned payload_type() const {
            return m_pt & 0x7f;
        }
    };

    struct ext {
        unsigned short type = 0, len = 0;
        unsigned char data[16] = {0};

        bool operator==(const struct ext& other) const {
            return type == other.type && len == other.len
                   && std::strncmp((const char*) data, (const char*) other.data, len) == 0;
        }

        inline bool operator!=(const struct ext& other) const {
            return !(*this == other);
        }

        [[nodiscard]] std::string to_string() const {

            std::stringstream ss;

            for (auto i = 0; i < len; i++) {
                ss << std::hex << std::setw(2) << std::setfill('0') << (unsigned) data[i];
            }

            return ss.str();
        }
    };

    static std::optional<struct ext> get_ext(const rtp::hdr* hdr, unsigned type) {

        struct ext ext;

        const auto* buf = reinterpret_cast<const unsigned char*>(hdr);
        const auto* extBuf = buf + 12;

        if (hdr->csrc_count() == 0 && hdr->extension() == 1) {

            std::uint16_t extBytes = ((extBuf[2] << 8) + (extBuf[3])) * 4;

            for (unsigned i = 4; i < extBytes + 4;) {

                if (extBuf[i] != 0) {

                    unsigned short extType = (extBuf[i] >> 4) & 0x0f;
                    unsigned short extLen = (extBuf[i] & 0x0f) + 1;

                    if (extType == type) {
                        ext.type = extType;
                        ext.len = extLen;
                        std::memcpy(ext.data, extBuf + i + 1,ext.len);
                        return ext;
                    }

                    i += (extLen + 1);

                } else { // padding
                    i++;
                }
            }
        }

        return std::nullopt;
    }

    static std::ostream& operator<<(std::ostream& os, const rtp::hdr& rtp) {
        os << "rtp: v=" << std::dec << rtp.version()
           << ",p=" << std::dec << rtp.padding()
           << ",x=" << std::dec << rtp.extension()
           << ",cc=" << std::dec << rtp.csrc_count()
           << ",m=" << std::dec << rtp.marker()
           << ",pt=" << std::dec << rtp.payload_type()
           << ",seq=" << std::dec << ntohs(rtp.seq)
           << ",ts=" << std::dec << ntohl(rtp.ts)
           << ",ssrc=0x" << std::hex << std::setw(8) << std::setfill('0')
           << ntohl(rtp.ssrc) << std::dec;

        return os;
    }
}

#endif
