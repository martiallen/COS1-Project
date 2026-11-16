#include "Room.h"

// Default constructor
// Creates a basic room if no specific information is provided
Room::Room()
{
    name = "Unknown";

    stableDescription =
        "There is nothing unusual here.";

    uneasyDescription =
        "Something about this place feels wrong.";

    unstableDescription =
        "The room does not look the way you remember it.";

    fracturedDescription =
        "You cannot trust what you are seeing.";

    examined = false;
}

// Constructor used when creating a specific room.
Room::Room(std::string roomName,std::string stableText,std::string uneasyText,std::string unstableText,std::string fracturedText)
{
    name = roomName;

    stableDescription = stableText;
    uneasyDescription = uneasyText;
    unstableDescription = unstableText;
    fracturedDescription = fracturedText;

    // A new room has not been examined yet
    examined = false;
}

// Returns the room's name.
std::string Room::GetName()
{
    return name;
}

// Returns a different description depending on Matt sanity
//
// Higher sanity produces a more reliable description
// Lower sanity produces increasingly distorted information
std::string Room::GetDescription(int sanity)
{
    if (sanity >= 76)
    {
        return stableDescription;
    }
    else if (sanity >= 51)
    {
        return uneasyDescription;
    }
    else if (sanity >= 26)
    {
        return unstableDescription;
    }
    else
    {
        return fracturedDescription;
    }
}

// Returns whether Matt has already examined the room
bool Room::GetExamined()
{
    return examined;
}

// Changes whether the room has been examined
void Room::SetExamined(bool roomExamined)
{
    examined = roomExamined;
}

// Adds an item to this room
void Room::AddItem(const Item& item)
{
    items.push_back(item);
}

// Returns the number of items still inside the room
int Room::GetItemCount()
{
    return items.size();
}

// Returns an item using its vector index
Item Room::GetItem(int index)
{
    return items[index];
}

// Removes an item after Matt takes it
//
// erase removes the item at the requested position
void Room::RemoveItem(int index)
{
    items.erase(items.begin() + index);
}