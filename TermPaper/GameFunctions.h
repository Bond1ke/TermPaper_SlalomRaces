#pragma once

#include "Menu.h"
#include "Tile.h"
#include <iostream>
#include <fstream>
#include <iostream>

bool init(); //initialize all subsystems

bool loadMedia(Tile* tiles[], const int TOTAL_TILES, const int TOTAL_TILE_SPRITES);	//Loads media
bool setTiles(Tile* tiles[], const int TOTAL_TILES, const int TOTAL_TILE_SPRITE);	//Sets tiles from tile map

void updateTheGame(Menu& menu);	//update some values to load the game again

void close();	//Frees media and shuts down SDL

void copy_array(Lines original[], Lines copied[], const int TOTAL_TILES, const int TOTAL_TILE_SPRITES);	//copies an original array of gate collision lines
void set_array(Lines original[], Lines copied[], const int TOTAL_TILES, const int TOTAL_TILE_SPRITES);	//sets for original array initial values