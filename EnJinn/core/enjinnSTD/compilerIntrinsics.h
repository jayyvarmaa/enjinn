#pragma once

//todo debug release configurations cmake

//compiler
//
#ifdef _MSC_VER
	#define ENJINN_MSVC
#endif

#ifdef __GNUC__
	#define ENJINN_GCC
#endif

#ifdef __clang__
	#define ENJINN_CLANG
#endif
//


//assume
#ifdef ENJINN_MSVC
	#define ENJINN_ASSUME(x) __assume((x))
#else
	#define ENJINN_ASSUME(x)
#endif

//assume unreachable //todo for other compilers
#ifdef ENJINN_MSVC
#define ENJINN_UNREACHABLE() __assume((0))
#else
#define ENJINN_UNREACHABLE()
#endif


//optimize off
#ifdef ENJINN_MSVC
	#define ENJINN_OPTIMIZE_OFF() __pragma(optimize( "", off ))
#else
	#define ENJINN_OPTIMIZE_OFF()
#endif



//optimize on
#ifdef ENJINN_MSVC
	#define ENJINN_OPTIMIZE_ON() __pragma(optimize( "", on ))
#else
	#define ENJINN_OPTIMIZE_ON()
#endif




