#include "Room.h"

// Default constructor
Room::Room()
{
    name = "Unknown";
    normalDescription = "There is nothing here.";
    distortedDescription = "Something feels wrong.";
}

// Constructor used when creating a room
Room::Room(std::string roomName,std::string normalText,std::string distortedText)
{
    name = roomName;
    normalDescription = normalText;
    distortedDescription = distortedText;
}

// Returns the room name
std::string Room::GetName()
{
    return name;
}

// Returns the room description based on the player sanity
std::string Room::GetDescription(int sanity)
{
    if (sanity >= 75)
    {
        return normalDescription;
    }

    return distortedDescription;
}