
#include <catch.h>

#include "rtp.h"
#include "data.h"

auto* rtp1 = (rtp::hdr*) test::data::rtp_pkt1;
auto* rtp2 = (rtp::hdr*) test::data::rtp_pkt2;
auto* rtp3 = (rtp::hdr*) test::data::rtp_pkt3;

TEST_CASE("rtp::total_ext_len", "[rtp]") {

    CHECK(rtp::total_ext_len(rtp1) == 12);
    CHECK(rtp::total_ext_len(rtp2) == 32);
}

TEST_CASE("rtp::ext_profile", "[rtp]") {

    CHECK(rtp::ext_profile(rtp1) == 0xbede);
    CHECK(rtp::ext_profile(rtp2) == 0xbede);
    CHECK(rtp::ext_profile(rtp3) == 0x0100);
}

TEST_CASE("rtp::has_one_byte_ext_headers", "[rtp]") {

    CHECK(rtp::has_one_byte_ext_headers(rtp1));
    CHECK(rtp::has_one_byte_ext_headers(rtp2));
    CHECK_FALSE(rtp::has_one_byte_ext_headers(rtp3));
}

TEST_CASE("rtp::has_two_byte_ext_headers", "[rtp]") {

    CHECK_FALSE(rtp::has_two_byte_ext_headers(rtp1));
    CHECK_FALSE(rtp::has_two_byte_ext_headers(rtp2));
    CHECK(rtp::has_two_byte_ext_headers(rtp3));
}

TEST_CASE("rtp::get_ext", "[rtp]") {

    SECTION("return std::nullopt for non-existing extensions") {

        CHECK(rtp::get_ext(rtp2, 1) == std::nullopt);
        CHECK(rtp::get_ext(rtp2, 12) == std::nullopt);
        CHECK(rtp::get_ext(rtp2, 8) == std::nullopt);
        CHECK(rtp::get_ext(rtp3, 12) == std::nullopt);
        CHECK(rtp::get_ext(rtp3, 8) == std::nullopt);
    }

    SECTION("returns one-byte header extensions") {

        auto ext2 = rtp::get_ext(rtp2, 2);
        auto ext4 = rtp::get_ext(rtp2, 4);
        auto ext9 = rtp::get_ext(rtp2, 9);
        auto ext3 = rtp::get_ext(rtp2, 3);
        auto ext6 = rtp::get_ext(rtp2, 6);
        auto ext7 = rtp::get_ext(rtp2, 7);
        auto ext13 = rtp::get_ext(rtp2, 13);

        CHECK(ext2 != std::nullopt);
        CHECK(ext4 != std::nullopt);
        CHECK(ext9 != std::nullopt);
        CHECK(ext3 != std::nullopt);
        CHECK(ext6 != std::nullopt);
        CHECK(ext7 != std::nullopt);
        CHECK(ext13 != std::nullopt);

        CHECK(ext2->len == 3);
        CHECK(ext2->data[0] == 0xea);
        CHECK(ext2->data[1] == 0x43);
        CHECK(ext2->data[2] == 0x2b);
        CHECK(ext4->len == 2);
        CHECK(ext4->data[0] == 0x00);
        CHECK(ext4->data[1] == 0x04);
        CHECK(ext9->len == 1);
        CHECK(ext3->len == 1);
        CHECK(ext6->len == 1);
        CHECK(ext7->len == 13);
        CHECK(ext13->len == 3);
        CHECK(ext13->data[0] == 0x40);
        CHECK(ext13->data[1] == 0x00);
        CHECK(ext13->data[2] == 0x01);
    }

    SECTION("returns two-byte header extensions") {

        auto ext2 = rtp::get_ext(rtp3, 2);
        auto ext4 = rtp::get_ext(rtp3, 4);
        auto ext9 = rtp::get_ext(rtp3, 9);
        auto ext3 = rtp::get_ext(rtp3, 3);
        auto ext6 = rtp::get_ext(rtp3, 6);
        auto ext7 = rtp::get_ext(rtp3, 7);
        auto ext13 = rtp::get_ext(rtp3, 13);

        CHECK(ext2 != std::nullopt);
        CHECK(ext4 != std::nullopt);
        CHECK(ext9 != std::nullopt);
        CHECK(ext3 != std::nullopt);
        CHECK(ext6 != std::nullopt);
        CHECK(ext7 != std::nullopt);
        CHECK(ext13 != std::nullopt);

        CHECK(ext2->len == 3);
        CHECK(ext4->len == 2);
        CHECK(ext9->len == 1);
        CHECK(ext3->len == 1);
        CHECK(ext6->len == 1);
        CHECK(ext7->len == 13);
        CHECK(ext13->len == 33);
        CHECK(ext13->data[0] == 0xc0);
        CHECK(ext13->data[1] == 0x00);
        CHECK(ext13->data[2] == 0x01);
        CHECK(ext13->data[32] == 0xf0);
    }
}