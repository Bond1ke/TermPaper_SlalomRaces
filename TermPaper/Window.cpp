#include "Window.h"
#include "GlobalVariable.h"

Window::Window()
{
	//Initialize non-existant window
	WindowData = NULL;
	Minimized = false;
	Width = 0;
	Height = 0;
}

bool Window::init()
{
	//Create window
	WindowData = SDL_CreateWindow("Slalom/Racing", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (WindowData != NULL)
	{
		Width = SCREEN_WIDTH;
		Height = SCREEN_HEIGHT;
	}

	return WindowData != NULL;
}

SDL_Renderer* Window::createRenderer()
{
	return SDL_CreateRenderer(WindowData, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

void Window::handleEvent(SDL_Event& e)
{
	//Window event occured
	if (e.type == SDL_WINDOWEVENT)
	{
		switch (e.window.event)
		{
			//Window minimized
		case SDL_WINDOWEVENT_MINIMIZED:
			Minimized = true;
			timer.pause();
			break;

			//Window maxized
		case SDL_WINDOWEVENT_MAXIMIZED:
			Minimized = false;
			timer.unpause();
			break;

			//Window restored
		case SDL_WINDOWEVENT_RESTORED:
			Minimized = false;
			if (gameIsPaused != true)
				timer.unpause();
			break;
		}
	}
}

void Window::free()
{
	if (WindowData != NULL)
	{
		SDL_DestroyWindow(WindowData);
	}

	Width = 0;
	Height = 0;
}

int Window::getWidth()
{
	return Width;
}

int Window::getHeight()
{
	return Height;
}

bool Window::isMinimized()
{
	return Minimized;
}