#pragma once

#include <iostream>
#include <gl2d/gl2d.h>
#include <imgui.h>
#include <baseContainer.h>
#include <shortcutApi/shortcutApi.h>
#include <enjinnSizes.h>
#include <imgui_spinner.h>
#include <fstream>
#include <globalAllocator/globalAllocator.h>
#include <TextEditor.h>

struct EnJinnTextEditor: public Container
{
	
	TextEditor textEditor;
	std::string buff;

	static ContainerStaticInfo containerInfo()
	{
		ContainerStaticInfo info = {};
		info.defaultHeapMemorySize = enjinn::MB(30);

		//todo to lower
		info.extensionsSuported = {".txt", ".c", ".cpp", ".glsl", ".frag", ".vert", ".geom", ".mtl", ".obj"};

		info.requestImguiIds = 1;

		return info;
	}

	bool create(RequestedContainerInfo &requestedInfo, enjinn::StaticString<256> commandLineArgument) override
	{
		//todo logs from containers
		//requestedInfo.

		std::string file = commandLineArgument.to_string();


		size_t size = 0;
		std::cout << "TextEditor: Opening file: " << file << "\n";
		
		if(!requestedInfo.getFileSizeBinary(file.c_str(), size))
		{
			std::cout << "TextEditor: Failed to get file size for: " << file << "\n";
		}
		std::cout << "TextEditor: File size: " << size << "\n";
		
		unsigned char *buffer = new unsigned char[size];
		
		if (!requestedInfo.readEntireFile(file.c_str(), buffer, size))
		{
			delete[] buffer;
			return 0;
		}

		buff = std::string(buffer, buffer + size); //todo
		delete[] buffer;

		textEditor.SetText(buff);

		return true;
	}

	bool update(enjinn::Input input, enjinn::WindowState windowState,
		RequestedContainerInfo &requestedInfo) override
	{

		//todo deffer
		ImGui::PushID(requestedInfo.requestedImguiIds);

		bool open = 1;
		if(!ImGui::Begin( std::string( "Text editor##" + std::to_string(requestedInfo.requestedImguiIds)).c_str() , &open))
		{
			if (!open)
			{
				ImGui::End();
				ImGui::PopID();
				return false;
			}

			ImGui::End();
			ImGui::PopID();
			return true;
		}

		if (!open)
		{
			ImGui::End();
			ImGui::PopID();
			return false;
		}


		textEditor.Render("TextEditor");
	

		ImGui::End();
		ImGui::PopID();

		return true;
	}

};