#include "pch.h"
#include "ProtoParser.h"
#include "CodeGen.h"

int main()
{
    ProtoParser parser;

    if (!parser.Parse("Protocol.proto"))
    {
        cout << "proto 파일을 찾을 수 없습니다." << endl;
        return -1;
    }

    auto& messages = parser.GetMessages();

    CodeGen::GeneratePacketEnum(messages);
    CodeGen::GenerateServerHandlerHeader(messages);
    CodeGen::GenerateClientHandlerHeader(messages);
    CodeGen::GenerateServerHandler(messages);
    CodeGen::GenerateClientHandler(messages);

    return 0;
}