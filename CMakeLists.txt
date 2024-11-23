cmake_minimum_required(VERSION 3.5.0)
project(SocketApp VERSION 0.1.0 LANGUAGES C CXX)

add_compile_definitions(NUM_CHANNELS=8)

add_library(ConfigSet INTERFACE)

target_compile_options(ConfigSet INTERFACE 
                                    -std=c++17
                                    -Wall
                                    -pthread
                                    -Wno-deprecated
                                    $<$<CONFIG:Debug>:-g;-O0>
                                    $<$<CONFIG:Release>:-O3>
)


add_subdirectory(ServerApp)

add_subdirectory(ClientApp)

install(TARGETS Server Client DESTINATION .)

