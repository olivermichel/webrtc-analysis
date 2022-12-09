#include <catch.h>

#include <stdexcept>

#include "util.h"

TEST_CASE("util::extract_bits", "[util]") {

    // 10'1011'1011, len = 10
    std::vector<bool> v1 = { true, false, true, false, true, true, true, false, true, true };

    SECTION("throws when given an invalid range") {

        CHECK_THROWS_AS(util::extract_bits(v1, 0, 14), std::invalid_argument);
        CHECK_THROWS_AS(util::extract_bits(v1, 8, 4), std::invalid_argument);
        CHECK_THROWS_AS(util::extract_bits(v1, 9, 2), std::invalid_argument);
        CHECK_THROWS_AS(util::extract_bits(v1, 0, 0), std::invalid_argument);
        CHECK_THROWS_AS(util::extract_bits(v1, 0, 33), std::invalid_argument);
        CHECK_NOTHROW(util::extract_bits(v1, 9, 1));
        CHECK_NOTHROW(util::extract_bits(v1, 8, 2));
    }

    SECTION("extracts a bit range") {

        CHECK(util::extract_bits(v1, 0) == 0b1);
        CHECK(util::extract_bits(v1, 0, 10) == 0b10'1011'1011);
        CHECK(util::extract_bits(v1, 4, 4) == 0b1110);

    }

}
