#include "window.h"
#include <algorithm>
#include <stb_image/stb_image.h>
#include <iostream>
#include <logs/assert.h>
#include "callbacks.h"
#include <filesystem>

#ifdef ENJINN_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#include <Windows.h>
#include <GLFW/glfw3native.h>
#endif

#include <enjinnSizes.h>

#include <safeSave/safeSave.h>

struct WindowRect
{
	int x = 100, y = 100, z = 640, w = 480;
};

void enjinn::EnJinnWindow::create()
{

	WindowRect wr = {};

#if ENJINN_DEVELOPMENT
	if (sfs::safeLoad(&wr, sizeof(wr), ENJINN_ENGINE_SAVES_PATH "windowPos", false) != sfs::noError)
	{
		wr = {};
	}
#endif
	
	if (wr.x < 0 || wr.y < 0 || wr.z <= 0 || wr.w <= 0)
	{
		wr = {};
	}
	
	//todo debug from engine
	//glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

	//glfwWindowHint(GLFW_SAMPLES, 1);

	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	context.wind = glfwCreateWindow((std::max)(wr.z, 1280), (std::max)(wr.w, 720), "EnJinn", NULL, NULL);
	glfwSetWindowPos(context.wind, wr.x, wr.y);

	input.hasFocus = true;

	ENJINN_PERMA_ASSERT(context.wind, "problem initializing window");
	glfwMakeContextCurrent(context.wind);

	glfwSetWindowUserPointer(context.wind, this);

	// Load App Icon
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(false); // Ensure standard format for Windows Icon
		unsigned char* pixels = stbi_load(ENJINN_ENGINE_RESOURCES_PATH "onlylogo.png", &width, &height, &channels, 4);
		stbi_set_flip_vertically_on_load(true); // Restore engine default (assuming gl2d needs it)
		if (pixels)
		{
			GLFWimage images[1];
			images[0].width = width;
			images[0].height = height;
			images[0].pixels = pixels;
			glfwSetWindowIcon(context.wind, 1, images);
			stbi_image_free(pixels);
		}
		else {
			std::cout << "Failed to load App Icon: " << ENJINN_ENGINE_RESOURCES_PATH "onlylogo.png" << "\n";
		}
	}

	glfwSetMouseButtonCallback(context.wind, mouseCallback);
	glfwSetWindowFocusCallback(context.wind, windowFocusCallback);
	glfwSetCharCallback(context.wind, characterCallback);
	glfwSetKeyCallback(context.wind, keyCallback);


#if ENJINN_SHOULD_REMOVE_IMGUI == 0
	//todo macro
	context.imguiAllocator.init(malloc(enjinn::MB(20)), enjinn::MB(20));
#endif
	//HWND hwnd = glfwGetWin32Window(context.wind);

	//LONG exStyle = GetWindowLongPtr(hwnd, GWL_EXSTYLE);
	//exStyle &= ~WS_EX_APPWINDOW;
	//exStyle |= WS_EX_TOOLWINDOW;
	//exStyle |= WS_EX_CONTEXTHELP;
	//exStyle &= ~WS_MAXIMIZEBOX;
	//exStyle &= ~WS_MINIMIZEBOX;
	//SetWindowLongPtr(hwnd, GWL_EXSTYLE, exStyle);

	//LONG style = GetWindowLongPtr(hwnd, GWL_STYLE);
	//style &= ~WS_MAXIMIZEBOX;
	//style &= ~WS_MINIMIZEBOX;
	//style &= ~WS_CAPTION;
	//style |= WS_DLGFRAME;
	//SetWindowLongPtr(hwnd, GWL_STYLE, style);
	


	timer = std::chrono::high_resolution_clock::now();
}

void enjinn::EnJinnWindow::saveWindowPositions()
{
#if ENJINN_DEVELOPMENT

	WindowRect wr = {};

	glfwGetWindowPos(context.wind, &wr.x, &wr.y);

	wr.z = windowState.windowW;
	wr.w = windowState.windowH;

	if (!std::filesystem::is_directory(ENJINN_ENGINE_RESOURCES_PATH))
	{
		std::filesystem::create_directory(ENJINN_ENGINE_RESOURCES_PATH);
	}

	if (!std::filesystem::is_directory(ENJINN_ENGINE_SAVES_PATH))
	{
		std::filesystem::create_directory(ENJINN_ENGINE_SAVES_PATH);
	}

	sfs::safeSave(&wr, sizeof(wr), ENJINN_ENGINE_SAVES_PATH "windowPos", false);

#endif

}

bool enjinn::EnJinnWindow::shouldClose()
{
	return glfwWindowShouldClose(context.wind);
}

void enjinn::EnJinnWindow::update()
{
#pragma region deltaTime
	auto end = std::chrono::high_resolution_clock::now();
	input.deltaTime = (std::chrono::duration_cast<std::chrono::microseconds>(end - timer)).count() / 1000000.0f;
	timer = end;

	if (input.deltaTime > 1.f / 10) { input.deltaTime = 1.f / 10; }
#pragma endregion

#pragma region input

	auto processInputBefore = [](enjinn::Button &b)
	{
		b.setTyped(false);
	};

	processInputBefore(input.lMouse);
	processInputBefore(input.rMouse);

	for (int i = 0; i < Button::BUTTONS_COUNT; i++)
	{
		processInputBefore(input.buttons[i]);
	}

	memset(input.typedInput, 0, sizeof(input.typedInput));

	for (int i = 0; i < Input::MAX_CONTROLLERS_COUNT; i++)
	{
		for (int j = 0; j < Controller::Buttons::ButtonsCount; j++)
		{
			processInputBefore(input.controllers[i].buttons[j]);
		}
	}

#pragma endregion

	glfwPollEvents();
	glfwSwapBuffers(context.wind);

#pragma region window state

	{
		int w = 0;
		int h = 0;
		glfwGetWindowSize(context.wind, &w, &h);

		windowState.windowW = w;
		windowState.windowH = h;
		
		glfwGetFramebufferSize(context.wind, &w, &h);
		windowState.frameBufferW = w;
		windowState.frameBufferH = h;
	}

#pragma endregion


#pragma region input

	#pragma region controller
	for (int i = 0; i <= Input::MAX_CONTROLLERS_COUNT; i++)
	{
		if (glfwJoystickPresent(i) && glfwJoystickIsGamepad(i))
		{
			input.controllers[i].connected = true;

			GLFWgamepadstate state;

			if (glfwGetGamepadState(i, &state))
			{
				for (int b = 0; b <= GLFW_GAMEPAD_BUTTON_LAST; b++)
				{
					enjinn::processAButton(input.controllers[i].buttons[b], state.buttons[b]);

					//updateButton(controllerButtons.buttons[i], deltaTime); //todo implement double epressed
				}

				input.controllers[i].LT = state.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER];
				input.controllers[i].RT = state.axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER];

				input.controllers[i].LStick.x = state.axes[GLFW_GAMEPAD_AXIS_LEFT_X];
				input.controllers[i].LStick.y = state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y];

				input.controllers[i].RStick.x = state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X];
				input.controllers[i].RStick.y = state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y];
			}
		}
		else
		{
			input.controllers[i].resetAllButtons();
		}

	}

	#pragma endregion


	double mouseX = 0;
	double mouseY = 0;
	glfwGetCursorPos(context.wind, &mouseX, &mouseY);
	input.mouseX = (int)mouseX;
	input.mouseY = (int)mouseY;

	auto processInput = [](enjinn::Button &b)
	{

		if (!b.lastState() && b.held())
		{
			b.setPressed(true);
			b.setTyped(true);
		}
		else
		{
			b.setPressed(false);
		}

		if (b.lastState() && !b.held())
		{
			b.setReleased(true);
		}
		else
		{
			b.setReleased(false);
		}

		b.setLastState(b.held());

	};

	processInput(input.lMouse);
	processInput(input.rMouse);

	for (int i = 0; i < Button::BUTTONS_COUNT; i++)
	{
		processInput(input.buttons[i]);
	}

	#pragma region controller
	for (int i = 0; i < Input::MAX_CONTROLLERS_COUNT; i++)
	{
		if(input.controllers[i].connected)
		for (int b = 0; b <= GLFW_GAMEPAD_BUTTON_LAST; b++)
		{
			processInput(input.controllers[i].buttons[b]);
		}
	}

	input.anyController.resetAllButtons();
	for (int i = 0; i <= Input::MAX_CONTROLLERS_COUNT; i++)
	{
		input.controllers[i].connected = true;

		if (input.controllers[i].connected)
		{
			input.anyController.connected = true;

			for (int b = 0; b <= GLFW_GAMEPAD_BUTTON_LAST; b++)
			{
				input.anyController.buttons[b].flags
					|= input.controllers[i].buttons[b].flags;
			}

			if (!input.anyController.LT) input.anyController.LT = input.controllers[i].LT;
			if (!input.anyController.RT) input.anyController.RT = input.controllers[i].RT;

			if (!input.anyController.LStick.x) input.anyController.LStick.x = input.controllers[i].LStick.x;
			if (!input.anyController.LStick.y) input.anyController.LStick.y = input.controllers[i].LStick.y;

			if (!input.anyController.RStick.x)  input.anyController.RStick.x = input.controllers[i].RStick.x;
			if (!input.anyController.RStick.y) input.anyController.RStick.y = input.controllers[i].RStick.y;
		}

	}
	#pragma endregion


#pragma endregion

}
