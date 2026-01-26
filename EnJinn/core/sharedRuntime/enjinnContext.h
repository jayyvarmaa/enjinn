#pragma once

#include <enjinnAllocator/freeListAllocator.h>

struct GLFWwindow;
struct ImGuiContext;
//void glfwMakeContextCurrent(GLFWwindow *handle)

namespace enjinn
{
	struct EnJinnContext
	{
		//using glfwMakeContextCurrent_t = decltype(glfwMakeContextCurrent);
		using glfwMakeContextCurrent_t = void(GLFWwindow *);

		glfwMakeContextCurrent_t *glfwMakeContextCurrentPtr = {};
		GLFWwindow *wind = {};
		ImGuiContext *ImGuiContext = {};
		void* userImguiState = nullptr; // Added for enjinnImgui

		enjinn::memory::FreeListAllocator imguiAllocator;
	};
};