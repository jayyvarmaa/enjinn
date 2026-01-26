#define ENJINN_DECLARE_CONTAINER(x) info.push_back( enjinn::ContainerInformation(sizeof(x), #x, x::containerInfo()) );


#include "dllMain.h"

#include <gl2d/gl2d.h>

#include <imgui.h>

#include "enjinnImgui/enjinnImgui.h"
#include <logs/assert.h>

#include "containers/enjinnGameplay.h"
#include <containers.h>

#include <memoryArena/memoryArena.h>
#include <globalAllocator/globalAllocator.h>

//todo use a global static array that can be accessed from other cpps and the macro will create an instance of a struct 
//that will push that container

ENJINN_API void getContainersInfo(std::vector<enjinn::ContainerInformation> &info)
{
	info.clear();
	ENJINN_ALL_CONTAINERS()

}

#undef ENJINN_DECLARE_CONTAINER

//this should not allocate memory
ENJINN_API bool constructContainer(Container **c, enjinn::memory::MemoryArena *arena, const char *name)
{
	*c = getContainer(name, arena);
	return *c != 0;

}

ENJINN_API void destructContainer(Container **c, enjinn::memory::MemoryArena *arena)
{
	//no need to call delete.
	(*c)->~Container();

}

ENJINN_API void bindAllocator(enjinn::memory::FreeListAllocator *arena)
{
	enjinn::memory::setGlobalAllocator(arena);
}

ENJINN_API void resetAllocator()
{
	enjinn::memory::setGlobalAllocatorToStandard();
}

ENJINN_API void dissableAllocators()
{
	enjinn::memory::dissableAllocators();
}

void gl2dErrorFunc(const char *msg, void* userData) //todo
{
	enjinn::LogManager *logManager = (enjinn::LogManager *)userData;

	enjinn::memory::pushCustomAllocatorsToStandard();

	if (logManager)
		logManager->log(msg, enjinn::logError);

	enjinn::memory::popCustomAllocatorsToStandard();
}

//used to initialize libraries 
ENJINN_API void gameplayStart(enjinn::EnJinnContext &enjinnContext, enjinn::LogManager &logs)
{
	enjinn::enjinnImgui::setImguiAllocator(enjinnContext.imguiAllocator);
	enjinn::initShortcutApi(); //todo

	//todo user should have functions to specify this
#pragma region init stuff
#ifdef ENJINN_DEVELOPMENT
	ENJINN_PERMA_ASSERT(gladLoadGL(), "Problem initializing glad from dll");
	enjinn::enjinnImgui::setImguiContext(enjinnContext);
#endif

	gl2d::init();
	gl2d::setUserDefinedData(&logs);
	gl2d::setErrorFuncCallback(gl2dErrorFunc);

#pragma endregion
}


//this won't be ever called in production so we can remove the code
ENJINN_API void gameplayReload(enjinn::EnJinnContext &enjinnContext, enjinn::LogManager &logs)
{
#ifdef ENJINN_DEVELOPMENT	

	enjinn::enjinnImgui::setImguiAllocator(enjinnContext.imguiAllocator); //todo check if really needed
	enjinn::initShortcutApi();


	ENJINN_PERMA_ASSERT(gladLoadGL(), "Problem initializing glad from dll");
	enjinn::enjinnImgui::setImguiContext(enjinnContext);

	gl2d::init();
	gl2d::setUserDefinedData(&logs);
	gl2d::setErrorFuncCallback(gl2dErrorFunc);
#endif
}

#if ENJINN_WINDOWS
#ifdef ENJINN_DEVELOPMENT


#include <Windows.h>
//https://learn.microsoft.com/en-us/windows/win32/dlls/dllmain
BOOL WINAPI DllMain(
	HINSTANCE hinstDLL,  // handle to DLL module
	DWORD fdwReason,     // reason for calling function
	LPVOID lpvReserved)  // reserved
{
	// Perform actions based on the reason for calling.
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
	// Initialize once for each new process.
	// Return FALSE to fail DLL load.
	break;

	case DLL_THREAD_ATTACH:
	// Do thread-specific initialization.
	break;

	case DLL_THREAD_DETACH:
	// Do thread-specific cleanup.
	break;

	case DLL_PROCESS_DETACH:

	if (lpvReserved != nullptr)
	{
		break; // do not do cleanup if process termination scenario
	}

	//if (old)
	//{
	//	std::cout.rdbuf(old);
	//}

	// Perform any necessary cleanup.
	break;
	}
	return TRUE;  // Successful DLL_PROCESS_ATTACH.
}


#endif
#endif
