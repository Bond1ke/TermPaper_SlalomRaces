#pragma once

#include "SDL2-2.0.22/include/SDL.h"
#include "SDL2_image-2.0.5/include/SDL_image.h"
#include "SDL2_ttf-2.19.3/include/SDL_ttf.h"
#include <iostream>

class Texture
{
public:
    //Initializes variables
    Texture();

    //Deallocates memory
    ~Texture();

    //Loads image at specified path
    bool loadFromFile(std::string path);

    //Creates image from font string
    bool loadFromRenderedText(std::string textureText, SDL_Color textColor);

    //Deallocates texture
    void free();

    //Renders texture at given point
    void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

    //Gets image dimensions
    int getWidth();
    int getHeight();

private:

    //The actual hardware texture
    SDL_Texture* ActualTexture;

    //Image dimensions
    int Width;
    int Height;
};
