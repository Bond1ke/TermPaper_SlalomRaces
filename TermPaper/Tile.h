#pragma once

#include "SDL2-2.0.22/include/SDL.h"
#include "GlobalVariable.h"

class Tile
{
public:
	//Initializes position and type
	Tile(int x, int y, int tileType);

	//Shows the tile
	void render(SDL_Rect& camera);

	//Get the tile type
	int getType();

	//Get the collision box
	SDL_Rect getBox();

private:
	//The attributes of the tile
	SDL_Rect mBox;

	//The tile type
	int mType;
};

bool checkCollision(SDL_Rect a, SDL_Rect b);//Box collision detector

bool touchesWall(SDL_Rect box, Tile* tiles[], const int TOTAL_TILES);//Checks collision box against set of tiles