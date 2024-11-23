cmake_minimum_required(VERSION 3.5.0)
project(Client VERSION 0.1.0 LANGUAGES CXX)

find_package(Qt6 COMPONENTS Widgets Core REQUIRED)

qt_standard_project_setup()

set(CPP_FILES   ClientConnection.cpp
                ClientCommandProcessor/GetRangeCommand.cpp
                ClientCommandProcessor/SetRangeCommand.cpp
                ClientCommandProcessor/GetStatusCommand.cpp
                ClientCommandProcessor/GetRangeCommand.cpp
                ClientCommandProcessor/GetResultCommand.cpp
                ClientCommandProcessor/StopMeasureCommand.cpp
                ClientCommandProcessor/StartMeasureCommand.cpp
                ClientCommandProcessor/CommandFactory.cpp
                ../Utils/Logger.cpp
                ../Utils/ChannelEnums.cpp
                ../Utils/ErrorCodeDefinition.cpp
                ../Utils/SocketHandle.cpp
                ../Utils/SocketMessage.cpp
                )

set(QT_FILES    ClientMain.cpp
                ClientApp.cpp
                MVC/Controller/ViewController.cpp
                MVC/DataModel/DataModel.cpp
                MVC/DataModel/ChannelData.cpp
                MVC/Views/StatusView.cpp
                MVC/Views/ChannelView.cpp
                MVC/Views/MainView.cpp
                ClientCommandProcessor/ResponceEvent.cpp
                ClientCommandProcessor/ServerExecutor.cpp
                ClientCommandProcessor/GetResultsProcess.cpp

)


qt_add_executable(${PROJECT_NAME} ${QT_FILES} ${CPP_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE Qt::Widgets Qt::Core ConfigSet)
