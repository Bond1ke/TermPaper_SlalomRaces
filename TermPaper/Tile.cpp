#include "Tile.h"


Tile::Tile(int x, int y, int tileType)
{
	//Get the offsets
	mBox.x = x;
	mBox.y = y;

	//Set the collision box
	mBox.w = TILE_WIDTH;
	mBox.h = TILE_HEIGHT;

	//Get the tile type
	mType = tileType;
}

void Tile::render(SDL_Rect& camera)
{
    //If the tile is on screen
    if (checkCollision(camera, mBox))
    {
        //Show the tile
        TileTexture.render(mBox.x - camera.x, mBox.y - camera.y, &TileClips[mType]);
    }
}

int Tile::getType()
{
	return mType;
}

SDL_Rect Tile::getBox()
{
	return mBox;
}

bool checkCollision(SDL_Rect a, SDL_Rect b)
{
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    //Calculate the sides of rect B
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

    //If any of the sides from A are outside of B
    if (bottomA <= topB)
    {
        return false;
    }

    if (topA >= bottomB)
    {
        return false;
    }

    if (rightA <= leftB)
    {
        return false;
    }

    if (leftA >= rightB)
    {
        return false;
    }

    //If none of the sides from A are outside B
    return true;
}

bool touchesWall(SDL_Rect box, Tile* tiles[], const int TOTAL_TILES)
{
    //Go through the tiles
    for (int i = 0; i < TOTAL_TILES; ++i)
    {
        //If the tile is a wall type tile
        if (tiles[i]->getType() == 0)
        {
            //If the collision box touches the wall tile
            if (checkCollision(box, tiles[i]->getBox()))
            {
                return true;
            }
        }

        //If the tile is a start line tile
        else if (tiles[i]->getType() == 4)
        {
            //if start has not been passed
            if (start_line.get_flag() == false)
            {
                //if the collision box touches the start line
                if (SDL_IntersectRectAndLine(&box, start_line.get_start_x(), start_line.get_start_y(), start_line.get_end_x(), start_line.get_end_y()) == true)
                {
                    gameIsStarted = true;
                    start_line.set_flag(true);
                    timer.start();
                    break;
                }
            }
            else
                continue;
        }

        //If the tile is a finish line tile
        else if (tiles[i]->getType() == 5)
        {
            //if finish has not been passed
            if (finish_line.get_flag() == false)
            {
                //if the collision box touches the finish line
                if (SDL_IntersectRectAndLine(&box, finish_line.get_start_x(), finish_line.get_start_y(), finish_line.get_end_x(), finish_line.get_end_y()) == true)
                {
                    gameIsFinished = true;
                    finish_line.set_flag(true);
                    timer.pause();
                    Mix_PlayChannel(-1, GameFinished, 0);
                    break;
                }
            }
            else
                continue;
        }
        
        //If the tiles are red flag or blue flag
        else if (tiles[i]->getType() == 3 || tiles[i]->getType() == 2)
        {
            //Go through the all gates (gate is a pair of flag of equal color)
            for (int i = 0; i < 21; ++i)
            {
                //if gates have not been passed
                if (all_lines[i].get_flag() == false)
                {
                    //if the collision box touches the gate line
                    if (SDL_IntersectRectAndLine(&box, all_lines[i].get_start_x(), all_lines[i].get_start_y(), all_lines[i].get_end_x(), all_lines[i].get_end_y()) == true)
                    {
                        Mix_PlayChannel(-1, ScoreEarned, 0);
                        all_lines[i].set_flag(true);
                        break;
                    }
                }
                else
                    continue;
            }
        }
    }

    //If no wall tiles were touched
    return false;
}