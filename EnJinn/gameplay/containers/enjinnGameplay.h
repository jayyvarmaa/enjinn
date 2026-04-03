#pragma once

#include <gl2d/gl2d.h>
#include <imgui.h>
#include <baseContainer.h>
#include <shortcutApi/shortcutApi.h>
#include <enjinnSizes.h>
#include <imgui_spinner.h>

struct Test
{
	int *ptr = 0;
	Test() { ptr = new int(5); }
	~Test() { delete ptr; ptr = nullptr; }
};

struct Gameplay : public Container
{

	gl2d::Renderer2D renderer;

	float *r = 0;

	//todo user can request imgui ids; shortcut manager context; allocators
	static ContainerStaticInfo containerInfo()
	{
		ContainerStaticInfo info = {};
		info.defaultHeapMemorySize = enjinn::MB(10);

		info.requestImguiFbo = true; //todo this should not affect the compatibility of input recording

		//info.openOnApplicationStartup = true;

		//info.bonusAllocators.push_back(100);
		//info.bonusAllocators.push_back(200);
		//info.bonusAllocators.push_back(300);
		//info.bonusAllocators.push_back(400);
		//info.bonusAllocators.push_back(100);
		//info.bonusAllocators.push_back(200);
		//info.bonusAllocators.push_back(300);
		//info.bonusAllocators.push_back(400);
		//info.bonusAllocators.push_back(100);
		//info.bonusAllocators.push_back(200);
		//info.bonusAllocators.push_back(300);
		//info.bonusAllocators.push_back(400);
		//info.bonusAllocators.push_back(200);
		//info.bonusAllocators.push_back(200);

		return info;
	}


	bool create(RequestedContainerInfo &requestedInfo, enjinn::StaticString<256> commandLineArgument)
	{
		renderer.create(requestedInfo.requestedFBO.fbo);
		//enjinn::initShortcutApi();
		r = new float;

		//void *test = new char[enjinn::MB(10)]; //todo let the allocator tell the engine somehow that it is out of memory
		return true;
	}

	bool update(enjinn::Input input, enjinn::WindowState windowState,
		RequestedContainerInfo &requestedInfo)
	{
		//todo keep window on top stuff



		if (input.buttons[enjinn::Button::A].released())
		{
			//.. A button was released
		}




		glClear(GL_COLOR_BUFFER_BIT);

		if (enjinn::shortcut(input, "Ctrl + S"))
		{
			requestedInfo.consoleWrite("save\n");
		}
		
		//if (input.buttons[enjinn::Button::S].released())
		//{
		//	requestedInfo.consoleWrite("save\n");
		//}

		if (input.buttons[enjinn::Button::G].released())
		{
			requestedInfo.createContainer("Gameplay");
		}

		auto launchLab = [&](const char* containerName)
		{
			requestedInfo.createContainer(containerName);
			requestedInfo.consoleWrite(containerName);
			requestedInfo.consoleWrite(" launched\n");
		};

		// Quick keyboard launch shortcuts for the new runtime labs.
		if (input.buttons[enjinn::Button::NR1].released())
		{
			launchLab("AnimationLab");
		}
		if (input.buttons[enjinn::Button::NR2].released())
		{
			launchLab("AudioLab");
		}
		if (input.buttons[enjinn::Button::NR3].released())
		{
			launchLab("ParticleLab");
		}

		requestedInfo.consoleWrite(input.typedInput);

		if (input.buttons[enjinn::Button::P].held())
		{
			requestedInfo.consoleWrite(std::to_string(input.deltaTime).c_str());
		}

		gl2d::enableNecessaryGLFeatures();
		renderer.updateWindowMetrics(windowState.windowW, windowState.windowH);

		*r += input.deltaTime * 4.f;

		if (input.hasFocus)
		{
			renderer.renderRectangle({10, 10, 100, 100}, Colors_Green, {}, *r);
		}
		else
		{
			renderer.renderRectangle({10, 10, 100, 100}, Colors_Blue, {}, *r);
		}
		
		int size = 11;
		renderer.renderRectangle({input.mouseX-size/2, input.mouseY-size/2, size, size},
			Colors_Red, {}, 0.f);


		//requestedInfo.consoleWrite((std::string("Mouse: ") + std::to_string(input.mouseX) + " " +
		//	std::to_string(input.mouseY) + "\n").c_str());


		//if (input.lMouse.pressed())
		//{
		//	std::cout << "pressed\n";
		//}
		//if (input.lMouse.released())
		//{
		//	std::cout << "released\n";
		//}

		//if (input.lMouse.typed())
		//{
		//	std::cout << "typed\n";
		//}

		//if (input.buttons[enjinn::Button::E].typed())
		//{
		//	std::cout << "e";
		//}

		//std::cout << state.deltaTime << "\n";

		renderer.flush();

		ImGui::SetNextWindowBgAlpha(0.85f);
		ImGui::SetNextWindowSize(ImVec2(360, 260), ImGuiCond_FirstUseEver);
		if (ImGui::Begin("Runtime Lab Launcher"))
		{
			ImGui::Text("New gameplay runtime containers");
			ImGui::Separator();

			ImGui::Text("Minecraft Dungeons Character Labs");
			if (ImGui::Button("Launch Animation Lab (1)", ImVec2(-1, 0)))
			{
				launchLab("AnimationLab");
			}
			if (ImGui::Button("Launch Audio Lab (2)", ImVec2(-1, 0)))
			{
				launchLab("AudioLab");
			}
			if (ImGui::Button("Launch Particle Emitter Lab (3)", ImVec2(-1, 0)))
			{
				launchLab("ParticleLab");
			}

			ImGui::Separator();
			ImGui::Text("Diagnostics Labs");
			if (ImGui::Button("AI & Behavior Diagnostics", ImVec2(-1, 0)))
			{
				launchLab("AIBehaviorDiagnosticsLab");
			}

			ImGui::Separator();
			ImGui::TextWrapped("AI & Behavior is the only active diagnostics lab exposed here right now. Other diagnostics labs are hidden until they are completed one by one.");
		}
		ImGui::End();

		return true;
	}

	//optional
	void destruct(RequestedContainerInfo &requestedInfo)
	{
		delete r;
		r = nullptr;
	}

};

//todo flag to clear screen from engine
//todo error popup
//todo error popup disable in release