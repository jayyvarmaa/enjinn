#pragma once

#include <enjinnConfig.h>

#include <enjinnImgui/enjinnImgui.h>
#include <logs/log.h>
#include <enjinnSizes.h>
#include <functional>
#include <unordered_map>
#include <string>
#include <vector>

namespace enjinn
{

	struct ConsoleWindow
	{

		void init(enjinn::enjinnImgui::ImGuiIdsManager &idManager);

		void update(bool &open);

		static constexpr char *ICON = ICON_FK_TERMINAL;
		static constexpr char *NAME = "console";
		static constexpr char *ICON_NAME = ICON_FK_TERMINAL " console";

		int imguiId = 0;

		constexpr static const size_t BUFFER_SIZE = 3000;

		char buffer[BUFFER_SIZE+1] = {};
		size_t bufferBeginPos = 0;

		bool wrapped = 0;

		void write(const char *c);
		
		// Command system
		using CommandCallback = std::function<void(const char* args, ConsoleWindow& console)>;
		
		void registerCommand(const char* name, const char* description, CommandCallback callback);
		void executeCommand(const char* commandLine);
		void writeLine(const char* c);

	private:
		struct CommandEntry
		{
			std::string name;
			std::string description;
			CommandCallback callback;
		};
		
		static constexpr size_t MAX_HISTORY = 32;
		static constexpr size_t INPUT_BUF_SIZE = 256;
		
		std::unordered_map<std::string, CommandEntry> commands;
		char inputBuf[INPUT_BUF_SIZE] = {};
		std::vector<std::string> history;
		int historyPos = -1;
		bool scrollToBottom = false;
		bool commandsRegistered = false;
		
		void registerBuiltinCommands();
	};


};

