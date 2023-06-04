#pragma once

#include "Player.h"
#include "AboutGame.h"

class Pause: public Menu
{
public:
	Pause();

	void handle_Pause(SDL_Event& e, Menu& menu, Player& skier);
	void handle_Menu(SDL_Event& e, int& currentData) override;

	void render() override;	//render all buttons on the screen
	void prerender() override;	//set default colour

	void render_press_to_start();

private:
	int currentbutton;

	static const int MainMenuDataSize = 3;

	Texture MainMenuData[MainMenuDataSize];
	std::string MenuWords[MainMenuDataSize];
	SDL_Point Position[MainMenuDataSize];

	Texture Message;

	bool highlight;
	bool soundplay;
};