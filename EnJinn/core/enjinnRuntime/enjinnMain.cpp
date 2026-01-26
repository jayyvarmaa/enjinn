#include <iostream>
#include <cstdio>
#include <filesystem>
#include <string>

#include <glad/glad.h>
#include <windowSystemm/window.h>


#include "logs/assert.h"
#include "dllLoader/dllLoader.h"

#if ENJINN_SHOULD_REMOVE_EDITOR

#else
	#include "enjinnImgui/enjinnImgui.h"
#endif

#include <memoryArena/memoryArena.h>
#include <runtimeContainer/runtimeContainer.h>

#include <logs/log.h>
#include <logs/logWindow.h>

#include <editor/editor.h>
#include <shortcutApi/shortcutApi.h>
#include <globalAllocator/globalAllocator.h>

#include <containerManager/containerManager.h>
#include <staticVector.h>

static bool shouldClose = false;

#if defined(ENJINN_WINDOWS)
#include <Windows.h>

BOOL WINAPI customConsoleHandlerRoutine(
	_In_ DWORD dwCtrlType
)
{

	if (dwCtrlType == CTRL_CLOSE_EVENT)
	{
		shouldClose = true;
	
		return true;
	}

	return false;
}

#endif

#pragma region gpu
extern "C"
{
	__declspec(dllexport) unsigned long NvOptimusEnablement = 1;
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#pragma endregion

int main()
{

#pragma region Console

	
//#if !(ENJINN_SHOULD_REMOVE_EDITOR)
//	//internal console
//
//	{
//
//
//		//std::streambuf *old = std::cout.rdbuf(consoleBuffer.rdbuf());
//
//		//std::cout << "Bla" << std::endl;
//		//printf("test\n");
//		//std::cout.sync_with_stdio();
//
//		//std::string text = buffer.str();
//	}
//
//#else
//	//normal console if enabeled
//#if defined(ENJINN_WINDOWS)
//#ifdef ENJINN_PRODUCTION
//#if ENJINN_ENABLE_CONSOLE_IN_PRODUCTION
//	{
//		AllocConsole();
//		(void)freopen("conin$", "r", stdin);
//		(void)freopen("conout$", "w", stdout);
//		(void)freopen("conout$", "w", stderr);
//		std::cout.sync_with_stdio();
//
//		//HWND hwnd = GetConsoleWindow(); //dissable console x button
//		//HMENU hmenu = GetSystemMenu(hwnd, FALSE);
//		//EnableMenuItem(hmenu, SC_CLOSE, MF_GRAYED);
//
//		SetConsoleCtrlHandler(0, true); //dissable ctrl+c shortcut in console
//		SetConsoleCtrlHandler(customConsoleHandlerRoutine, true); //custom exti function on clicking x button on console
//	}
//#endif
//#endif
//#endif
//#endif


#pragma endregion


#pragma region init global variables stuff
	enjinn::initShortcutApi();
#pragma endregion

#pragma region log
	enjinn::LogManager logs;
	logs.init(enjinn::LogManager::DefaultLogFile);

#pragma endregion
	//todo (in the future) increment id if it wasn't possible to copy the file
#pragma region load dll
	std::filesystem::path currentPath = std::filesystem::current_path();
	enjinn::LoadedDll loadedDll;
	ENJINN_PERMA_ASSERT(loadedDll.tryToloadDllUntillPossible(0, logs, std::chrono::seconds(5)),
		"Couldn't load dll");
#pragma endregion
	
#pragma region enjinn imgui id manager
	enjinn::enjinnImgui::ImGuiIdsManager imguiIdsManager;
#pragma endregion

#pragma region push notification manager
#if !(ENJINN_SHOULD_REMOVE_PUSH_NOTIFICATIONS)
	enjinn::PushNotificationManager pushNotificationManager; 
	pushNotificationManager.init();
	logs.pushNotificationManager = &pushNotificationManager;
#endif
#pragma endregion

#pragma region init window opengl imgui and context
	ENJINN_PERMA_ASSERT(glfwInit(), "Problem initializing glfw");
	//glfwSetErrorCallback(error_callback); todo
	enjinn::EnJinnWindow window = {};
	window.create();

	ENJINN_PERMA_ASSERT(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "Problem initializing glad");

	logs.log(("OpenGL Version: " + std::string((const char*)glGetString(GL_VERSION))).c_str());
	logs.log(("OpenGL Vendor: " + std::string((const char*)glGetString(GL_VENDOR))).c_str());
	logs.log(("OpenGL Renderer: " + std::string((const char*)glGetString(GL_RENDERER))).c_str());


	enjinn::enjinnImgui::initImgui(window.context);

	window.context.glfwMakeContextCurrentPtr = glfwMakeContextCurrent;
#pragma endregion

#pragma region container manager

	enjinn::ContainerManager containerManager;

	containerManager.init();

#pragma endregion

#pragma region init dll reaml

	loadedDll.gameplayStart_(window.context, logs);

	
#pragma endregion


#pragma region shortcuts
	enjinn::ShortcutManager shortcutManager;
#pragma endregion

#pragma region editor
#if !ENJINN_SHOULD_REMOVE_EDITOR
	enjinn::Editor editor; 
	

	editor.init(shortcutManager, imguiIdsManager);
#endif
#pragma endregion

#if ENJINN_PRODUCTION == 1
	for (auto &c : loadedDll.containerInfo)
	{
		if (c.containerStaticInfo.openOnApplicationStartup)
		{
		#if !ENJINN_SHOULD_REMOVE_EDITOR
			auto container = containerManager.createContainer
			(c, loadedDll, logs, imguiIdsManager, &editor.consoleWindow, std::string());
		#else
			auto container = containerManager.createContainer
			(c, loadedDll, logs, imguiIdsManager, nullptr, std::string());
		#endif
		}
	}
#endif


	while (!shouldClose)
	{
		if (window.shouldClose())
		{
			shouldClose = true;
			break;
		}


	#pragma region start imgui
		enjinn::enjinnImgui::imguiStartFrame(window.context);
	#pragma endregion

	#pragma region clear screen

	#if ENJINN_PRODUCTION
		//#if ENJINN_CLEAR_SCREEN_BY_ENGINE_IN_PRODUCTION && ENJINN_CLEAR_DEPTH_BY_ENGINE _IN_PRODUCTION
		//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//#elif ENJINN_CLEAR_SCREEN_BY_ENGINE_IN_PRODUCTION
		//	glClear(GL_COLOR_BUFFER_BIT);
		//#elif ENJINN_CLEAR_DEPTH_BY_ENGINE_IN_PRODUCTION
		//	glClear(GL_DEPTH_BUFFER_BIT);
		//#endif

	#else

		glClear(GL_COLOR_BUFFER_BIT);

	#endif



	#pragma endregion

	#pragma region editor stuff
	#if !ENJINN_SHOULD_REMOVE_EDITOR
		editor.update(window.input, shortcutManager, logs, 
			pushNotificationManager, loadedDll, containerManager, imguiIdsManager);
	#endif
	#pragma endregion



	#pragma region container manager

	#if !(ENJINN_SHOULD_REMOVE_EDITOR)
		if (editor.shouldReloadDll)
		{
			editor.shouldReloadDll = false;
			containerManager.reloadDll(loadedDll, window, logs);
		}

		containerManager.update(loadedDll, window, logs, imguiIdsManager, &editor.consoleWindow);

	#else
		containerManager.update(loadedDll, window, logs, imguiIdsManager, nullptr);
	#endif

		//close engine if no more containers are open
	#if ENJINN_PRODUCTION == 1
		if (containerManager.runningContainers.empty())
		{
			shouldClose = true;
		}
	#endif


	#pragma endregion

	#pragma region push notification
	#if !(ENJINN_SHOULD_REMOVE_PUSH_NOTIFICATIONS)
		static bool pushNoticicationOpen = true;
		pushNotificationManager.update(pushNoticicationOpen);
	#endif
	#pragma endregion

	#pragma region end imgui frame
		enjinn::enjinnImgui::imguiEndFrame(window.context);
	#pragma endregion

	#pragma region window update
		window.update();
	#pragma endregion


	#pragma region shortcut manager update
		shortcutManager.update(window.input);
	#pragma endregion
	
	#if !ENJINN_SHOULD_REMOVE_EDITOR
		editor.saveFlagsData();
	#endif

	window.saveWindowPositions();

	}

	#if !ENJINN_SHOULD_REMOVE_EDITOR
		editor.saveFlagsData();
	#endif

	//todo flag?
	window.saveWindowPositions();


	containerManager.destroyAllContainers(loadedDll, logs);

	
	

	//terminate();

	return 0;
}