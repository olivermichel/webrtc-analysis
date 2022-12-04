#include <catch.h>
#include <arpa/inet.h>

#include <iostream>

#include "twcc.h"
#include "rtcp.h"
#include "data.h"

TEST_CASE("rtcp::twcc::hdr", "[twcc]") {

    auto* rtcp = (rtcp::hdr*) test::data::twcc_pkt1;

    CHECK(rtcp->version() == 2);
    CHECK(rtcp->padding() == 1);
    CHECK(rtcp->fmt() == 15);
    CHECK(rtcp->pt == 205);
    CHECK(ntohs(rtcp->len) == 7);
    CHECK(ntohl(rtcp->ssrc) == 0x00000001);

    auto* twcc = (rtcp::twcc::hdr*) (test::data::twcc_pkt1 + rtcp::HDR_LEN);

    CHECK(ntohl(twcc->source_ssrc) == 0xdd586eb6);
    CHECK(ntohs(twcc->base_seq) == 1);
    CHECK(ntohs(twcc->pkt_status_count) == 9);
    CHECK(twcc->ref_time() == 6197520);
    CHECK(twcc->fb_pkt_count() == 0);

    auto pkts_remaining = ntohs(twcc->pkt_status_count);
    auto offset = 0;

    while (pkts_remaining > 0) {

        auto* chunk = (std::uint16_t*) (test::data::twcc_pkt1 + rtcp::HDR_LEN + rtcp::twcc::HDR_LEN + offset);

        std::uint16_t type    = (ntohs(*chunk) & 0b10000000'00000000) >> 15;
        std::uint16_t symbol  = (ntohs(*chunk) & 0b01100000'00000000) >> 13;
        std::uint16_t run_len = (ntohs(*chunk) & 0b00011111'11111111);

        pkts_remaining -= run_len;
        offset += 2;

        // parse associated recv delta here instead of in second loop if packet status is received
        //  - use different offsets for recv delta and status chunks

        // recv delta: 8 bits For each "packet received" status, in the packet
        //               status chunks, a receive delta block will follow.  See
        //               details below.

        // The status of a packet is described using a 2-bit symbol:
        //   00 Packet not received
        //   01 Packet received, small delta
        //   10 Packet received, large or negative delta
        //   11 [Reserved]
    }

    for (pkts_remaining = ntohs(twcc->pkt_status_count); pkts_remaining > 0; offset++, pkts_remaining--) {

        auto delta = (std::uint8_t*) (test::data::twcc_pkt1 + rtcp::HDR_LEN + rtcp::twcc::HDR_LEN + offset);
        std::cout << (unsigned) *delta / 4 << std::endl;
    }

}
