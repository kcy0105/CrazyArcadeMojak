#include "pch.h"
#include "CodeGen.h"
#include <fstream>

void CodeGen::GeneratePacketEnum(const std::vector<std::string>& messages)
{
    std::ofstream file("PacketEnum.h");

    file << "#pragma once\n\n";
    file << "#include \"Protocol.pb.h\"\n\n";

    // enum 持失
    file << "enum PacketId\n{\n";

    int id = 10;

    for (auto& msg : messages)
    {
        file << "    " << msg << " = " << id++ << ",\n";
    }

    file << "};\n\n";

    // PacketIdType template
    file << "template<typename T>\n";
    file << "struct PacketIdType\n";
    file << "{\n";
    file << "};\n\n";

    // specialization 持失
    for (auto& msg : messages)
    {
        file << "template<>\n";
        file << "struct PacketIdType<Protocol::" << msg << ">\n";
        file << "{\n";
        file << "    static const uint16 value = " << msg << ";\n";
        file << "};\n\n";
    }
}

void CodeGen::GenerateServerHandlerHeader(const std::vector<std::string>& messages)
{
    std::ofstream file("ServerPacketHandler.h");

    file << "#pragma once\n\n";
    file << "#include \"Protocol.pb.h\"\n\n";

    file << "class ServerPacketHandler\n";
    file << "{\n";
    file << "public:\n\n";

    for (auto& msg : messages)
    {
        if (msg[0] != 'C')
            continue;

        file << "    static void Handle_"
            << msg
            << "(SessionRef session, Protocol::"
            << msg
            << "& pkt);\n\n";
    }

    file << "};\n";
}

void CodeGen::GenerateClientHandlerHeader(const std::vector<std::string>& messages)
{
    std::ofstream file("ClientPacketHandler.h");

    file << "#pragma once\n\n";
    file << "#include \"Protocol.pb.h\"\n\n";

    file << "class ClientPacketHandler\n";
    file << "{\n";
    file << "public:\n\n";

    for (auto& msg : messages)
    {
        if (msg[0] != 'S')
            continue;

        file << "    static void Handle_"
            << msg
            << "(SessionRef session, Protocol::"
            << msg
            << "& pkt);\n\n";
    }

    file << "};\n";
}

void CodeGen::GenerateServerHandler(const std::vector<std::string>& messages)
{
    std::ofstream file("ServerPacketHandler.gen.cpp");

    file << "#include \"pch.h\"\n";
    file << "#include \"ServerPacketHandler.h\"\n";
    file << "#include \"PacketHandlerRegistry.h\"\n";
    file << "#include \"PacketEnum.h\"\n";
    file << "#include \"PacketMacro.h\"\n\n";

    for (auto& msg : messages)
    {
        if (msg[0] != 'C')
            continue;

        file << "REGISTER_PACKET("
            << msg
            << ", Protocol::"
            << msg
            << ", ServerPacketHandler::Handle_"
            << msg
            << ");\n";
    }
}

void CodeGen::GenerateClientHandler(const std::vector<std::string>& messages)
{
    std::ofstream file("ClientPacketHandler.gen.cpp");

    file << "#include \"pch.h\"\n";
    file << "#include \"ClientPacketHandler.h\"\n";
    file << "#include \"PacketHandlerRegistry.h\"\n";
    file << "#include \"PacketEnum.h\"\n";
    file << "#include \"PacketMacro.h\"\n\n";

    for (auto& msg : messages)
    {
        if (msg[0] != 'S')
            continue;

        file << "REGISTER_PACKET("
            << msg
            << ", Protocol::"
            << msg
            << ", ClientPacketHandler::Handle_"
            << msg
            << ");\n";
    }
}