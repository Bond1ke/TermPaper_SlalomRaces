#pragma once

#include "GameFunctions.h"
#include "Lines.h"

class Player
{
public:
    //The dimensions of the player
    static const int PLAYER_WIDTH = 90;
    static const int PLAYER_HEIGHT = 90;

    //Maximum axis velocity of the player
    static const int PLAYER_VEL = 10;

    //Initializes the variables
    Player();

    //Takes key presses and adjusts the player's velocity
    void handleEvent(SDL_Event& e);

    void VelocitySetter(int x, int y);
    void CoordinatesSetter(int x, int y);

    //Moves the player
    void move(Tile* tiles[], const int TOTAL_TILES);

    //Centers the camera over the player
    void setCamera(SDL_Rect& camera);

    //Shows the player on the screen
    void render(SDL_Rect& camera);

    SDL_Rect* getPlayerBox();

private:
    //Collision box of the player
    SDL_Rect Box;

    //The velocity of the player
    int VelX, VelY;
};