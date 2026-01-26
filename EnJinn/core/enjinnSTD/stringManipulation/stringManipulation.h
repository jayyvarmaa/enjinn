#pragma once
#include <string>
#include <vector>

namespace enjinn
{
	//todo overengineer with restrict and stuff



	//dest can be also source
	void removeCharacters(char *dest, const char *source, const char *charsToRemove, size_t destSize);

	//dest can be also source
	void toLower(char *dest, const char *source, size_t size);

	//dest can be also source
	void toUpper(char *dest, const char *source, size_t size);

	//checks if char is in source
	bool findChar(const char *source, char c);

	//returns char coppied (will be at max size-1)
	size_t strlcpy(char *dst, const char *src, size_t size);

	size_t strlcpy(char *dst, const std::string &src, size_t size);

	//template<size_t N>
	//inline size_t strlcpy(char *dst, enjinn::StaticString<N> &s)
	//{
	//	return strlcpy(dst, s.data(), s.size());
	//}


	std::vector<std::string> split(const char *source, char c);

	
}