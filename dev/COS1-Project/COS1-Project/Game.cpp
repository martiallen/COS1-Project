#include "Game.h"

#include <iostream>
#include <limits>

// Game constructor
//
// The game begins in a running state
// and the prototype rooms are created
Game::Game()
{
    gameRunning = true;

    CreateRooms();
}

// Creates all rooms and items currently used in the prototype
//
// Week 2 still uses only three rooms
// We are adding depth instead of adding a huge map
void Game::CreateRooms()
{
    // Create the train car
    Room trainCar(
        "Train Car",

        // Stable description
        "You stand inside an empty train car. "
        "The overhead lights flicker faintly.",

        // Uneasy description
        "The train car is empty, but you cannot shake "
        "the feeling that someone was sitting behind you.",

        // Unstable description
        "The windows reflect seats that do not appear "
        "to exist inside the train.",

        // Fractured description
        "Every seat is occupied.\n"
        "None of the passengers have faces.\n"
        "You blink.\n"
        "The train is empty again."
    );

    // Create the platform.
    Room platform(
        "Platform",

        // Stable description
        "The platform is empty. "
        "A cold draft moves through the station.",

        // Uneasy description
        "The platform is empty, but footsteps echo "
        "somewhere beyond the stairs.",

        // Unstable description
        "The platform stretches farther than it should. "
        "Something moves beyond the edge of the light.",

        // Fractured description
        "The platform has no end.\n"
        "Red light spills across the concrete.\n"
        "Something is walking toward you."
    );

    // Create the waiting room
    Room waitingRoom(
        "Waiting Room",

        // Stable description
        "Rows of empty chairs face a cracked station clock.",

        // Uneasy description
        "The station clock has stopped. "
        "You recognize the time, but you do not know why.",

        // Unstable description
        "The chairs seem closer together than before. "
        "One of them is still warm.",

        // Fractured description
        "Someone is sitting in every chair.\n"
        "Every face is yours."
    );

    // Create Week 2 items.
    Item trainTicket(
        "Train Ticket",
        "A worn train ticket. "
        "The date has been scratched away."
    );

    Item photograph(
        "Family Photograph",
        "A photograph of Matt with his family. "
        "The corner containing his face has been torn."
    );

    Item dogTags(
        "Dog Tags",
        "A pair of military dog tags. "
        "Matt recognizes them immediately."
    );

    // Place one item into each room
    trainCar.AddItem(trainTicket);
    platform.AddItem(dogTags);
    waitingRoom.AddItem(photograph);

    // Store all rooms inside the game's room vector
    rooms.push_back(trainCar);
    rooms.push_back(platform);
    rooms.push_back(waitingRoom);
}

// Starts the application's main loop
void Game::Run()
{
    // Keep displaying the main menu until the user exits
    while (gameRunning)
    {
        DisplayMainMenu();
    }
}

// Displays the application main menu
void Game::DisplayMainMenu()
{
    std::cout << "\n";
    std::cout << "============================\n";
    std::cout << "       THE LAST ROOM\n";
    std::cout << "============================\n";
    std::cout << "1. New Game\n";
    std::cout << "2. Instructions\n";
    std::cout << "3. Exit\n";
    std::cout << "============================\n";
    std::cout << "Enter choice: ";

    // Use the reusable input validation method
    int choice = GetValidatedInput(1, 3);

    // Start a new game
    if (choice == 1)
    {
        StartNewGame();
    }

    // Display basic instructions
    else if (choice == 2)
    {
        std::cout << "\n";
        std::cout << "Explore the environment and examine your surroundings.\n";
        std::cout << "You may discover objects connected to Matt's past.\n";
        std::cout << "Your decisions and experiences can affect sanity.\n";
        std::cout << "As sanity changes, Matt's perception may also change.\n";
    }

    // End the application
    else if (choice == 3)
    {
        gameRunning = false;

        std::cout << "\nExiting game...\n";
    }
}


void Game::StartNewGame()
{
    // Reset Matt to his starting values.
    // This restores sanity, location, and clears inventory
    // and memories through the Player constructor.
    player = Player();

    // Clear the previous room objects.
    // This removes their collected-item and examined states.
    rooms.clear();

    // Rebuild the original rooms and collectible items.
    CreateRooms();

    // Clear all previous memory event progress.
    memoryStates.clear();

    // Give every room an unresolved memory state.
    for (int i = 0; i < rooms.size(); i++)
    {
        memoryStates.push_back(0);
    }

    // Controls whether the player remains in gameplay.
    bool playing = true;

    // Display the opening scene.
    std::cout << "\n";
    std::cout << "====================================\n";
    std::cout << "You wake to the sound of metal\n";
    std::cout << "grinding against metal.\n";
    std::cout << "\n";
    std::cout << "For a moment, you cannot remember\n";
    std::cout << "where you are.\n";
    std::cout << "====================================\n";

    // Continue gameplay until the player returns
    // to the main menu.
    while (playing)
    {
        // Display the current room and menu choices.
        DisplayGameMenu();

        // Week 3 adds two new gameplay menu choices.
        int choice = GetValidatedInput(1, 8);

        if (choice == 1)
        {
            // Examine the current room.
            ExploreRoom();
        }
        else if (choice == 2)
        {
            // Collect an item from the current room.
            CollectItem();
        }
        else if (choice == 3)
        {
            // Move to another room.
            MovePlayer();
        }
        else if (choice == 4)
        {
            // Display physical inventory.
            player.DisplayInventory();
        }
        else if (choice == 5)
        {
            // Display sanity and psychological state.
            DisplaySanity();
        }
        else if (choice == 6)
        {
            // WEEK 3: Display recovered memories.
            player.DisplayMemories();
        }
        else if (choice == 7)
        {
            // WEEK 3: Confront or avoid a memory.
            HandleMemoryEvent();
        }
        else if (choice == 8)
        {
            // Return to the main menu.
            // Unsaved progress is discarded in Week 3.
            playing = false;
        }
    }
}



void Game::DisplayGameMenu()
{
    // Get the index of Matt's current location.
    int roomIndex = player.GetCurrentRoom();

    std::cout << "\n";
    std::cout << "----------------------------\n";
    std::cout << rooms[roomIndex].GetName() << "\n";
    std::cout << "----------------------------\n";

    // Use Matt's sanity to determine the room description.
    std::cout << rooms[roomIndex].GetDescription(
        player.GetSanity()
    );

    std::cout << "\n\n";

    // Week 3 expanded gameplay menu.
    std::cout << "1. Examine surroundings\n";
    std::cout << "2. Collect item\n";
    std::cout << "3. Move\n";
    std::cout << "4. View inventory\n";
    std::cout << "5. Check sanity\n";
    std::cout << "6. View memory journal\n";
    std::cout << "7. Investigate memory\n";
    std::cout << "8. Return to main menu\n";

    std::cout << "\nEnter choice: ";
}

// Allows Matt to investigate his current room
//
// The first examination affects sanity
// Repeatedly examining the same room does not continue
// removing sanity
void Game::ExploreRoom()
{
    // Determine Matt current room
    int roomIndex = player.GetCurrentRoom();

    // Get the current room examined state
    bool alreadyExamined =
        rooms[roomIndex].GetExamined();

    // If this room was already examined
    // do not repeat the full sanity event
    if (alreadyExamined)
    {
        std::cout << "\n";
        std::cout << "You search the area again.\n";
        std::cout << "Nothing new immediately stands out.\n";

        return;
    }

    // Train Car event.
    if (roomIndex == 0)
    {
        std::cout << "\n";
        std::cout << "Dark stains mark the floor near the end of the car.\n";
        std::cout << "For a moment, you remember screaming.\n";
        std::cout << "Then the memory disappears.\n";

        // The experience reduces Matt sanity
        player.ChangeSanity(-10);

        std::cout << "\nSanity decreased by 10.\n";
    }

    // Platform event.
    else if (roomIndex == 1)
    {
        std::cout << "\n";
        std::cout << "Footsteps echo behind you.\n";
        std::cout << "You turn around.\n";
        std::cout << "The platform is empty.\n";

        // The experience reduces Matt sanity
        player.ChangeSanity(-8);

        std::cout << "\nSanity decreased by 8.\n";
    }

    // Waiting Room event.
    else if (roomIndex == 2)
    {
        std::cout << "\n";
        std::cout << "The station clock has stopped.\n";
        std::cout << "The hands point to a time you recognize.\n";
        std::cout << "You cannot remember why it matters.\n";

        // The experience reduces Matt sanity
        player.ChangeSanity(-7);

        std::cout << "\nSanity decreased by 7.\n";
    }

    // Mark the room as examined so the event
    // does not repeatedly reduce sanity
    rooms[roomIndex].SetExamined(true);
}

// Allows Matt to collect an item from his current room
void Game::CollectItem()
{
    // Determine Matt's current room
    int roomIndex = player.GetCurrentRoom();

    // Check whether this room still contains any items
    if (rooms[roomIndex].GetItemCount() == 0)
    {
        std::cout << "\n"; std::cout << "There is nothing here to collect.\n";

        return;
    }

    // For Week 2 each prototype room contains only one item
    Item foundItem = rooms[roomIndex].GetItem(0);

    std::cout << "\n"; std::cout << "You found: " << foundItem.GetName() << "\n";

    std::cout << foundItem.GetDescription()<< "\n";

    std::cout << "\n";
    std::cout << "Take this item?\n";
    std::cout << "1. Yes\n";
    std::cout << "2. No\n";
    std::cout << "Enter choice: ";

    // Validate the Yes/No input.
    int choice = GetValidatedInput(1, 2);

    // Add the item to Matt inventory
    if (choice == 1)
    {
        player.AddItem(foundItem);

        // Remove the item from the room so it cannot
        // be collected more than once
        rooms[roomIndex].RemoveItem(0);

        std::cout << "\n";
        std::cout << foundItem.GetName()
            << " added to inventory.\n";
    }
    else
    {
        std::cout << "\n";
        std::cout << "You leave it where you found it.\n";
    }
}

// Allows Matt to move between the current prototype rooms
void Game::MovePlayer()
{
    std::cout << "\n";
    std::cout << "Where would you like to go?\n";
    std::cout << "1. Train Car\n";
    std::cout << "2. Platform\n";
    std::cout << "3. Waiting Room\n";
    std::cout << "Enter choice: ";

    // Validate room selection
    int choice = GetValidatedInput(1, 3);

    // Menu choices begin at 1,
// but vector indexes begin at 0
    int newRoomIndex = choice - 1;

    // Check whether Matt selected the room he is already inside
    if (newRoomIndex == player.GetCurrentRoom())
    {
        std::cout << "\n";
        std::cout << "You are already here.\n";

        return;
    }

    // Update Matt current room
    player.SetCurrentRoom(newRoomIndex);

    std::cout << "\n";
    std::cout << "You move carefully through the station.\n";
}

// Displays Matt numerical sanity and current sanity state
void Game::DisplaySanity()
{
    std::cout << "\n";
    std::cout << "============================\n";
    std::cout << "          SANITY\n";
    std::cout << "============================\n";

    std::cout << "Sanity: " << player.GetSanity() << "/100\n";

    std::cout << "State: " << player.GetSanityState() << "\n";
}

// Reusable integer input validation method
//
// minimum = smallest accepted menu choice
// maximum = largest accepted menu choice
//
// The method continues asking until the user enters
// a valid integer inside the accepted range
int Game::GetValidatedInput(int minimum, int maximum)
{
    int choice;

    while (true)
    {
        std::cin >> choice;

        // Check whether the user entered something
        // that could not be stored as an integer
        if (std::cin.fail())
        {
            // Reset cin error state
            std::cin.clear();

            // Remove invalid characters still waiting
            // inside the input buffer
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');

            std::cout << "\nInvalid input.\n";
            std::cout << "Enter a number between "
                << minimum
                << " and "
                << maximum
                << ": ";
        }

        // Check whether the number falls outside
        // the accepted menu range.
        else if (choice < minimum || choice > maximum)
        {
            std::cout << "\nInvalid selection.\n";
            std::cout << "Enter a number between "
                << minimum
                << " and "
                << maximum
                << ": ";
        }

        // The input is valid.
        else
        {
            // Remove any remaining characters from
            // the current input line.
            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(),
                '\n'
            );

            return choice;
        }
    }
}


void Game::HandleMemoryEvent()
{
    // Find Matt's current room.
    int roomIndex = player.GetCurrentRoom();

    // Read the event state for this location.
    int eventState = memoryStates[roomIndex];

    // A completed memory cannot be repeated for rewards.
    if (eventState == 2)
    {
        std::cout << "\n";
        std::cout << "You have already confronted this memory.\n";
        std::cout << "There is nothing more to recover here.\n";
        return;
    }

    std::cout << "\n";
    std::cout << "====================================\n";
    std::cout << "          MEMORY EVENT\n";
    std::cout << "====================================\n";

    // Each prototype room has its own memory event.
    if (roomIndex == 0)
    {
        // TRAIN CAR MEMORY
        std::cout << "\n";
        std::cout << "A scream cuts through the silence.\n";
        std::cout << "You remember the train.\n";
        std::cout << "You remember someone needed help.\n";
        std::cout << "Your body would not move.\n";
    }
    else if (roomIndex == 1)
    {
        // PLATFORM MEMORY
        std::cout << "\n";
        std::cout << "You see yourself standing on the platform.\n";
        std::cout << "The scene repeats, but something is wrong.\n";
        std::cout << "You hear a voice asking why you froze.\n";
    }
    else if (roomIndex == 2)
    {
        // WAITING ROOM MEMORY
        std::cout << "\n";
        std::cout << "A familiar voice echoes through the room.\n";
        std::cout << "You remember your family waiting for you.\n";
        std::cout << "You remember promises you have not kept.\n";
    }

    // SANITY-DEPENDENT PERCEPTION
    //
    // At low sanity, the memory is interpreted
    // through additional psychological distortion.
    if (player.GetSanity() <= 50)
    {
        std::cout << "\n";
        std::cout << "The walls begin to change.\n";
        std::cout << "A second voice speaks over the first.\n";
        std::cout << "It sounds exactly like yours.\n";
    }

    // Display the player's available decisions.
    std::cout << "\n";
    std::cout << "What will you do?\n";
    std::cout << "1. Confront the memory\n";
    std::cout << "2. Look away\n";
    std::cout << "3. Leave the memory alone\n";
    std::cout << "Enter choice: ";

    // Reuse the input validation from Week 2.
    int choice = GetValidatedInput(1, 3);

    if (choice == 1)
    {
        // CONFRONTATION
        //
        // Matt acknowledges the memory and gains clarity.
        // This is a small sanity recovery, not a cure.

        std::cout << "\n";
        std::cout << "You force yourself to stay with the memory.\n";
        std::cout << "It hurts, but the scene becomes clearer.\n";

        // Restore a limited amount of sanity.
        player.ChangeSanity(8);

        // Store a different memory fragment for each room.
        if (roomIndex == 0)
        {
            player.AddMemory(
                "Train Car: Matt remembers freezing "
                "during the violent incident."
            );
        }
        else if (roomIndex == 1)
        {
            player.AddMemory(
                "Platform: Matt recognizes the guilt "
                "he has carried since the incident."
            );
        }
        else if (roomIndex == 2)
        {
            player.AddMemory(
                "Waiting Room: Matt remembers the family "
                "he has been pulling away from."
            );
        }

        // Mark this memory as permanently resolved
        // during the current game session.
        memoryStates[roomIndex] = 2;

        std::cout << "\n";
        std::cout << "Sanity increased by up to 8.\n";
        std::cout << "Memory fragment recovered.\n";
    }
    else if (choice == 2)
    {
        // AVOIDANCE
        //
        // The first avoidance increases Matt's instability.
        // The memory remains unresolved so the player
        // can return to confront it later.

        std::cout << "\n";
        std::cout << "You turn away before the memory finishes.\n";
        std::cout << "The sound follows you.\n";

        // Only penalize the first avoidance.
        // This prevents the player from farming sanity loss.
        if (eventState == 0)
        {
            player.ChangeSanity(-12);

            // Mark the memory as avoided but unresolved.
            memoryStates[roomIndex] = 1;

            std::cout << "\n";
            std::cout << "Sanity decreased by up to 12.\n";
        }
        else
        {
            std::cout << "\n";
            std::cout << "You have avoided this memory before.\n";
            std::cout << "It remains unresolved.\n";
        }
    }
    else
    {
        // LEAVE
        //
        // Matt chooses not to engage with the memory.
        // No sanity changes and the event stays available.

        std::cout << "\n";
        std::cout << "You step away from the memory.\n";
        std::cout << "It remains just beyond your reach.\n";
    }

    // Display the player's updated psychological state.
    std::cout << "\n";
    std::cout << "Current sanity: "
        << player.GetSanity()
        << "/100\n";

    std::cout << "Mental state: "
        << player.GetSanityState()
        << "\n";
}

