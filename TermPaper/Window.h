#pragma once

#include "SDL2-2.0.22/include/SDL.h"
#include "SDL2_image-2.0.5/include/SDL_image.h"
#include <sstream>

class Window
{
public:
	//Intializes internals
	Window();

	//Creates window
	bool init();

	//Creates renderer from internal window
	SDL_Renderer* createRenderer();

	//Handles window events
	void handleEvent(SDL_Event& e);

	//Deallocates internals
	void free();

	//Window dimensions
	int getWidth();
	int getHeight();
	
	//Minimise Window
	bool isMinimized();

private:
	//Window data
	SDL_Window* WindowData;

	//Window dimensions
	int Width;
	int Height;

	//Window focus
	bool Minimized;
};