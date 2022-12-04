
#include <arpa/inet.h>
#include <cstdint>

namespace rtcp::twcc {

    struct hdr {

        std::uint32_t source_ssrc           = 0;
        std::uint16_t base_seq              = 0;
        std::uint16_t pkt_status_count      = 0;
        std::uint32_t ref_time_fb_pkt_count = 0;

        [[nodiscard]] int ref_time() const {
            return ntohl(ref_time_fb_pkt_count) >> 8;
        }

        [[nodiscard]] unsigned fb_pkt_count() const {
            return (unsigned) ntohl(ref_time_fb_pkt_count) & 0x000000ff;
        }
    };

    static const std::size_t HDR_LEN = 12;
}
