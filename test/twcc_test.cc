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

    auto* twcc_buf = test::data::twcc_pkt1 + rtcp::HDR_LEN;
    auto* twcc = (rtcp::twcc::hdr*) twcc_buf;

    CHECK(ntohl(twcc->source_ssrc) == 0xdd586eb6);
    CHECK(ntohs(twcc->base_seq) == 1);
    CHECK(ntohs(twcc->pkt_status_count) == 9);
    CHECK(twcc->ref_time() == 6197520);
    CHECK(twcc->fb_pkt_count() == 0);

    auto pkt_fb = rtcp::twcc::pkt_feedback(rtcp, twcc, twcc_buf + rtcp::twcc::HDR_LEN);

    CHECK(pkt_fb.size() == 9);

    CHECK(pkt_fb[0].seq == 1);
    CHECK(pkt_fb[0].received);
    CHECK(pkt_fb[0].small_delta);
    CHECK(pkt_fb[0].timestamp == 396641287); // 6197520 * 64 + 7

    CHECK(pkt_fb[1].seq == 2);
    CHECK(pkt_fb[1].received);
    CHECK(pkt_fb[1].small_delta);
    CHECK(pkt_fb[1].timestamp == 396641290); // 6197520 * 64 + 7 + 3

    CHECK(pkt_fb[7].seq == 8);
    CHECK(pkt_fb[7].received);
    CHECK(pkt_fb[7].small_delta);
    CHECK(pkt_fb[7].timestamp == 396641318); // 6197520 * 64 + 7 + 3 + 11 + 14 + 0 + 1 + 0 + 2

    CHECK(pkt_fb[8].seq == 9);
    CHECK(pkt_fb[8].received);
    CHECK(pkt_fb[8].small_delta);
    CHECK(pkt_fb[8].timestamp == 396641324); // 6197520 * 64 + 7 + 3 + 11 + 14 + 0 + 1 + 0 + 2 + 6
}
