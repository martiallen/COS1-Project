#pragma once

#include <string>

class Item
{
private:

    // The name shown to the player
    std::string name;

    // A description explaining what the item is
    std::string description;

public:

    // Default constructor
    Item();

    // Constructor used when creating a specific item
    Item(std::string itemName, std::string itemDescription);

    // Returns the item name
    std::string GetName();

    // Returns the item description
    std::string GetDescription();
};