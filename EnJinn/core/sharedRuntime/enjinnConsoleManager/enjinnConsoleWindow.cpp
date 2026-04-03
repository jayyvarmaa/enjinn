//////////////////////////////////////////
//enjinnConsoleWindow.cpp
//Luta Vlad(c) 2022
//https://github.com/meemknight/EnJinn
//////////////////////////////////////////

#include <enjinnConfig.h>
#if !ENJINN_SHOULD_REMOVE_EDITOR

#include "enjinnConsoleWindow.h"
#include <logs/assert.h>
#include <iostream>
#include <cstring>

namespace enjinn
{


	void ConsoleWindow::init(enjinn::enjinnImgui::ImGuiIdsManager &idManager)
	{
		imguiId = idManager.getImguiIds();
	}

	void ConsoleWindow::update(bool &open)
	{
		if (!commandsRegistered)
		{
			registerBuiltinCommands();
			commandsRegistered = true;
		}
		
		ImGui::PushID(imguiId);

		if (!ImGui::Begin(ICON_NAME, &open))
		{
			ImGui::End();
			ImGui::PopID();
			return;
		}

		// Output area
		float footerHeight = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
		ImGui::BeginChild("##console scrolling", ImVec2(0, -footerHeight), false);

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
		
		if (wrapped)
		{
			if(bufferBeginPos <= BUFFER_SIZE - 1)
			ImGui::TextWrapped(&buffer[bufferBeginPos + 1]);
			ImGui::TextWrapped(buffer);
		}
		else
		{
			ImGui::TextWrapped(buffer);
		}

		ImGui::PopStyleVar();

		if (scrollToBottom || ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
		{
			ImGui::SetScrollHereY(1.0f);
			scrollToBottom = false;
		}

		ImGui::EndChild();

		// Input line
		ImGui::Separator();
		bool reclaimFocus = false;
		ImGuiInputTextFlags inputFlags = ImGuiInputTextFlags_EnterReturnsTrue |
			ImGuiInputTextFlags_CallbackHistory;
		
		// History callback
		auto historyCallback = [](ImGuiInputTextCallbackData* data) -> int
		{
			ConsoleWindow* console = (ConsoleWindow*)data->UserData;
			if (data->EventFlag == ImGuiInputTextFlags_CallbackHistory)
			{
				int prevPos = console->historyPos;
				if (data->EventKey == ImGuiKey_UpArrow)
				{
					if (console->historyPos == -1)
						console->historyPos = (int)console->history.size() - 1;
					else if (console->historyPos > 0)
						console->historyPos--;
				}
				else if (data->EventKey == ImGuiKey_DownArrow)
				{
					if (console->historyPos != -1)
					{
						if (++console->historyPos >= (int)console->history.size())
							console->historyPos = -1;
					}
				}
				
				if (prevPos != console->historyPos)
				{
					const char* historyStr = (console->historyPos >= 0)
						? console->history[console->historyPos].c_str() : "";
					data->DeleteChars(0, data->BufTextLen);
					data->InsertChars(0, historyStr);
				}
			}
			return 0;
		};
		
		ImGui::PushItemWidth(-1);
		if (ImGui::InputText("##input", inputBuf, INPUT_BUF_SIZE, inputFlags,
			historyCallback, (void*)this))
		{
			if (inputBuf[0] != '\0')
			{
				// Echo command
				writeLine("> ");
				writeLine(inputBuf);
				writeLine("\n");
				
				// Add to history (avoid duplicates at end)
				if (history.empty() || history.back() != inputBuf)
				{
					history.push_back(inputBuf);
					if (history.size() > MAX_HISTORY)
						history.erase(history.begin());
				}
				
				executeCommand(inputBuf);
				historyPos = -1;
			}
			inputBuf[0] = '\0';
			reclaimFocus = true;
		}
		ImGui::PopItemWidth();
		
		if (reclaimFocus)
			ImGui::SetKeyboardFocusHere(-1);


		ImGui::End();
		ImGui::PopID();

	}

	void ConsoleWindow::write(const char *c)
	{
		size_t i = 0;
		while (c[i] != 0)
		{
			buffer[bufferBeginPos] = c[i];
			i++;
			bufferBeginPos++;

			if (bufferBeginPos >= BUFFER_SIZE)
			{
				bufferBeginPos = 0;
				wrapped = true;
			}

			if (wrapped)
			{
				buffer[bufferBeginPos] = 0;
			}
		}

	}

	void ConsoleWindow::writeLine(const char* c)
	{
		write(c);
		scrollToBottom = true;
	}

	void ConsoleWindow::registerCommand(const char* name, const char* description, CommandCallback callback)
	{
		CommandEntry entry;
		entry.name = name;
		entry.description = description;
		entry.callback = callback;
		commands[name] = entry;
	}

	void ConsoleWindow::executeCommand(const char* commandLine)
	{
		// Parse command name and args
		char cmdBuf[INPUT_BUF_SIZE];
		strncpy(cmdBuf, commandLine, INPUT_BUF_SIZE - 1);
		cmdBuf[INPUT_BUF_SIZE - 1] = '\0';
		
		// Find first space to split command name from args
		char* args = nullptr;
		for (int i = 0; cmdBuf[i]; i++)
		{
			if (cmdBuf[i] == ' ')
			{
				cmdBuf[i] = '\0';
				args = &cmdBuf[i + 1];
				break;
			}
		}
		
		auto it = commands.find(cmdBuf);
		if (it != commands.end())
		{
			it->second.callback(args ? args : "", *this);
		}
		else
		{
			writeLine("Unknown command: ");
			writeLine(cmdBuf);
			writeLine("\nType 'help' for available commands.\n");
		}
	}

	void ConsoleWindow::registerBuiltinCommands()
	{
		registerCommand("help", "List all available commands", 
			[](const char*, ConsoleWindow& c)
		{
			c.writeLine("Available commands:\n");
			for (auto& [name, entry] : c.commands)
			{
				c.writeLine("  ");
				c.writeLine(name.c_str());
				c.writeLine(" - ");
				c.writeLine(entry.description.c_str());
				c.writeLine("\n");
			}
		});
		
		registerCommand("clear", "Clear console output",
			[](const char*, ConsoleWindow& c)
		{
			memset(c.buffer, 0, sizeof(c.buffer));
			c.bufferBeginPos = 0;
			c.wrapped = false;
		});
		
		registerCommand("echo", "Print text to console",
			[](const char* args, ConsoleWindow& c)
		{
			c.writeLine(args);
			c.writeLine("\n");
		});
		
		registerCommand("version", "Print engine version",
			[](const char*, ConsoleWindow& c)
		{
			c.writeLine("EnJinn Engine " "0.1.0" "\n");
		});
	}

};

#else

#include "enjinnConsoleWindow.h"

namespace enjinn
{

	void ConsoleWindow::init(enjinn::enjinnImgui::ImGuiIdsManager &idManager)
	{
	}

	void ConsoleWindow::update(bool &open)
	{
	}

	void ConsoleWindow::write(const char *c)
	{
	}

	void ConsoleWindow::writeLine(const char* c)
	{
	}

	void ConsoleWindow::registerCommand(const char* name, const char* description, CommandCallback callback)
	{
	}

	void ConsoleWindow::executeCommand(const char* commandLine)
	{
	}

};

#endif