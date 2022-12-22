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

TEST_CASE("util::extract_non_symmetric", "[util]") {

    std::vector<bool> v1 = { false, false };
    std::vector<bool> v2 = { false, true };
    std::vector<bool> v3 = { true, false };
    std::vector<bool> v4 = { true, true, false };
    std::vector<bool> v5 = { true, true, true };

    CHECK(util::extract_non_symmetric(v1, 0, 5) == std::pair<std::uint32_t, std::uint32_t>{0b000, 2});
    CHECK(util::extract_non_symmetric(v2, 0, 5) == std::pair<std::uint32_t, std::uint32_t>{0b001, 2});
    CHECK(util::extract_non_symmetric(v3, 0, 5) == std::pair<std::uint32_t, std::uint32_t>{0b010, 2});
    CHECK(util::extract_non_symmetric(v4, 0, 5) == std::pair<std::uint32_t, std::uint32_t>{0b011, 3});
    CHECK(util::extract_non_symmetric(v5, 0, 5) == std::pair<std::uint32_t, std::uint32_t>{0b100, 3});
}