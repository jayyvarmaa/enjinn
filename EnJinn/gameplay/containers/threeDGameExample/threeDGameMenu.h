#pragma once

#include <gl2d/gl2d.h>
#include <imgui.h>
#include <baseContainer.h>
#include <shortcutApi/shortcutApi.h>
#include <enjinnSizes.h>
#include <imgui_spinner.h>
#include <glui/glui.h>
#include <engineLibraresSupport/engineGL2DSupport.h>

struct ThreeDGameMenu : public Container
{

	gl2d::Renderer2D renderer;
	gl2d::Font font;
	gl2d::Texture logo;
	gl2d::Texture button;
	glui::RendererUi ui;

	static ContainerStaticInfo containerInfo()
	{
		ContainerStaticInfo info = {};
		info.defaultHeapMemorySize = enjinn::MB(100);

		info.requestImguiFbo = true; 
		//info.openOnApplicationStartup = true;
		
		

		return info;
	}


	bool create(RequestedContainerInfo &requestedInfo, enjinn::StaticString<256> commandLineArgument)
	{
		renderer.create(requestedInfo.requestedFBO.fbo);
		//enjinn::initShortcutApi(); //todo do this in dll load

		font = enjinn::gl2d::loadFont(ENJINN_RESOURCES_PATH "mcDungeons/CommodorePixeled.ttf", requestedInfo);
		logo = enjinn::gl2d::loadTexture(ENJINN_RESOURCES_PATH "threedgame/logo.png", requestedInfo);
		button = enjinn::gl2d::loadTexture(ENJINN_RESOURCES_PATH "mcDungeons/button.png", requestedInfo);


		return true;
	}

	bool update(enjinn::Input input, enjinn::WindowState windowState,
		RequestedContainerInfo &requestedInfo)
	{

		glClear(GL_COLOR_BUFFER_BIT);



		renderer.updateWindowMetrics(windowState.windowW, windowState.windowH);

		
		ui.Begin(69);
		{
			
			ui.Texture(1, logo);

			if (ui.Button("Start game", Colors_White, button))
			{
				requestedInfo.createContainer("ThreeDGameExample");
				return 0;
			}

			//ui.BeginMenu("Settings", Colors_White, button);
			//{
			//	if (ui.Button("Adaptive rezolution", Colors_White, button))
			//	{
			//
			//	}
			//}
			//ui.EndMenu();

		}
		ui.End();


		ui.renderFrame(renderer, font, {input.mouseX, input.mouseY},
			input.lMouse.pressed(), input.lMouse.held(), input.lMouse.released(),
			input.buttons[enjinn::Button::Escape].released(), input.typedInput, input.deltaTime);

		renderer.flush();

		

		return true;
	}

	void destruct(RequestedContainerInfo &requestedInfo)
	{

		renderer.cleanup();
		font.texture.cleanup();
		logo.cleanup();
		button.cleanup();

	}

};
