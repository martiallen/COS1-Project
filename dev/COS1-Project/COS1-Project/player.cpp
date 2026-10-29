#include "Player.h"

// Constructor
// Gives the player default starting values
Player::Player()
{
    name = "Matt";
    sanity = 100;
    currentRoom = 0;
}

// Sets the player name
void Player::SetName(std::string playerName)
{
    name = playerName;
}

// Returns the player name
std::string Player::GetName()
{
    return name;
}

// Returns the player's current sanity
int Player::GetSanity()
{
    return sanity;
}

// Changes the player's sanity
// The value is kept between 0 and 100
void Player::ChangeSanity(int amount)
{
    sanity += amount;

    if (sanity > 100)
    {
        sanity = 100;
    }

    if (sanity < 0)
    {
        sanity = 0;
    }
}

// Returns the index of the room the player is currently in
int Player::GetCurrentRoom()
{
    return currentRoom;
}

// Changes the player's current room
void Player::SetCurrentRoom(int roomIndex)
{
    currentRoom = roomIndex;
}