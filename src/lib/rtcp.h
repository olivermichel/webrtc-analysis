#ifndef WEBRTC_ANALYSIS_RTCP_H
#define WEBRTC_ANALYSIS_RTCP_H

#include <iomanip>

namespace rtcp {

    static bool contains_rtcp(const unsigned char* buf, std::size_t len) {

        return len >= 12 && ((buf[0] >> 6) & 0x03) == 0x02 && buf[1] > 191;
    }

    static std::string pt_name(std::uint8_t pt) {

        switch (pt) {
            case 200: return "sr";
            case 201: return "rr";
            case 202: return "sdes";
            case 203: return "bye";
            case 204: return "app";
            default:  return "unknown";
        }
    }

    struct hdr {

        // https://datatracker.ietf.org/doc/html/rfc3550#section-6.4.1
        // https://datatracker.ietf.org/doc/html/rfc3550#section-6.4.2

        std::uint8_t v_p_rc = 0;
        std::uint8_t pt     = 0;
        std::uint16_t len   = 0;
        std::uint32_t ssrc  = 0;

        [[nodiscard]] unsigned version() const {
            return (v_p_rc >> 6) & 0x03;
        }

        [[nodiscard]] unsigned padding() const {
            return (v_p_rc >> 5) & 0x01;
        }

        [[nodiscard]] unsigned recep_rep_count() const {
            return v_p_rc & 0x1f;
        }

        struct sr {
            std::uint32_t ntp_ts_msw        = 0;
            std::uint32_t ntp_ts_lsw        = 0;
            std::uint32_t rtp_ts            = 0;
            std::uint32_t sender_pkt_count  = 0;
            std::uint32_t sender_byte_count = 0;
        };

        union msg {
            struct sr sr;
        };

        msg msg;
    };

    static std::ostream &operator<<(std::ostream &os, const rtcp::hdr &rtcp) {
        os << "rtcp: v="  << std::dec << rtcp.version()
           << ",p="      << std::dec << rtcp.padding()
           << ",rc="     << std::dec << rtcp.recep_rep_count()
           << ",pt="     << std::dec << (unsigned) rtcp.pt
           << ",len="    << std::dec << ntohs(rtcp.len)
           << ",ssrc=0x" << std::hex << std::setw(8) << std::setfill('0')
           << ntohl(rtcp.ssrc) << std::dec;

        return os;
    }
}

#endif