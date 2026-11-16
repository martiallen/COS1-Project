#include "Player.h"

#include <iostream>

// Default constructor
// Gives Matt his starting values when a new game begins
Player::Player()
{
    name = "Matt";
    sanity = 100;
    currentRoom = 0;
}

// Changes the player name
void Player::SetName(std::string playerName)
{
    name = playerName;
}

// Returns the player name
std::string Player::GetName()
{
    return name;
}

// Returns Matt's current sanity value
int Player::GetSanity()
{
    return sanity;
}

// Changes Matt sanity
//
// Positive numbers increase sanity
// Negative numbers decrease sanity
//
// The value is prevented from going above 100
// or below 0
void Player::ChangeSanity(int amount)
{
    sanity += amount;

    // Prevent sanity from exceeding the maximum value.
    if (sanity > 100)
    {
        sanity = 100;
    }

    // Prevent sanity from falling below zero
    if (sanity < 0)
    {
        sanity = 0;
    }
}

// Returns the name of Matts current psychological state
//
// These states will eventually control more than room
// descriptions but Week 2 uses them primarily for perception
std::string Player::GetSanityState()
{
    if (sanity >= 76)
    {
        return "Stable";
    }
    else if (sanity >= 51)
    {
        return "Uneasy";
    }
    else if (sanity >= 26)
    {
        return "Unstable";
    }
    else
    {
        return "Fractured";
    }
}

// Returns the index of Matts current room
int Player::GetCurrentRoom()
{
    return currentRoom;
}

// Changes Matt's current room.
void Player::SetCurrentRoom(int roomIndex)
{
    currentRoom = roomIndex;
}

// Adds a collected item to Matt's inventory
void Player::AddItem(const Item& item)
{
    inventory.push_back(item);
}

// Displays every item Matt has collected
void Player::DisplayInventory()
{
    std::cout << "\n";
    std::cout << "============================\n";
    std::cout << "         INVENTORY\n";
    std::cout << "============================\n";

    // Check whether Matt has collected anything
    if (inventory.empty())
    {
        std::cout << "Your inventory is empty.\n";
        return;
    }

    // Loop through the inventory and display each item
    for (int i = 0; i < inventory.size(); i++)
    {
        std::cout << i + 1 << ". " << inventory[i].GetName() << "\n";

        std::cout << "   " << inventory[i].GetDescription() << "\n";
    }

    }// closes displayinventory()



    // WEEK 3: Stores a recovered memory.
    //
    // The memory is passed by const reference so the function
    // can read the text without modifying the original string.
    void Player::AddMemory(const std::string & memory)
    {
        // Add the recovered memory to the player's vector.
        memories.push_back(memory);
    }

    // WEEK 3: Displays Matt's recovered memories.
    void Player::DisplayMemories()
    {
        std::cout << "\n";
        std::cout << "============================\n";
        std::cout << "       MEMORY JOURNAL\n";
        std::cout << "============================\n";

        // Check whether any memories have been recovered.
        if (memories.empty())
        {
            std::cout << "No memories recovered.\n";
            return;
        }

        // Loop through each recovered memory.
        for (int i = 0; i < memories.size(); i++)
        {
            std::cout << "\nMemory " << i + 1 << ":\n";
            std::cout << memories[i] << "\n";
        }
    }

    // WEEK 3: Returns the total number of memories.
    int Player::GetMemoryCount()
    {
        return static_cast<int>(memories.size());
    }



