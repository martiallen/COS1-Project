#include "Item.h"

// Default constructor
// Creates a generic item if no information is provided
Item::Item()
{
    name = "Unknown Item";
    description = "You are not sure what this is.";
}

// Constructor used to create an item with a name and description
Item::Item(std::string itemName, std::string itemDescription)
{
    name = itemName;
    description = itemDescription;
}

// Returns the name of the item
std::string Item::GetName()
{
    return name;
}

// Returns the description of the item
std::string Item::GetDescription()
{
    return description;
}