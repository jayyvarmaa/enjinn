#pragma once
//////////////////////////////////////////
//enjinnConfig.h
//Luta Vlad(c) 2022
//https://github.com/meemknight/EnJinn
//////////////////////////////////////////



//////////////////////////////////////////
//
//constants
//
//ENJINN_WINDOWS
//ENJINN_LINUX
//ENJINN_DEVELOPMENT
//ENJINN_PRODUCTION
// 
//  
//////////////////////////////////////////




///chose the assert function to be run on perma assert in production code

//#define ENJINN_INTERNAL_CURRENT_ASSERT_FUNCTION enjinn::assert::assertFunctionProduction
#define ENJINN_INTERNAL_CURRENT_ASSERT_FUNCTION enjinn::assert::assertFunctionDevelopment
//#define ENJINN_INTERNAL_CURRENT_ASSERT_FUNCTION enjinn::assert::assertFunctionToLog
//#define ENJINN_INTERNAL_CURRENT_ASSERT_FUNCTION enjinn::assert::terminate
//#define ENJINN_INTERNAL_CURRENT_ASSERT_FUNCTION //remove all asserts in production (I don't recommand this)




#define ENJINN_REMOVE_PUSH_NOTIFICATIONS_IN_PRODUCTION 0


#define ENJINN_REMOVE_LOGS_TO_FILE_IN_PRODUCTION 0
#define ENJINN_REMOVE_LOGS_TO_NOTIFICATIONS_IN_PRODUCTION 0


#define ENJINN_REMOVE_OPTIONAL_NOVALUE_CHECKS_IN_PRODUCTION 1


#define ENJINN_REMOVE_PUSH_NOTIFICATION_IN_PRODUCTION 0



#define ENJINN_REMOVE_IMGUI_IN_PRODUCTION 1


 #define ENJINN_ENGINE_SAVES_PATH ENJINN_ENGINE_RESOURCES_PATH "/engineSaves/"


#include <enjinnConfigInternal.h>

#define ENJINN_NEW(type) new type
#define ENJINN_DELETE(ptr) delete ptr

