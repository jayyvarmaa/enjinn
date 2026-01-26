#include "shortcutApi.h"
#include <stringManipulation/stringManipulation.h>
#include <iostream>
#include <unordered_map>
#include <set>
#include <imgui.h>
#include <imgui.h>

struct Mapping
{
	short normal = 0;
	short imgui = 0;
};

//todo remove global things that allocate memory
std::unordered_map<std::string, Mapping> buttonMapping;

void enjinn::initShortcutApi()
{
	buttonMapping =
	{
		{"a", {enjinn::Button::A, ImGuiKey_A}},
		{ "b", {enjinn::Button::B, ImGuiKey_B} },
		{ "c", {enjinn::Button::C, ImGuiKey_C} },
		{ "d", {enjinn::Button::D, ImGuiKey_D} },
		{ "e", {enjinn::Button::E, ImGuiKey_E} },
		{ "f", {enjinn::Button::F, ImGuiKey_F} },
		{ "g", {enjinn::Button::G, ImGuiKey_G} },
		{ "h", {enjinn::Button::H, ImGuiKey_H} },
		{ "i", {enjinn::Button::I, ImGuiKey_I} },
		{ "j", {enjinn::Button::J, ImGuiKey_J} },
		{ "k", {enjinn::Button::K, ImGuiKey_K} },
		{ "l", {enjinn::Button::L, ImGuiKey_L} },
		{ "m", {enjinn::Button::M, ImGuiKey_M} },
		{ "n", {enjinn::Button::N, ImGuiKey_N} },
		{ "o", {enjinn::Button::O, ImGuiKey_O} },
		{ "p", {enjinn::Button::P, ImGuiKey_P} },
		{ "q", {enjinn::Button::Q, ImGuiKey_Q} },
		{ "r", {enjinn::Button::R, ImGuiKey_R} },
		{ "s", {enjinn::Button::S, ImGuiKey_S} },
		{ "t", {enjinn::Button::T, ImGuiKey_T} },
		{ "u", {enjinn::Button::U, ImGuiKey_U} },
		{ "v", {enjinn::Button::V, ImGuiKey_V} },
		{ "w", {enjinn::Button::W, ImGuiKey_W} },
		{ "x", {enjinn::Button::X, ImGuiKey_X} },
		{ "y", {enjinn::Button::Y, ImGuiKey_Y} },
		{ "z", {enjinn::Button::Z, ImGuiKey_Z} },
		{ "0", {enjinn::Button::NR0, ImGuiKey_0}}, { "1", {enjinn::Button::NR1, ImGuiKey_1} }, { "2", {enjinn::Button::NR2, ImGuiKey_2} }, { "3", {enjinn::Button::NR3, ImGuiKey_3} },
		{ "4", {enjinn::Button::NR4, ImGuiKey_0}}, { "5", {enjinn::Button::NR5, ImGuiKey_5} }, { "6", {enjinn::Button::NR6, ImGuiKey_6} }, { "7", {enjinn::Button::NR7, ImGuiKey_7} },
		{ "8", {enjinn::Button::NR8, ImGuiKey_8}}, { "9", {enjinn::Button::NR9, ImGuiKey_9} },
		{ "space", {enjinn::Button::Space , ImGuiKey_Space}},
		{ "enter", {enjinn::Button::Enter, ImGuiKey_Enter} },
		{ "escape", {enjinn::Button::Escape, ImGuiKey_Escape} },
		{ "esc", {enjinn::Button::Escape, ImGuiKey_Escape} },
		{ "up", {enjinn::Button::Up, ImGuiKey_UpArrow} },
		{ "down", {enjinn::Button::Down , ImGuiKey_DownArrow}},
		{ "left", {enjinn::Button::Left , ImGuiKey_LeftArrow}},
		{ "right", {enjinn::Button::Right , ImGuiKey_RightArrow}},
		{ "ctrl", {enjinn::Button::LeftCtrl , ImGuiKey_LeftCtrl}},
		{ "tab", {enjinn::Button::Tab , ImGuiKey_Tab}},
		{ "alt", {enjinn::Button::LeftAlt , ImGuiKey_LeftAlt}},

	};

}



namespace enjinn
{

std::vector<std::string> tokenizeShortcutSimple(const char *shortcut)
{
	char data[256] = {};
	enjinn::removeCharacters(data, shortcut, "\n \t\r\v", sizeof(data));
	enjinn::toLower(data, data, sizeof(data));

	auto token = enjinn::split(data, '+');

	return token;
};


std::vector<std::string> tokenizeShortcutNormalized(const char *shortcut)
{
	
	auto token = tokenizeShortcutSimple(shortcut);

	std::set<std::string> tokenSet;

	for (auto &i : token)
	{

		auto it = buttonMapping.find(i);

		if (it != buttonMapping.end())
		{
			tokenSet.insert(it->first);
		}
	}

	std::vector<std::string> ret;
	ret.reserve(tokenSet.size());

	for (auto &i : tokenSet)
	{
		ret.push_back(std::move(i));
	}

	return ret;

}

std::string normalizeShortcutName(const char *shortcut)
{
	auto t = tokenizeShortcutNormalized(shortcut);

	std::string ret = "";
	for (int i = 0; i < t.size(); i++)
	{
		t[i][0] = std::toupper(t[i][0]);

		ret += t[i];

		if (i < t.size()-1)
		{
			ret += "+";
		}
	}
	
	return ret;
}



//todo shortcut should rely on glfw backend when imgui is disabeled in production build
bool shortcut(const enjinn::Input &input, const char *shortcut)
{
	auto token = tokenizeShortcutSimple(shortcut);
	
	if (token.empty()) { return 0; }


	bool pressed = false;

	if (0)
	{	//noraml implementation
		for (auto &t : token)
		{
			auto it = buttonMapping.find(t);
			if (it != buttonMapping.end())
			{
				if (input.buttons[it->second.normal].pressed())
				{
					pressed = true;
				}
				else if (!input.buttons[it->second.normal].held())
				{
					return false;
				}
			}
		}
	}
	else
	{	//imgui backend
		for (auto &t : token)
		{
			auto it = buttonMapping.find(t);
			if (it != buttonMapping.end())
			{
				if (ImGui::IsKeyPressed(it->second.imgui, false))
				{
					pressed = true;
				}
				else if (!ImGui::IsKeyDown(it->second.imgui))
				{
					return false;
				}
			}
		}
	}



	return pressed;
}

bool MenuItem(const enjinn::Input &input, const char *label, const char *shortcut, bool *p_selected, bool enabled)
{
	if (enjinn::shortcut(input, shortcut))
	{
		*p_selected = !*p_selected;
	}

	return ImGui::MenuItem(label, shortcut, p_selected, enabled);
}



void ShortcutManager::update(const enjinn::Input &input)
{
	for (auto &i : registeredShortcuts)
	{
		if (shortcut(input, i.second.shortcut.c_str()))
		{
			*i.second.toggle = !*i.second.toggle;
		}
	}

}

bool ShortcutManager::registerShortcut(const char *name, const char *s, bool *toggle, bool editable)
{

	if (registeredShortcuts.find(name) != registeredShortcuts.end())
	{
		//todo log error
		return 0;
	}
	else
	{
		registeredShortcuts[name] 
			= Shortcut{std::move(normalizeShortcutName(s)), toggle, editable};
		return 1;
	}

}

const char *ShortcutManager::getShortcut(const char *name)
{
	auto it = registeredShortcuts.find(name);

	if (it == registeredShortcuts.end()) { return ""; }
	else 
	{ return it->second.shortcut.c_str(); };

}

}
