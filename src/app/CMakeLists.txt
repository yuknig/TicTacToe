cmake_minimum_required(VERSION 3.16)
project(TicTacToe)
set (TicTacToe_Headers
    Application.h
    BoardWidget.h
    MainWindow.h
    NewGameDialog.h
    StatusBar.h
    Svgs.h
)

set (TicTacToe_Sources
    Application.cpp
    BoardWidget.cpp
    main.cpp
    MainWindow.cpp
    NewGameDialog.cpp
    StatusBar.cpp
    Svgs.cpp
)

set (TicTacToe_Libraries
    GameLib
    Qt6::Widgets
    Qt6::Svg
)

find_package(Qt6 REQUIRED COMPONENTS Widgets Svg)
qt_standard_project_setup()

qt_add_executable(TicTacToe ${TicTacToe_Sources} ${TicTacToe_Headers})
target_link_libraries(TicTacToe PRIVATE ${TicTacToe_Libraries})

if(MSVC)
    add_custom_command(TARGET TicTacToe POST_BUILD
                       COMMAND "${Qt6_DIR}/../../../bin/windeployqt.exe" ARGS "$(TargetPath)"
                       COMMENT "Copying Qt dependencies")
endif()
