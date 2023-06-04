#include "Player.h"

Player::Player()
{
    //Initialize the collision box
    Box.x = LEVEL_WIDTH/2;
    Box.y = 10;
    Box.w = PLAYER_WIDTH;
    Box.h = PLAYER_HEIGHT;

    //Initialize the velocity
    VelX = 0;
    VelY = 13;
}

SDL_Rect* Player::getPlayerBox()
{
    return &Box;
}

void Player::handleEvent(SDL_Event& e)
{
    ///when key is pressed
    if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
    {
        //Adjust velocity
        switch (e.key.keysym.sym)
        {
        case SDLK_LEFT: 
            VelX -= PLAYER_VEL;
            VelY -= 2;
            break;
        case SDLK_RIGHT: 
            VelX += PLAYER_VEL;
            VelY -= 2;
            break;
        }
    }
    
    //when key is released
    else if (e.type == SDL_KEYUP && e.key.repeat == 0)
    {
        //Adjust velocity
        switch (e.key.keysym.sym)
        {
        case SDLK_LEFT: 
            VelX += PLAYER_VEL;
            VelY += 2;
            break;
        case SDLK_RIGHT: 
            VelX -= PLAYER_VEL;
            VelY += 2;
            break;
        }
    }
}

void Player::move(Tile* tiles[], const int TOTAL_TILES)
{
    //Move the player left or right
    Box.x += VelX;

    //If the player went too far to the left or right or touched a wall
    if ((Box.x < 0) || (Box.x + PLAYER_WIDTH > LEVEL_WIDTH) || touchesWall(Box, tiles, TOTAL_TILES))
    {
        //move back
        Box.x -= VelX;
    }

    //Move the player up or down
    Box.y += VelY;

    if ((Box.y < 0) || touchesWall(Box, tiles, TOTAL_TILES))
    {
        //move back
        Box.y -= VelY;
    }

}

void Player::render(SDL_Rect& camera)
{
    //Show the player
    if (VelX < 0)
        currentClip = &PlayerClips[0];
    else if (VelX > 0)
        currentClip = &PlayerClips[2];
    else if (VelX == 0)
        currentClip = &PlayerClips[1];

    PlayerTexture.render(Box.x - camera.x, Box.y - camera.y, currentClip);
}

void Player::setCamera(SDL_Rect& camera)
{
    //Center the camera over the player
    camera.x = (Box.x + PLAYER_WIDTH / 2) - SCREEN_WIDTH / 2;
    camera.y = (Box.y + PLAYER_HEIGHT / 2) - SCREEN_HEIGHT / 2;

    //Keep the camera in bounds
    if (camera.x < 0)
    {
        camera.x = 0;
    }
    if (camera.y < 0)
    {
        camera.y = 0;
    }
    if (camera.x > LEVEL_WIDTH - camera.w)
    {
        camera.x = LEVEL_WIDTH - camera.w;
    }
    if (camera.y > LEVEL_HEIGHT - camera.h)
    {
        camera.y = LEVEL_HEIGHT - camera.h;
    }
}

void Player::VelocitySetter(int x, int y)
{
    VelX = x;
    VelY = y;
}

void Player::CoordinatesSetter(int x, int y)
{
    Box.x = x;
    Box.y = y;
}