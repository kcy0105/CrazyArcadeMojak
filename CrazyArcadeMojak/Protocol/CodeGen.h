#pragma once
#include <vector>
#include <string>

class CodeGen
{
public:
    static void GeneratePacketEnum(const std::vector<std::string>& messages);
    static void GenerateServerHandlerHeader(const std::vector<std::string>& messages);
    static void GenerateClientHandlerHeader(const std::vector<std::string>& messages);
    static void GenerateServerHandler(const std::vector<std::string>& messages);
    static void GenerateClientHandler(const std::vector<std::string>& messages);
};