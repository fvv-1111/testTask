cmake_minimum_required(VERSION 3.5.0)
project(Server VERSION 0.1.0 LANGUAGES CXX)


set(CPP_FILES   ServerMain.cpp
                Server.cpp
                Connections.cpp
                Connection.cpp
                ChannelSimulator/Channel.cpp
                ChannelSimulator/ChannelsSet.cpp
                ChannelSimulator/ChannelValueGenerator.cpp
                ServerCommandProcessor/GetResultCommand.cpp
                ServerCommandProcessor/GetStatusCommand.cpp
                ServerCommandProcessor/SetRangeCommand.cpp
                ServerCommandProcessor/GetRangeCommand.cpp
                ServerCommandProcessor/StartMeasureCommand.cpp
                ServerCommandProcessor/StopMeasureCommand.cpp
                ../Utils/Logger.cpp
                ../Utils/ChannelEnums.cpp
                ../Utils/ErrorCodeDefinition.cpp
                ../Utils/SocketHandle.cpp
                ../Utils/SocketMessage.cpp
                )


add_executable(${PROJECT_NAME} ${CPP_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE ConfigSet)

#set(GCC_COMPILE_OPTIONS "-std=c++17;-Wall;-pthread;-Wno-deprecated;")
#set(GCC_COMPILE_DEBUG_OPTIONS "${GCC_COMPILE_OPTIONS};-g;-O0")
#set(GCC_COMPILE_RELEASE_OPTIONS "${GCC_COMPILE_OPTIONS};-O3")
#target_compile_options(${PROJECT_NAME} PUBLIC "$<$<CONFIG:Debug>:${GCC_COMPILE_DEBUG_OPTIONS}>")
#target_compile_options(${PROJECT_NAME} PUBLIC "$<$<CONFIG:Release>:${GCC_COMPILE_RELEASE_OPTIONS}>")
