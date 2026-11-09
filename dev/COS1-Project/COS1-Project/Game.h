#pragma once

#include <vector>

#include "Player.h"
#include "Room.h"
#include "Item.h"

class Game
{
private:

    // Main player object
    Player player;

    // Stores all rooms in the current prototype
    std::vector<Room> rooms;

    // Controls whether the entire application continues running
    bool gameRunning;

    // Creates the prototype rooms and their items
    void CreateRooms();

    // Displays the title/main menu
    void DisplayMainMenu();

    // Displays the menu used while playing
    void DisplayGameMenu();

    // Starts a fresh game session
    void StartNewGame();

    // Allows Matt to examine his current location
    void ExploreRoom();

    // Allows Matt to collect an item from the current room
    void CollectItem();

    // Allows Matt to move between prototype locations
    void MovePlayer();

    // Displays Matt's current sanity info
    void DisplaySanity();

    // Handles integer input validation
    int GetValidatedInput(int minimum, int maximum);

public:

    // Game constructor
    Game();

    // Starts and controls the application
    void Run();
};