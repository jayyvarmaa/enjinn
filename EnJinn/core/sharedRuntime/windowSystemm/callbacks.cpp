#include "callbacks.h"
#include "window.h"
#include <string.h>

void addTypedInput(enjinn::Input &input, unsigned int c)
{
	if (c < 127)
	{
		auto l = strlen(input.typedInput);
		if (l < sizeof(input.typedInput) - 1)
		{
			input.typedInput[l++] = c;
			input.typedInput[l] = 0;
		}
	}
}

void characterCallback(GLFWwindow *window, unsigned int codepoint)
{
	auto ptr = glfwGetWindowUserPointer(window);
	enjinn::EnJinnWindow &enjinnWindow = *(enjinn::EnJinnWindow *)ptr;

	addTypedInput(enjinnWindow.input, codepoint);
}

void windowFocusCallback(GLFWwindow *window, int focused)
{
	auto ptr = glfwGetWindowUserPointer(window);
	enjinn::EnJinnWindow &enjinnWindow = *(enjinn::EnJinnWindow *)ptr;

	if (focused)
	{
		enjinnWindow.input.hasFocus = 1;
	}
	else
	{
		enjinnWindow.input.hasFocus = 0;
	}
}

void enjinn::processAButton(enjinn::Button &b, int action)
{
	if (action == GLFW_PRESS)
	{
		b.setHeld(true);

	}
	else if (action == GLFW_RELEASE)
	{
		b.setHeld(false);
	}
	else if (action == GLFW_REPEAT)
	{
		b.setHeld(true);
		b.setTyped(true);
	}

};

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	auto ptr = glfwGetWindowUserPointer(window);
	enjinn::EnJinnWindow &enjinnWindow = *(enjinn::EnJinnWindow *)ptr;

	if ((action == GLFW_REPEAT || action == GLFW_PRESS) && key == GLFW_KEY_BACKSPACE)
	{
		addTypedInput(enjinnWindow.input, 8);
	}

	if(key >= GLFW_KEY_A && key <= GLFW_KEY_Z)
	{
		int index = key - GLFW_KEY_A;
		processAButton(enjinnWindow.input.buttons[enjinn::Button::A + index], action);
	}else if (key >= GLFW_KEY_0 && key <= GLFW_KEY_9)
	{
		int index = key - GLFW_KEY_0;
		processAButton(enjinnWindow.input.buttons[enjinn::Button::NR0 + index], action);
	}else
	{

		if (key == GLFW_KEY_SPACE)
		{
			processAButton(enjinnWindow.input.buttons[enjinn::Button::Space], action);
		}
		else
		if (key == GLFW_KEY_ENTER)
		{
			processAButton(enjinnWindow.input.buttons[enjinn::Button::Enter], action);
		}
		else
		if (key == GLFW_KEY_ESCAPE)
		{
			processAButton(enjinnWindow.input.buttons[enjinn::Button::Escape], action);
		}
		else
		if (key == GLFW_KEY_UP)
		{
			processAButton(enjinnWindow.input.buttons[enjinn::Button::Up], action);
		}
		else
		if (key == GLFW_KEY_DOWN)
		{
			processAButton(enjinnWindow.input.buttons[enjinn::Button::Down], action);
		}
		else
		if (key == GLFW_KEY_LEFT)
		{
			processAButton(enjinnWindow.input.buttons[enjinn::Button::Left], action);
		}
		else
		if (key == GLFW_KEY_RIGHT)
		{
			processAButton(enjinnWindow.input.buttons[enjinn::Button::Right], action);
		}
		else
		if (key == GLFW_KEY_LEFT_CONTROL)
		{
			processAButton(enjinnWindow.input.buttons[enjinn::Button::LeftCtrl], action);
		}else
		if (key == GLFW_KEY_TAB)
		{
			processAButton(enjinnWindow.input.buttons[enjinn::Button::Tab], action);
		}else
		if (key == GLFW_KEY_LEFT_ALT)
		{
			processAButton(enjinnWindow.input.buttons[enjinn::Button::LeftAlt], action);
		}
	}

}

void mouseCallback(GLFWwindow *window, int key, int action, int mods)
{

	auto ptr = glfwGetWindowUserPointer(window);
	enjinn::EnJinnWindow &enjinnWindow = *(enjinn::EnJinnWindow *)ptr;

	if (key == GLFW_MOUSE_BUTTON_LEFT)
	{
		processAButton(enjinnWindow.input.lMouse, action);
	}
	else if(key == GLFW_MOUSE_BUTTON_RIGHT)
	{
		processAButton(enjinnWindow.input.rMouse, action);
	}


}
