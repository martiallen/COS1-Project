#pragma once

#include <string>
#include <vector>

#include "Item.h"

class Player
{
private:

    // The protagonist name
    std::string name;

    // Matt's current sanity value
    // Sanity stays between 0 and 100
    int sanity;

    // Stores the index of the room Matt is currently inside
    int currentRoom;

    // Stores all items Matt has collected
    std::vector<Item> inventory;


    // WEEK 3: Stores the memories Matt has recovered.
    // Each memory is stored as a string.
    std::vector<std::string> memories;


public:

    // Default constructor
    Player();

    // Sets the player's name.
    void SetName(std::string playerName);

    // Returns the player name
    std::string GetName();

    // Returns Matt's current sanity
    int GetSanity();

    // Changes sanity while keeping it between 0 and 100
    void ChangeSanity(int amount);

    // Returns a text description of Matt's current sanity state
    std::string GetSanityState();

    // Returns the index of Matt current room
    int GetCurrentRoom();

    // Changes Matt's current room
    void SetCurrentRoom(int roomIndex);

    // Adds an item to Matt's inventory
    void AddItem(const Item& item);

    // Displays every item currently in Matt inventory
    void DisplayInventory();


    // WEEK 3: Adds a recovered memory to Matt journal.
    void AddMemory(const std::string& memory);

    // WEEK 3: Displays all recovered memory fragments.
    void DisplayMemories();

    // WEEK 3: Returns the number of recovered memories.
    int GetMemoryCount();

};