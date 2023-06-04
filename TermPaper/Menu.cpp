#include "Menu.h"

Menu::Menu()
{
	highlight = false;
	show = true;

	defaultColor = { 0, 0, 0, 0xFF };
	highlightColor = { 0xFF, 0, 0, 0xFF };

	currentbutton = 0;

	MenuWords[0] = "Play";
	MenuWords[1] = "About Game";
	MenuWords[2] = "Exit";

	soundplay = false;

	prerender();
}

void Menu::handle_Menu(SDL_Event& e, int& currentData)
{
	if (highlight)	//if some button is highlighted
	{
		prerender();	//set default color
		highlight = false;
	}

	//handle keyboard input
	if (e.type == SDL_KEYDOWN && show == true)
	{
		switch (e.key.keysym.sym)
		{
			//Previous data entry
		case SDLK_UP:
			//Rerender previous entry input point
			MainMenuData[currentData].loadFromRenderedText(MenuWords[currentData].c_str(), defaultColor);
			--currentData;
			if (currentData < 0)
			{
				currentData = MainMenuDataSize - 1;
			}

			//Rerender current entry input point
			MainMenuData[currentData].loadFromRenderedText(MenuWords[currentData].c_str(), highlightColor);
			currentbutton = 1;
			Mix_PlayChannel(-1, ButtonDefault, 0);
			break;
			//Next data entry
		case SDLK_DOWN:
			//Rerender previous entry input point
			MainMenuData[currentData].loadFromRenderedText(MenuWords[currentData].c_str(), defaultColor);
			++currentData;
			if (currentData == MainMenuDataSize)
			{
				currentData = 0;
			}

			//Rerender current entry input point
			MainMenuData[currentData].loadFromRenderedText(MenuWords[currentData].c_str(), highlightColor);
			currentbutton = 2;
			Mix_PlayChannel(-1, ButtonDefault, 0);
			break;
		case SDLK_RETURN:
			if (currentData == 0)
			{
				gameIsStarted = true;
			}
			else if (currentData == 1)
			{
				gameAboutGame = true;
			}
			else if (currentData == 2)
			{
				quitTheGame = true;
			}
			MainMenuData[currentData].loadFromRenderedText(MenuWords[currentData].c_str(), defaultColor);
			show = false;
			Mix_PlayChannel(-1, ButtonPressed, 0);
			break;
		}
	}
	
	//If mouse event happened
	else if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP)
	{
		int num = -1;
		//Handle button events
		for (int i = 0; i < MainMenuDataSize; ++i)
		{

			//Get mouse position
			int x, y;
			SDL_GetMouseState(&x, &y);

			//Check if mouse is in button
			bool inside = true;
			bool flag = false;

			//Mouse is left of the button
			if (x < Position[i].x)
			{
				inside = false;
			}
			//Mouse is right of the button
			else if (x > Position[i].x + MainMenuData[i].getWidth())
			{
				inside = false;
			}
			//Mouse above the button
			else if (y < Position[i].y)
			{
				inside = false;
			}
			//Mouse below the button
			else if (y > Position[i].y + MainMenuData[i].getHeight())
			{
				inside = false;
			}

			//Mouse is outside button
			if (inside)
			{
				//Set mouse over sprite
				switch (e.type)
				{
				case SDL_MOUSEMOTION:
					MainMenuData[i].loadFromRenderedText(MenuWords[i].c_str(), highlightColor);
					break;
				case SDL_MOUSEBUTTONDOWN:
					MainMenuData[i].loadFromRenderedText(MenuWords[i].c_str(), highlightColor);
					Mix_PlayChannel(-1, ButtonPressed, 0);
					flag = true;
					break;
				}

				if (soundplay == true)	//if need to play the sound
				{
					Mix_PlayChannel(-1, ButtonDefault, 0);
					soundplay = false;
				}

				highlight = true;
				currentData = i;
			}
			else
			{
				MainMenuData[i].loadFromRenderedText(MenuWords[i].c_str(), defaultColor);
				if (i == currentData)	//if we are staying on the same button and mouse is outside the button, we set "soundplay" variable to "true"
					soundplay = true;
			}

			if (flag) 
			{
				switch (i)
				{
				case 0: gameIsStarted = true; break;
				case 1: gameAboutGame = true; break;
				case 2: quitTheGame = true; break;
				}
				show = false;
			}
		}
	}
}

void Menu::prerender()
{
	//this function sets default colour for all buttons

	for (int i = 0; i < MainMenuDataSize; ++i)
	{
		MainMenuData[i].loadFromRenderedText(MenuWords[i].c_str(), defaultColor);

		Position[i].x = (SCREEN_WIDTH - MainMenuData[i].getWidth()) / 2;
		Position[i].y = MainMenuData[0].getHeight() * 2 * i + SCREEN_HEIGHT / 3;
	}
}

void Menu::render()
{
	//Clear screen
	SDL_SetRenderDrawColor(Renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(Renderer);

	//Render text textures
	for (int i = 0; i < MainMenuDataSize; ++i)
	{
		MainMenuData[i].render(Position[i].x, Position[i].y);
	}

	//Update screen
	SDL_RenderPresent(Renderer);
}

bool Menu::show_menu()
{
	return show;
}

bool Menu::about_game()
{
	return aboutGame;
}

void Menu::show_menu_setter(bool flag)
{
	show = flag;
}

void Menu::about_game_setter(bool flag)
{
	aboutGame = flag;
}