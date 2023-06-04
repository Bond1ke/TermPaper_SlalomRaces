#pragma once

#include "SDL2-2.0.22/include/SDL.h"
#include "SDL2_image-2.0.5/include/SDL_image.h"
#include "GlobalVariable.h"

class Menu
{
public:
	Menu();

	virtual void handle_Menu(SDL_Event& e, int& currentData);
	virtual void render();
	virtual void prerender();

	bool about_game();
	bool show_menu();
	void show_menu_setter(bool flag);

	void about_game_setter(bool flag);
	bool aboutGame;

	SDL_Color highlightColor;
	SDL_Color defaultColor;

private:
	int currentbutton;
	static const int MainMenuDataSize = 3;

	Texture MainMenuData[MainMenuDataSize];
	std::string MenuWords[MainMenuDataSize];
	SDL_Point Position [MainMenuDataSize];

	bool highlight;
	bool show;
	bool soundplay;
};