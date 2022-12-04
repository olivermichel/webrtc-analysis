
#include <arpa/inet.h>
#include <cstdint>
#include <vector>

#include "rtcp.h"

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

    struct pkt_fb {
        std::uint16_t seq  = 0;
        bool received      = false;
        bool small_delta   = false;
        unsigned timestamp = 0;
    };

    static std::vector<pkt_fb> pkt_feedback(const struct rtcp::hdr* rtcp,
            const struct rtcp::twcc::hdr* twcc, const unsigned char* buf) {

        unsigned offset = 0, pkt_i = 0, seq = ntohs(twcc->base_seq);
        std::vector<pkt_fb> pkt_fb(ntohs(twcc->pkt_status_count));

        //TODO: check for not parsing beyond message boundary (use rtcp header length field)

        // step 1: parse status chunks
        for (auto pkts_rem = ntohs(twcc->pkt_status_count); pkts_rem > 0; offset += 2) {

            auto* chunk = (std::uint16_t*) (buf + offset);

            //TODO: deal with the two different types of status chunks
            std::uint16_t type    = (ntohs(*chunk) & 0b10000000'00000000) >> 15;
            std::uint16_t symbol  = (ntohs(*chunk) & 0b01100000'00000000) >> 13;
            std::uint16_t run_len = (ntohs(*chunk) & 0b00011111'11111111);

            pkts_rem -= run_len;

            for (auto loop_i = 0; loop_i < run_len; loop_i++, pkt_i++) {
                pkt_fb[pkt_i].seq         = seq++;

                // The status of a packet is described using a 2-bit symbol:
                // 00 - not received, 01 - received, small delta, 10 - received, large or neg. d.

                pkt_fb[pkt_i].received    = (symbol == 0x01 || symbol == 0x10);
                pkt_fb[pkt_i].small_delta = (symbol == 0x01);
            }

            // recv delta: 8 bits For each "packet received" status, in the packet
            //               status chunks, a receive delta block will follow.  See
            //               details below.
        }

        pkt_i = 0;

        // step 2: parse recv deltas for each packet that was received and has small delta
        for (auto delta_i = 0; pkt_i < pkt_fb.size(); pkt_i++) {

            if (pkt_fb[pkt_i].received && pkt_fb[pkt_i].small_delta) {

                unsigned delta = *(buf + offset) / 4;

                // abs. tstamp: if first recv delta, add ref time, then use increment from prev.
                pkt_fb[pkt_i].timestamp = delta_i == 0 ? (twcc->ref_time() * 64) + delta
                                                       : pkt_fb[pkt_i-1].timestamp + delta;

                offset++, delta_i++;
            }
        }

        return pkt_fb;
    }

    static const std::size_t HDR_LEN = 12;
}
