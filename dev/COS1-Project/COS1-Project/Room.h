#pragma once

#include <string>
#include <vector>

#include "Item.h"

class Room
{
private:

    // Name of the location
    std::string name;

    // Description shown while Matt is stable
    std::string stableDescription;

    // Description shown when Matt becomes uneasy
    std::string uneasyDescription;

    // Description shown when Matt becomes unstable
    std::string unstableDescription;

    // Description shown when Matt becomes fractured
    std::string fracturedDescription;

    // Tracks whether Matt has already examined this room
    bool examined;

    // Stores items currently located inside the room
    std::vector<Item> items;

public:

    // Default constructor.
    Room();

    // Constructor used to create a complete room
    Room(
        std::string roomName,std::string stableText,std::string uneasyText,std::string unstableText,std::string fracturedText);

    // Returns the room name
    std::string GetName();

    // Returns a room description based on Matt sanity
    std::string GetDescription(int sanity);

    // Returns whether the room has already been examined
    bool GetExamined();

    // Changes the examined state of the room
    void SetExamined(bool roomExamined);

    // Adds an item to the room
    void AddItem(const Item& item);

    // Returns how many items remain inside the room
    int GetItemCount();

    // Returns a specific item from the room
    Item GetItem(int index);

    // Removes an item after Matt collects it
    void RemoveItem(int index);
};