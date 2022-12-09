#include <catch.h>
#include "av1.h"

TEST_CASE("av1", "[av1]") {

    const unsigned dd1_len = 13;
    const unsigned char dd1_bytes[] = {0x80, 0x00, 0x01, 0x80,
                                       0x00, 0x3a, 0x41, 0x01,
                                       0x80, 0x9f, 0x80, 0x77,
                                       0x80};

    av1::dependency_descriptor dd1{dd1_bytes, dd1_len};

    CHECK(dd1.mandatory_fields().start_of_frame());
    CHECK_FALSE(dd1.mandatory_fields().end_of_frame());
    CHECK(dd1.mandatory_fields().template_id() == 0);
    CHECK(dd1.mandatory_fields().frame_number() == 1);

    CHECK(dd1.template_dependency_structure_present_flag());
    CHECK_FALSE(dd1.active_decode_targets_present_flag());
    CHECK_FALSE(dd1.custom_dtis_flag());
    CHECK_FALSE(dd1.custom_fdiffs_flag());
    CHECK_FALSE(dd1.custom_chains_flag());



}
