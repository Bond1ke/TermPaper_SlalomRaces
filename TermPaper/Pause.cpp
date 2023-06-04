#include "Pause.h"

Pause::Pause()
{
	highlight = false;

	defaultColor = { 0, 0, 0, 0xFF };
	highlightColor = { 0xFF, 0, 0, 0xFF };

	currentbutton = 0;

	MenuWords[0] = "Continue";
	MenuWords[1] = "About Game";
	MenuWords[2] = "Exit";

	Message.loadFromRenderedText("PRESS ENTER TO START THE GAME", highlightColor);

	soundplay = false;

	prerender();
}

void Pause::handle_Pause(SDL_Event& e, Menu& menu, Player& skier)
{
	//handle PRESS ENTER TO START THE GAME
	if (enterIsPressed == false)	//if game has not been started yet
	{
		if (e.type == SDL_KEYDOWN)
		{
			switch (e.key.keysym.sym)
			{
			case SDLK_RETURN:	//when user presses ENTER
				enterIsPressed = true;	
				skier.VelocitySetter(0, 13);
				Mix_PlayChannel(-1, ButtonPressed, 0);
				break;
			}
		}
	}

	//handle ESCAPE button during the game proccess
	else if (enterIsPressed == true && gameIsFinished == false)
	{
		if (e.type == SDL_KEYDOWN)
		{
			switch (e.key.keysym.sym)
			{
			case SDLK_ESCAPE:	//when user presses ESCAPE
				skier.VelocitySetter(0, 13);
				timer.pause();
				gameIsPaused = true;	//Pause the game
				continueGame = false;
				gameAboutGame = false;
				quitTheGame = false;
				Mix_PlayChannel(-1, ButtonPressed, 0);
				break;
			}
		}
	}

	//handle PRESS ENTER TO EXIT TO THE MAIN MENU
	else if (gameIsFinished == true)
	{
		if (e.type == SDL_KEYDOWN)
		{
			switch (e.key.keysym.sym)
			{
			case SDLK_RETURN:	//when user presses ENTER
				skier.VelocitySetter(0, 13);
				skier.CoordinatesSetter(LEVEL_WIDTH / 2, 10);
				updateTheGame(menu);
				Mix_PlayChannel(-1, ButtonPressed, 0);
				break;
			}
		}
	}
}

void Pause::render_press_to_start()
{
	Message.loadFromRenderedText("PRESS ENTER TO START THE GAME", highlightColor);
	Message.render(590, 500);
	SDL_RenderPresent(Renderer);
}

void Pause::render()
{
	//render such buttons as "Continue", "About Game, "Exit"
	for (int i = 0; i < MainMenuDataSize; ++i)
		MainMenuData[i].render(Position[i].x, Position[i].y);

	SDL_RenderPresent(Renderer);
}

void Pause::prerender()
{
	//this function sets default colour for all buttons

	for (int i = 0; i < MainMenuDataSize; ++i)
	{
		MainMenuData[i].loadFromRenderedText(MenuWords[i].c_str(), defaultColor);

		Position[i].x = (SCREEN_WIDTH - MainMenuData[i].getWidth()) / 2;
		Position[i].y = MainMenuData[0].getHeight() * 2 * i + SCREEN_HEIGHT / 3;
	}
}

void Pause::handle_Menu(SDL_Event& e, int& currentData)
{
	if (highlight)	//if some button is highlighted
	{
		prerender();	//set default color
		highlight = false;
	}

	//handle keyboard input
	if (e.type == SDL_KEYDOWN)
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
				continueGame = true;
				gameIsPaused = false;
				timer.unpause();
			}
			else if (currentData == 1)
			{
				aboutGame = true;
				gameAboutGame = true;
			}
			else if (currentData == 2)
			{
				quitTheGame = true;
			}
			MainMenuData[currentData].loadFromRenderedText(MenuWords[currentData].c_str(), defaultColor);
			Mix_PlayChannel(-1, ButtonPressed, 0);
			break;
		}
	}

	//If mouse event happened
	else if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP)
	{
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

			if (inside)	//if mouse is inside the button
			{
				//Set mouse over sprite
				switch (e.type)
				{
				case SDL_MOUSEMOTION:
					MainMenuData[i].loadFromRenderedText(MenuWords[i].c_str(), highlightColor);
					break;
				case SDL_MOUSEBUTTONDOWN:
					MainMenuData[i].loadFromRenderedText(MenuWords[i].c_str(), highlightColor);
					flag = true;
					Mix_PlayChannel(-1, ButtonPressed, 0);
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
				case 0:
				{
					continueGame = true;
					gameIsPaused = false;
					timer.unpause();
					break;
				}
				case 1: gameAboutGame = true; break;
				case 2: quitTheGame = true; break;
				}
			}
		}
	}
}