#pragma once
//////////////////////////////////////////
//enjinnConfigInternal.h
//Luta Vlad(c) 2022
//https://github.com/meemknight/EnJinn
//////////////////////////////////////////

#pragma region Platform



#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) || defined(__WIN32__) || defined(__NT__)

#define ENJINN_WINDOWS 1

#elif defined(__linux__)

#define ENJINN_LINUX 1

#else

#error "EnJinn supports only windows and linux"

#endif

#pragma endregion

#ifdef ENJINN_DEVELOPMENT
#define ENJINN_API extern "C" __declspec(dllexport)
#elif defined(ENJINN_PRODUCTION)
#define ENJINN_API
#endif




#if (defined(ENJINN_PRODUCTION) && ENJINN_REMOVE_IMGUI_IN_PRODUCTION)
#define ENJINN_SHOULD_REMOVE_IMGUI 1
#else
#define ENJINN_SHOULD_REMOVE_IMGUI 0
#endif


#if ((defined(ENJINN_PRODUCTION) && ENJINN_REMOVE_PUSH_NOTIFICATION_IN_PRODUCTION) || ENJINN_SHOULD_REMOVE_IMGUI)
#define ENJINN_SHOULD_REMOVE_PUSH_NOTIFICATIONS 1
#else
#define ENJINN_SHOULD_REMOVE_PUSH_NOTIFICATIONS 0
#endif


#if ((defined(ENJINN_PRODUCTION)) || ENJINN_SHOULD_REMOVE_IMGUI)
#define ENJINN_SHOULD_REMOVE_EDITOR 1
#else
#define ENJINN_SHOULD_REMOVE_EDITOR 0
#endif

//#define ENJINN_SHOULD_REMOVE_IMGUI 0
//#define ENJINN_SHOULD_REMOVE_EDITOR 0