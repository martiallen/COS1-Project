#pragma once

#include <string>

class Room
{
private:
    std::string name;
    std::string normalDescription;
    std::string distortedDescription;

public:
    Room();

    Room(std::string roomName,std::string normalText, std::string distortedText);

    std::string GetName();

    // Displays a different description depending on sanity
    std::string GetDescription(int sanity);
};