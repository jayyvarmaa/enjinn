#pragma once
//////////////////////////////////////////
//dllMain.h
//Luta Vlad(c) 2022
//https://github.com/meemknight/EnJinn
//////////////////////////////////////////

#include <enjinnConfig.h>
#include <imgui.h>
#include <enjinnImgui/enjinnImgui.h>
#include <containers.h>
#include <containerInformation.h>
#include <vector>
#include <memoryArena/memoryArena.h>
#include <enjinnAllocator/freeListAllocator.h>
#include <sstream>

ENJINN_API void gameplayStart(enjinn::EnJinnContext &enjinnContext, enjinn::LogManager &logs);
ENJINN_API void setConsoleBuffer(std::streambuf *buf);
ENJINN_API void gameplayReload(enjinn::EnJinnContext &enjinnContext, enjinn::LogManager &logs);
ENJINN_API void getContainersInfo(std::vector<enjinn::ContainerInformation> &info);
ENJINN_API bool constructContainer(Container **c, enjinn::memory::MemoryArena *arena, const char *name);
ENJINN_API void destructContainer(Container **c, enjinn::memory::MemoryArena *arena);
ENJINN_API void bindAllocator(enjinn::memory::FreeListAllocator *arena);
ENJINN_API void resetAllocator();
ENJINN_API void dissableAllocators();
