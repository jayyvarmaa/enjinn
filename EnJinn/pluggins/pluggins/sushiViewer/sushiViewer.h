#pragma once

#include <gl2d/gl2d.h>
#include <imgui.h>
#include <baseContainer.h>
#include <shortcutApi/shortcutApi.h>
#include <enjinnSizes.h>
#include <imgui_spinner.h>
#include <sushi/sushi.h>
#include <enjinnImgui/enjinnImgui.h>

struct SushiViewer: public Container
{

	gl2d::Renderer2D renderer;
	sushi::SushyContext sushiContext;
	gl2d::Font font;

	static ContainerStaticInfo containerInfo();

	bool create(RequestedContainerInfo &requestedInfo, enjinn::StaticString<256> commandLineArgument);

	bool update(enjinn::Input input, enjinn::WindowState windowState,
		RequestedContainerInfo &requestedInfo);

	void destruct(RequestedContainerInfo &requestedInfo);

	std::vector<unsigned int> toDelete;

	struct ImGuiStuff
	{
		//set this to something to automatically change the current element
		unsigned int elementId = 0;

		bool dragging = 0;
		glm::vec2 dragBegin = {};
		glm::vec2 originalPos = {};
		enjinn::enjinnImgui::FileSelector fileSelector{"Selected File", ENJINN_RESOURCES_PATH, {".sushi"}};
		
	}img;

	void displaySushiTextElementImgui(::sushi::Text &e, glm::vec4 parent, int id);

	void displaySushiParentElementImgui(::sushi::SushiParent &e, glm::vec4 parent, bool displayChildren);

	void displaySushiTransformImgui(::sushi::Transform &e, glm::vec4 parent, int id);

	void displaySushiBackgroundImgui(::sushi::Background &e, int id);
};

