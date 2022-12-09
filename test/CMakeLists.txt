
set(WEBRTC_ANALYSIS_SRC
        ../src/lib/av1.h ../src/lib/av1.cc
        ../src/lib/gcc_sim.h
        ../src/lib/util.h
        ../src/lib/twcc.h)

set(WEBRTC_ANALYSIS_TEST_UNIT_SRC
        av1_test.cc
        gcc_sim_test.cc
        ntp_test.cc
        twcc_test.cc
        util_test.cc)

add_executable(unit unit_main.cc
        ${WEBRTC_ANALYSIS_SRC}
        ${WEBRTC_ANALYSIS_TEST_UNIT_SRC})

target_include_directories(unit PRIVATE ${PROJECT_SOURCE_DIR}/ext/include)
target_include_directories(unit PRIVATE ${PROJECT_SOURCE_DIR}/src/lib)
target_include_directories(unit PRIVATE ${PROJECT_SOURCE_DIR}/test/include)

add_test(NAME unit COMMAND unit WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
