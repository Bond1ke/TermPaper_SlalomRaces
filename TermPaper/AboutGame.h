#pragma once

#include "Menu.h"
#include "GlobalVariable.h"
#include "SDL2-2.0.22/include/SDL.h"
#include "SDL2_image-2.0.5/include/SDL_image.h"
#include <fstream>
#include <iostream>
#include <string>

class AboutGame : public Menu
{
public:
	AboutGame();

	void handle_Menu(SDL_Event& e, Menu& menu);
	void render() override;

	bool about_Game_getter();

	int CountLinesInFile(char* filename);
	int GetStringsFromFileS(char* filename, std::string** _lines, int n);

private:
	std::string* lines;	//список строк типа string*
	int countlines;	//кол-во строк
	Texture info;	//текстура текста
	bool about_Game_flag;	//выйти в главное меню?
};