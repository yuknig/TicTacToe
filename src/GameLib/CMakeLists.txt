set (GameLib_headers
    Board.h
    GameControl.h
    GameModel.h
    GameTypes.h
    GeomTypes.h
    IGameView.h
    IPlayerController.h
    IStatusMessage.h
    ComputerPlayerMove.h
    PlayerControllers.h
    Util.h
)

set (GameLib_sources
    Board.cpp
    GameControl.cpp
    GameModel.cpp
    ComputerPlayerMove.cpp
    PlayerControllers.cpp
)

add_library(GameLib ${GameLib_headers} ${GameLib_sources})
