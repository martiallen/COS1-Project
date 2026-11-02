#pragma once

#include <vector>

#include "Player.h"
#include "Room.h"
#include "Item.h"

class Game
{
private:
    Player player;

    std::vector<Room> rooms;

    bool gameRunning;

    void CreateRooms();

    void DisplayMainMenu();
    void DisplayGameMenu();

    void StartNewGame();
    void ExploreRoom();
    void MovePlayer();

public:
    Game();

    void Run();
};
