#include "Item.h"

// Default constructor
Item::Item()
{
    name = "Unknown Item";
    description = "There is nothing unusual about it.";
}

// Constructor used when creating an item
Item::Item(std::string itemName, std::string itemDescription)
{
    name = itemName;
    description = itemDescription;
}

// Returns the item name
std::string Item::GetName()
{
    return name;
}

// Returns the item description
std::string Item::GetDescription()
{
    return description;
}