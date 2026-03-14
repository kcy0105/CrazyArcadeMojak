#pragma once
#include <string>
#include <vector>

class ProtoParser
{
public:
    bool Parse(const std::string& path);

    const std::vector<std::string>& GetMessages() const { return _messages; }

private:
    std::vector<std::string> _messages;
};