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


    // WEEK 3: Tracks the state of each room's memory event.
    //
    // 0 = Memory has not been confronted or avoided.
    // 1 = Memory was avoided at least once.
    // 2 = Memory has been confronted and resolved.
    //
    // Each index corresponds to a room in the rooms vector.
    std::vector<int> memoryStates;



    // WEEK 3: Runs the psychological memory event
    // associated with Matt's current room.
    void HandleMemoryEvent();


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
    void DisplaySanity() const;

    // Handles integer input validation
    int GetValidatedInput(int minimum, int maximum);


    // WEEK 4: Runs the active gameplay menu.
    // Used after starting a new game or loading one.
    void PlayGame();

    // WEEK 4: Writes the current game state to a file.
    // Returns true if the file was saved successfully.
    bool SaveGame();

    // WEEK 4: Reads a previously saved game.
    // Returns true if valid progress was restored.
    bool LoadGame();


public:

    // Game constructor
    Game();

    // Starts and controls the application
    void Run();
};