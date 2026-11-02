#include "Game.h"

#include <iostream>
#include <limits>

// Constructor
Game::Game()
{
    gameRunning = true;

    CreateRooms();
}

// Creates the first few rooms used in the prototype
void Game::CreateRooms()
{
    Room roomOne(
        "Train Car",
        "You stand inside an empty train car. "
        "The overhead lights flicker faintly.",
        "You stand inside an empty train car. "
        "The lights pulse above you like a slow heartbeat."
    );

    Room roomTwo(
        "Platform",
        "The platform is empty. A cold draft moves through the station.",
        "The platform stretches farther than it should. "
        "Something moves beyond the edge of the light."
    );

    Room roomThree("Waiting Room","Rows of empty chairs face a cracked station clock.","Rows of chairs face you. " "For a moment, you are certain someone is sitting in one of them.");

    rooms.push_back(roomOne);
    rooms.push_back(roomTwo);
    rooms.push_back(roomThree);
}

// Runs the application main loop
void Game::Run()
{
    while (gameRunning)
    {
        DisplayMainMenu();
    }
}

// Displays the main menu
void Game::DisplayMainMenu()
{
    int choice;

    std::cout << "\n";
    std::cout << "============================\n";
    std::cout << "       THE LAST ROOM\n";
    std::cout << "============================\n";
    std::cout << "1. New Game\n";
    std::cout << "2. Instructions\n";
    std::cout << "3. Exit\n";
    std::cout << "\nEnter choice: ";

    std::cin >> choice;

    // Input validation
    if (std::cin.fail())
    {
        std::cin.clear();
        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        std::cout << "\nInvalid input. Enter a number.\n";
        return;
    }

    if (choice == 1)
    {
        StartNewGame();
    }
    else if (choice == 2)
    {
        std::cout << "\n";
        std::cout << "Explore the environment and make choices.\n";
        std::cout << "Your actions may affect Matt's sanity.\n";
        std::cout << "The world may not always appear the same.\n";
    }
    else if (choice == 3)
    {
        gameRunning = false;

        std::cout << "\nExiting game...\n";
    }
    else
    {
        std::cout << "\nPlease enter a valid menu option.\n";
    }
}

// Starts a new game
void Game::StartNewGame()
{
    // Reset the player to a fresh starting state
    player = Player();

    bool playing = true;

    std::cout << "\n";
    std::cout << "You wake to the sound of metal grinding against metal.\n";
    std::cout << "For a moment, you cannot remember where you are.\n";

    while (playing)
    {
        DisplayGameMenu();

        int choice;

        std::cout << "\nEnter choice: ";
        std::cin >> choice;

        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(),
                '\n'
            );

            std::cout << "\nInvalid input.\n";
        }
        else if (choice == 1)
        {
            ExploreRoom();
        }
        else if (choice == 2)
        {
            MovePlayer();
        }
        else if (choice == 3)
        {
            std::cout << "\nSanity: "
                << player.GetSanity()
                << "/100\n";
        }
        else if (choice == 4)
        {
            playing = false;
        }
        else
        {
            std::cout << "\nPlease enter a valid option.\n";
        }
    }
}

// Displays the gameplay menu
void Game::DisplayGameMenu()
{
    int roomIndex = player.GetCurrentRoom();

    std::cout << "\n";
    std::cout << "----------------------------\n";
    std::cout << rooms[roomIndex].GetName() << "\n";
    std::cout << "----------------------------\n";

    std::cout << rooms[roomIndex].GetDescription(
        player.GetSanity()
    );

    std::cout << "\n\n";

    std::cout << "1. Examine surroundings\n";
    std::cout << "2. Move\n";
    std::cout << "3. Check sanity\n";
    std::cout << "4. Return to main menu\n";
}

// Allows the player to examine the current room
void Game::ExploreRoom()
{
    int roomIndex = player.GetCurrentRoom();

    if (roomIndex == 0)
    {
        std::cout << "\n";
        std::cout << "You notice dark stains near the end of the train car.\n";
        std::cout << "You stare at them longer than you should.\n";

        player.ChangeSanity(-10);

        std::cout << "\nSanity decreased.\n";
    }
    else if (roomIndex == 1)
    {
        std::cout << "\n";
        std::cout << "You hear footsteps somewhere behind you.\n";
        std::cout << "When you turn around, the platform is empty.\n";

        player.ChangeSanity(-5);

        std::cout << "\nSanity decreased.\n";
    }
    else if (roomIndex == 2)
    {
        std::cout << "\n";
        std::cout << "The station clock has stopped.\n";
        std::cout << "The hands point to a time you recognize, "
            << "but you cannot remember why.\n";

        player.ChangeSanity(-5);

        std::cout << "\nSanity decreased.\n";
    }
}

// Allows basic movement between prototype rooms
void Game::MovePlayer()
{
    int choice;

    std::cout << "\nWhere would you like to go?\n";
    std::cout << "1. Train Car\n";
    std::cout << "2. Platform\n";
    std::cout << "3. Waiting Room\n";

    std::cout << "\nEnter choice: ";
    std::cin >> choice;

    if (std::cin.fail())
    {
        std::cin.clear();
        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        std::cout << "\nInvalid input.\n";
        return;
    }

    if (choice >= 1 && choice <= 3)
    {
        player.SetCurrentRoom(choice - 1);

        std::cout << "\nYou move carefully through the station.\n";
    }
    else
    {
        std::cout << "\nThat location is not available.\n";
    }
}