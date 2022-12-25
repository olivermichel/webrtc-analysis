
#include <catch.h>

#include "rtp.h"
#include "data.h"

TEST_CASE("rtp::total_ext_len", "[rtp]") {

    auto* rtp1 = (rtp::hdr*) test::data::rtp_pkt1;
    CHECK(rtp::total_ext_len(rtp1) == 12);

    auto* rtp2 = (rtp::hdr*) test::data::rtp_pkt2;
    CHECK(rtp::total_ext_len(rtp2) == 32);
}
