
set(WEBRTC_ANALYSIS_SRC
        ../src/lib/twcc.h)

set(WEBRTC_ANALYSIS_TEST_UNIT_SRC
        twcc_test.cc)

add_executable(unit unit_main.cc
        ${WEBRTC_ANALYSIS_SRC}
        ${WEBRTC_ANALYSIS_TEST_UNIT_SRC})

target_include_directories(unit PRIVATE ${PROJECT_SOURCE_DIR}/ext/include)
target_include_directories(unit PRIVATE ${PROJECT_SOURCE_DIR}/src/lib)
target_include_directories(unit PRIVATE ${PROJECT_SOURCE_DIR}/test/include)

add_test(NAME unit COMMAND unit WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
