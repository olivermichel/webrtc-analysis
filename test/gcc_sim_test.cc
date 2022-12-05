#include <catch.h>
#include "gcc_sim.h"

TEST_CASE("gcc_sim", "[gcc_sim]") {

    gcc_sim gcc;

    gcc.add_media_pkt(1, 1);
    gcc.add_media_pkt(2, 3);
    gcc.add_media_pkt(3, 3);
    gcc.add_media_pkt(4, 4);
    gcc.add_media_pkt(5, 6);
    gcc.add_twcc_fb(1, true, 5);
    gcc.add_twcc_fb(2, true, 7);
    gcc.add_twcc_fb(3, true, 8);
    gcc.add_twcc_fb(4, true, 8);
    gcc.add_media_pkt(6, 7);
    gcc.add_media_pkt(7, 9);
    gcc.add_media_pkt(8, 9);
    gcc.add_twcc_fb(5, true, 10);
    gcc.add_twcc_fb(6, true, 12);
    gcc.add_twcc_fb(7, true, 15);
    gcc.add_twcc_fb(8, true, 15);
}