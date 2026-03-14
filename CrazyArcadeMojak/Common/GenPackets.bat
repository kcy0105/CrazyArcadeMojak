pushd %~dp0

REM ===== protoc =====
protoc-21.12-win64\binaries\protoc.exe -I=./ --cpp_out=./ Struct.proto
protoc-21.12-win64\binaries\protoc.exe -I=./ --cpp_out=./ Protocol.proto

IF ERRORLEVEL 1 PAUSE

REM ===== PacketGen =====
..\Binaries\x64\Protocol.exe

REM ===== Server =====
XCOPY /Y Struct.pb.h "..\Server"
XCOPY /Y Struct.pb.cc "..\Server"
XCOPY /Y Protocol.pb.h "..\Server"
XCOPY /Y Protocol.pb.cc "..\Server"
XCOPY /Y PacketEnum.h "..\Server"
XCOPY /Y ServerPacketHandler.h "..\Server"
XCOPY /Y ServerPacketHandler.gen.cpp "..\Server"

REM ===== Client =====
XCOPY /Y Struct.pb.h "..\Client"
XCOPY /Y Struct.pb.cc "..\Client"
XCOPY /Y Protocol.pb.h "..\Client"
XCOPY /Y Protocol.pb.cc "..\Client"
XCOPY /Y PacketEnum.h "..\Client"
XCOPY /Y ClientPacketHandler.h "..\Client"
XCOPY /Y ClientPacketHandler.gen.cpp "..\Client"

REM ===== temp ªË¡¶ =====
DEL /Q /F *.pb.h
DEL /Q /F *.pb.cc
DEL /Q /F *.gen.cpp
DEL /Q /F PacketEnum.h
DEL /Q /F ClientPacketHandler.h
DEL /Q /F ServerPacketHandler.h

PAUSE