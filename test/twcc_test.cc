#include <catch.h>
#include <arpa/inet.h>

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


}
