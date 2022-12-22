cmake_minimum_required(VERSION 3.23)
project(webrtc-analysis)

set(CMAKE_CXX_STANDARD 17)

include(cmake/pcap.cmake)
include(cmake/catch2.cmake)
include(cmake/cxxopts.cmake)

add_executable(rtp_analyzer src/cmd/rtp_analyzer_main.cc
        src/lib/av1.h src/lib/av1.cc
        src/lib/gcc_sim.h
        src/lib/net.h
        src/lib/pcap_file_reader.h src/lib/pcap_file_reader.cc
        src/lib/pcap_util.h
        src/lib/rtcp.h
        src/lib/rtp.h
        src/lib/twcc.h
        src/lib/stun.h)

target_include_directories(rtp_analyzer PUBLIC ${PCAP_INCLUDE_DIRS})
target_include_directories(rtp_analyzer PUBLIC ext/include)
target_link_libraries(rtp_analyzer ${PCAP_LIBRARIES})
set_target_properties(rtp_analyzer PROPERTIES LINKER_LANGUAGE CXX)

add_subdirectory(test)
