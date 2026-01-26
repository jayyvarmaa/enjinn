#pragma once
#include "enjinnConfig.h"
#include <filesystem>
#include <logs/assert.h>


#ifdef ENJINN_WINDOWS

#define NOMINMAX
#include <Windows.h>

namespace enjinn
{

	
	struct FileChanged
	{
	
		std::filesystem::path path;
		FILETIME time = {};

		void setFile(const char *path);
		bool changed();
	
	private:
	
	};
	
	#else
	
	
	struct FileChanged
	{
	
	
		std::filesystem::path path;
	
		void setFile(const char *path) {}
		bool changed()
		{
			ENJINN_PERMA_ASSERT(0, "file changed only supported on windows");
			return 0;
		}
	
	
	};
	
	#endif

};
