#pragma once
// Global configuration
#define ENJINN_VERSION "0.1.0"

// CMake injects the real path via -D; this fallback keeps IntelliSense happy.
#ifndef ENJINN_RESOURCES_PATH
#define ENJINN_RESOURCES_PATH "./resources/"
#endif
constexpr const char* PHASE_UNLOCK_SECRET = "jayyvarmaa";
