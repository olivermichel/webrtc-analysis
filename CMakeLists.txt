cmake_minimum_required(VERSION 3.23)
project(webrtc_analysis)

set(CMAKE_CXX_STANDARD 17)

include(cmake/pcap.cmake)

add_executable(webrtc_analysis src/cmd/rtp_analyzer_main.cc
        src/lib/net.h
        src/lib/pcap_file_reader.h src/lib/pcap_file_reader.cc
        src/lib/pcap_util.h
        src/lib/rtcp.h
        src/lib/rtp.h
        src/lib/twcc.h
        src/lib/stun.h)

target_include_directories(webrtc_analysis PUBLIC ${PCAP_INCLUDE_DIRS})
target_link_libraries(webrtc_analysis ${PCAP_LIBRARIES})
set_target_properties(webrtc_analysis PROPERTIES LINKER_LANGUAGE CXX)
