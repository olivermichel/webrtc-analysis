#include <catch.h>
#include "ntp.h"

TEST_CASE("ntp: middle_bits_from_ntp", "[ntp]") {

    CHECK(ntp::middle_bits_from_ntp(0xb44db70520000000) == 0xb7052000);
}