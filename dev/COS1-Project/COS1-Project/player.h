#pragma once


#include <string>

class Player
{
private:
    std::string name;
    int sanity;
    int currentRoom;

public:
    Player();

    // Player information
    void SetName(std::string playerName);
    std::string GetName();

    // Sanity system
    int GetSanity();
    void ChangeSanity(int amount);

    // Player location
    int GetCurrentRoom();
    void SetCurrentRoom(int roomIndex);
};
