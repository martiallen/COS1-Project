#include "Game.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <limits>



// WEEK 4: Name of the file used for saving progress.
//
// The file will be created in the application's
// current working directory.
const std::string SAVE_FILE = "the_last_room_save.txt";

// Maximum counts accepted from a save file.
// These limits help reject damaged or invalid save data.
const int MAX_SAVE_ITEMS = 100;
const int MAX_SAVE_MEMORIES = 100;



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

void Game::Run()
{
    while (gameRunning)
    {
        // Show the main menu
        DisplayMainMenu();

        // Get a valid menu selection
        int choice = GetValidatedInput(1, 3);

        // Process the player's selection
        switch (choice)
        {
        case 1:
            StartNewGame();
            break;

        case 2:
            // Attempt to load the player's saved progress
            if (LoadGame())
            {
                std::cout << "\nSaved game loaded successfully!\n";

                // Resume gameplay here without resetting saved progress
                PlayGame();
            }
            else
            {
                std::cout << "\nUnable to load saved game.\n";
            }
            break;

        case 3:
            // Display a message before closing the application
            std::cout << "\nExiting the game...\n";
            std::cout << "Thank you for playing.\n";

            // Stop the main application loop
            gameRunning = false;
            break;
        }
    }
}

//Week 4 


// WEEK 4: Saves Matt current game progress.
//
// This function writes the current game state
// into a text file using std::ofstream.
//
// Returns true if saving succeeds.
// Returns false if a file error occurs.
bool Game::SaveGame()
{
    // Open the file for writing.
    //
    // std::ios::trunc replaces any existing save.
    std::ofstream output(
        SAVE_FILE,
        std::ios::out | std::ios::trunc
    );

    // Check whether the file opened correctly.
    if (!output.is_open())
    {
        std::cout << "\nERROR: Unable to save game.\n";
        return false;
    }

  
    // SECTION 1: SAVE FILE VERSION
   

    // Helps identify compatible save files.
    output << "THE_LAST_ROOM_V1\n";

  
    // SECTION 2: PLAYER STATE
  

    // Save Matt's current sanity and room index.
    output << player.GetSanity() << " "
        << player.GetCurrentRoom() << "\n";

   
    // SECTION 3: PLAYER INVENTORY


    // Save the number of collected items.
    int inventoryCount = player.GetInventoryCount();

    output << inventoryCount << "\n";

    // Save each item's name and description.
    for (int i = 0; i < inventoryCount; i++)
    {
        Item item = player.GetInventoryItem(i);

        // Quoted strings preserve spaces.
        output << std::quoted(item.GetName()) << " "
            << std::quoted(item.GetDescription())
            << "\n";
    }

   
    // SECTION 4: RECOVERED MEMORIES
   

    // Save how many memories Matt has recovered.
    int memoryCount = player.GetMemoryCount();

    output << memoryCount << "\n";

    // Write each recovered memory.
    for (int i = 0; i < memoryCount; i++)
    {
        output << std::quoted(
            player.GetMemory(i)
        ) << "\n";
    }


    // SECTION 5: ROOM INFORMATION
 

    // Save the number of rooms in the game.
    output << rooms.size() << "\n";

    // Loop through every room.
    for (int i = 0;
        i < static_cast<int>(rooms.size());
        i++)
    {
        // Save whether this room was examined.
        //
        // 1 = examined
        // 0 = not examined
        int examined = 0;

        if (rooms[i].GetExamined())
        {
            examined = 1;
        }

        // Save examination state and remaining items.
        output << examined << " "
            << rooms[i].GetItemCount()
            << "\n";

        // Save any items still inside this room.
        for (int j = 0;
            j < rooms[i].GetItemCount();
            j++)
        {
            Item item = rooms[i].GetItem(j);

            output << std::quoted(item.GetName())
                << " "
                << std::quoted(item.GetDescription())
                << "\n";
        }
    }

  
    // SECTION 6: MEMORY EVENT STATES


    // Save how many memory events exist.
    output << memoryStates.size() << "\n";

    // Save the state of each memory event.
    //
    // 0 = untouched
    // 1 = avoided
    // 2 = confronted
    for (int i = 0;
        i < static_cast<int>(memoryStates.size());
        i++)
    {
        output << memoryStates[i] << "\n";
    }

 
    // SECTION 7: FINALIZE THE FILE
  

    // Attempt to flush all output.
    output.flush();

    // Check whether writing completed successfully.
    if (!output)
    {
        std::cout << "\nERROR: Save failed.\n";

        output.close();

        return false;
    }

    // Close the save file.
    output.close();

    std::cout << "\nGame saved successfully.\n";
    std::cout << "File: " << SAVE_FILE << "\n";

    return true;
}


// WEEK 4: Loads saved game progress.
//
// Returns true when a saved game is restored.
// Returns false when the file is missing or invalid.
//
// Temporary objects prevent corrupted data from
// partially changing the current game.
bool Game::LoadGame()
{
   
    // SECTION 1: OPEN SAVE FILE
    

    // Open the save file for reading.
    std::ifstream input(SAVE_FILE);

    // Check whether the save file exists.
    if (!input.is_open())
    {
        std::cout << "\nNo save file found.\n";
        std::cout << "Start a New Game first.\n";

        return false;
    }

    // SECTION 2: VALIDATE FILE VERSION
   

    std::string version;

    // Read the save file's version identifier.
    if (!(input >> version) ||
        version != "THE_LAST_ROOM_V1")
    {
        std::cout << "\nERROR: Invalid save format.\n";
        return false;
    }

    // ----------------------------------------
    // SECTION 3: CREATE TEMPORARY GAME STATE
    // ----------------------------------------

    // This temporary player starts with default values.
    Player loadedPlayer;

    // Copy the current rooms.
    // Their mutable contents will be replaced below.
    std::vector<Room> loadedRooms = rooms;

    // Temporary vector for memory-event states.
    std::vector<int> loadedStates;

   
    // SECTION 4: LOAD PLAYER INFORMATION


    int savedSanity = 0;
    int savedRoom = 0;

    // Read sanity and location.
    if (!(input >> savedSanity >> savedRoom))
    {
        std::cout << "\nERROR: Invalid player data.\n";
        return false;
    }

    // Make sure sanity is within valid limits.
    if (savedSanity < 0 || savedSanity > 100)
    {
        std::cout << "\nERROR: Invalid sanity value.\n";
        return false;
    }

    // Make sure the saved room exists.
    if (savedRoom < 0 ||
        savedRoom >= static_cast<int>(loadedRooms.size()))
    {
        std::cout << "\nERROR: Invalid room location.\n";
        return false;
    }

    // Adjust the temporary player's sanity.
    //
    // Since Player starts at 100, the difference
    // sets sanity to the saved value.
    loadedPlayer.ChangeSanity(
        savedSanity - loadedPlayer.GetSanity()
    );

    // Restore Matt's room location.
    loadedPlayer.SetCurrentRoom(savedRoom);

  
    // SECTION 5: LOAD INVENTORY


    int inventoryCount = 0;

    // Read the number of collected items.
    if (!(input >> inventoryCount) ||
        inventoryCount < 0 ||
        inventoryCount > MAX_SAVE_ITEMS)
    {
        std::cout << "\nERROR: Invalid inventory count.\n";
        return false;
    }

    // Read each saved item.
    for (int i = 0; i < inventoryCount; i++)
    {
        std::string itemName;
        std::string itemDescription;

        // Read the name and description.
        if (!(input >> std::quoted(itemName)
            >> std::quoted(itemDescription)))
        {
            std::cout << "\nERROR: Invalid inventory data.\n";
            return false;
        }

        // Prevent unusually large or damaged strings.
        if (itemName.size() > 500 ||
            itemDescription.size() > 500)
        {
            std::cout << "\nERROR: Item text is invalid.\n";
            return false;
        }

        // Rebuild the saved item.
        Item loadedItem(itemName, itemDescription);

        // Add the item to Matt's inventory.
        loadedPlayer.AddItem(loadedItem);
    }

    
    // SECTION 6: LOAD RECOVERED MEMORIES
 
    int memoryCount = 0;

    // Read the number of recovered memories.
    if (!(input >> memoryCount) ||
        memoryCount < 0 ||
        memoryCount > MAX_SAVE_MEMORIES)
    {
        std::cout << "\nERROR: Invalid memory count.\n";
        return false;
    }

    // Restore each recovered memory.
    for (int i = 0; i < memoryCount; i++)
    {
        std::string memory;

        // Read the quoted memory description.
        if (!(input >> std::quoted(memory)) ||
            memory.size() > 500)
        {
            std::cout << "\nERROR: Invalid memory data.\n";
            return false;
        }

        // Add the memory to the temporary player.
        loadedPlayer.AddMemory(memory);
    }

   
    // SECTION 7: LOAD ROOM INFORMATION
  

    int roomCount = 0;

    // Read the total number of saved rooms.
    if (!(input >> roomCount))
    {
        std::cout << "\nERROR: Invalid room count.\n";
        return false;
    }

    // The prototype requires the same number of
    // rooms that currently exist in the game.
    if (roomCount <= 0 ||
        roomCount != static_cast<int>(loadedRooms.size()))
    {
        std::cout << "\nERROR: Incompatible room data.\n";
        return false;
    }

    // Restore each room individually.
    for (int i = 0; i < roomCount; i++)
    {
        int examined = 0;
        int itemCount = 0;

        // Read whether the room was examined
        // and how many items remain inside.
        if (!(input >> examined >> itemCount))
        {
            std::cout << "\nERROR: Invalid room state.\n";
            return false;
        }

        // Validate the examination flag.
        if (examined != 0 && examined != 1)
        {
            std::cout << "\nERROR: Invalid examined state.\n";
            return false;
        }

        // Validate the number of room items.
        if (itemCount < 0 ||
            itemCount > MAX_SAVE_ITEMS)
        {
            std::cout << "\nERROR: Invalid room item count.\n";
            return false;
        }

        // Restore whether Matt examined this room.
        loadedRooms[i].SetExamined(examined == 1);

        // Remove existing items from the temporary room.
        // We will replace them with saved items.
        while (loadedRooms[i].GetItemCount() > 0)
        {
            loadedRooms[i].RemoveItem(0);
        }

        // Read all items remaining in this room.
        for (int j = 0; j < itemCount; j++)
        {
            std::string itemName;
            std::string itemDescription;

            if (!(input >> std::quoted(itemName)
                >> std::quoted(itemDescription)))
            {
                std::cout << "\nERROR: Invalid room item.\n";
                return false;
            }

            // Validate saved item text.
            if (itemName.size() > 500 ||
                itemDescription.size() > 500)
            {
                std::cout << "\nERROR: Invalid room item text.\n";
                return false;
            }

            // Recreate the saved room item.
            Item loadedItem(itemName, itemDescription);

            // Put the item back in its original room.
            loadedRooms[i].AddItem(loadedItem);
        }
    }

  
    // SECTION 8: LOAD MEMORY EVENT STATES
  

    int stateCount = 0;

    // Read the number of memory events.
    if (!(input >> stateCount) ||
        stateCount != roomCount)
    {
        std::cout << "\nERROR: Invalid memory-event count.\n";
        return false;
    }

    // Load each memory event's progress.
    for (int i = 0; i < stateCount; i++)
    {
        int state = -1;

        // Read the event's saved state.
        if (!(input >> state))
        {
            std::cout << "\nERROR: Invalid memory-event data.\n";
            return false;
        }

        // Only three states are supported:
        //
        // 0 = untouched
        // 1 = avoided
        // 2 = confronted
        if (state < 0 || state > 2)
        {
            std::cout << "\nERROR: Invalid memory state.\n";
            return false;
        }

        loadedStates.push_back(state);
    }

   
    // SECTION 9: CHECK FOR EXTRA SAVE DATA
  

    // Reject unexpected additional non-whitespace
    // content after the expected saved data.
    std::string extra;

    if (input >> extra)
    {
        std::cout << "\nERROR: Extra save data found.\n";
        return false;
    }

    // SECTION 10: RESTORE THE GAME
  

    // The save data has passed validation.
    // It is now safe to replace the live game state.
    player = loadedPlayer;
    rooms = loadedRooms;
    memoryStates = loadedStates;

    // Show confirmation to the player.
    std::cout << "\n";
    std::cout << "Game loaded successfully.\n";

    std::cout << "Current Location: "
        << rooms[player.GetCurrentRoom()].GetName()
        << "\n";

    std::cout << "Sanity: "
        << player.GetSanity()
        << "/100\n";

    return true;
}

// WEEK 4: Controls the active gameplay loop.
//
// This method does not reset the player or rooms.
// That allows a loaded game to resume correctly.
void Game::PlayGame()
{
    bool playing = true;

    // Continue until the player returns
    // to the main menu.
    while (playing)
    {
        // Display current location and available actions.
        DisplayGameMenu();

        // Week 4 has nine gameplay choices.
        int choice = GetValidatedInput(1, 9);

        if (choice == 1)
        {
            // Examine the current room.
            ExploreRoom();
        }
        else if (choice == 2)
        {
            // Collect an available item.
            CollectItem();
        }
        else if (choice == 3)
        {
            // Move between rooms.
            MovePlayer();
        }
        else if (choice == 4)
        {
            // Display Matt's physical inventory.
            player.DisplayInventory();
        }
        else if (choice == 5)
        {
            // Display current sanity information.
            DisplaySanity();
        }
        else if (choice == 6)
        {
            // Display recovered memory fragments.
            player.DisplayMemories();
        }
        else if (choice == 7)
        {
            // Confront or avoid a psychological memory.
            HandleMemoryEvent();
        }
        else if (choice == 8)
        {
            // WEEK 4: Save current progress.
            SaveGame();
        }
        else if (choice == 9 || choice == 0)
        {
            // Return to the application's main menu.
            playing = false;
        }
    }
}

void Game::DisplayMainMenu()
{
    std::cout << "\n====MAIN MENU====\n";
    std::cout << "1. New Game\n";
    std::cout << "2. Load Game\n";
    std::cout << "3. Exit Game\n";

}

// WEEK 4: Displays the main application menu.
//
// The player can start a new game,
// load previously saved progress,
// read instructions, or exit.

// WEEK 4: Displays the gameplay menu
// and the player's current environment.
void Game::DisplayGameMenu()
{
    // Find Matt's current room.
    int roomIndex = player.GetCurrentRoom();

    std::cout << "\n";
    std::cout << "----------------------------\n";
    std::cout << rooms[roomIndex].GetName() << "\n";
    std::cout << "----------------------------\n";

    // Room descriptions depend on Matt's sanity.
    std::cout << rooms[roomIndex].GetDescription(
        player.GetSanity()
    );

    std::cout << "\n\n";

    std::cout << "1. Examine surroundings\n";
    std::cout << "2. Collect item\n";
    std::cout << "3. Move\n";
    std::cout << "4. View inventory\n";
    std::cout << "5. Check sanity\n";
    std::cout << "6. View memory journal\n";
    std::cout << "7. Investigate memory\n";

    // WEEK 4: New save functionality.
    std::cout << "8. Save Game\n";

    std::cout << "9. Return to Main Menu\n";

    std::cout << "\nEnter choice: ";
}




// WEEK 4: Starts a completely new game.
//
// This resets Matt and recreates the rooms,
// inventory, memory states, and sanity.
void Game::StartNewGame()
{
    // Reset Matt to his default starting values.
    // This clears his inventory and memory journal.
    player = Player();

    // Remove all existing rooms from a previous game.
    rooms.clear();

    // Recreate the original rooms and their items.
    CreateRooms();

    // Remove all previous memory progress.
    memoryStates.clear();

    // Every room begins with an untouched memory.
    for (int i = 0;
        i < static_cast<int>(rooms.size());
        i++)
    {
        memoryStates.push_back(0);
    }

    // Display the opening scene.
    std::cout << "\n";
    std::cout << "====================================\n";
    std::cout << "Metal grinds against metal.\n";
    std::cout << "Matt wakes inside an empty train car.\n";
    std::cout << "He cannot remember how he arrived.\n";
    std::cout << "====================================\n";

    // Enter the gameplay loop.
    PlayGame();
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
void Game::DisplaySanity() const
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

// WEEK 4: Validates menu input.
//
// The player must enter a complete whole number
// within the supplied range.
//
// Examples:
// 1     = valid when within the requested range
// hello = invalid
// 3.5   = invalid
// 1abc  = invalid
// 99    = invalid when outside the range
//
// Returns 0 if console input is closed.
int Game::GetValidatedInput(int minimum, int maximum)
{
    std::string line;

    // Keep asking until valid input is provided.
    while (true)
    {
        // Read the entire input line.
        if (!std::getline(std::cin, line))
        {
            // Exit cleanly if input is no longer available.
            return 0;
        }

        // Create a stream from the player's input.
        std::istringstream inputStream(line);

        int choice = 0;

        // Stores unexpected extra text.
        std::string extra;

        // Verify that:
        // 1. An integer can be read.
        // 2. No additional text follows it.
        // 3. The value falls within the allowed range.
        if ((inputStream >> choice) &&
            !(inputStream >> extra) &&
            choice >= minimum &&
            choice <= maximum)
        {
            // Valid input: return the menu choice.
            return choice;
        }

        // Invalid input: explain the accepted range.
        std::cout << "\nInvalid input.\n";

        std::cout << "Enter a whole number between "
            << minimum
            << " and "
            << maximum
            << ": ";
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


        // WEEK 4: Check whether all three memory
        // fragments have been recovered.
        //
        // This represents completion of the COS1 prototype,
        // not the ending of the full psychological horror game.
        if (player.GetMemoryCount() == 3)
        {
            std::cout << "\n";
            std::cout << "====================================\n";
            std::cout << "          PROTOTYPE ENDING\n";
            std::cout << "====================================\n";

            std::cout << "\n";
            std::cout << "Three memories return.\n";
            std::cout << "The station lights stop flickering.\n";
            std::cout << "For the first time, the world is quiet.\n";

            std::cout << "\n";
            std::cout << "Matt has not escaped his past.\n";
            std::cout << "But he can finally see it clearly.\n";

            std::cout << "\n";
            std::cout << "END OF PROTOTYPE\n";

            std::cout << "====================================\n";
        }


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

