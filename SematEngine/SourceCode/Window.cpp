#include "Window.h"

Window::Window(bool _active) : active(_active)
{

}

Window::~Window()
{

}

void Window::SetActive()
{
	active = !active;
}

void Window::Draw() {};

bool Window::IsActive() const
{
	return active;
}