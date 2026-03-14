#include "pch.h"
#include "ProtoParser.h"
#include <fstream>

bool ProtoParser::Parse(const std::string& path)
{
    std::ifstream file(path);

    if (!file.is_open())
        return false;

    std::string word;

    while (file >> word)
    {
        if (word == "message")
        {
            std::string name;
            file >> name;

            _messages.push_back(name);
        }
    }

    return true;
}