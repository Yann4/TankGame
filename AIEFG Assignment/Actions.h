#pragma once
#include <map>
#include <functional>
#include <string>
#include "Structs.h"

namespace Actions
{
    enum class Event : unsigned int{NAME, SPEAK, ENTER, HELP, QUIT};

    static std::map<std::string, std::function<Event(std::string, ClientIdentifer)>> actions;

    std::function<Event(std::string, ClientIdentifer)> getAction(std::string command);
    bool setAction(std::string command, std::function<Event(std::string, ClientIdentifer)> action);

    bool containsAction(std::string command);
    std::string getValidActionString(std::string command);

    //Returns a number as a fixed length string; i.e 79 at fixed length 3 will be 079
    //Returns empty string if it's impossible
    std::string intToFLString(int toConvert, int numDigits);
};
