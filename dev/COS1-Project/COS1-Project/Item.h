#pragma once

#include <string>

class Item
{
private:
    std::string name;
    std::string description;

public:
    Item();

    Item(std::string itemName, std::string itemDescription);

    std::string GetName();
    std::string GetDescription();
};