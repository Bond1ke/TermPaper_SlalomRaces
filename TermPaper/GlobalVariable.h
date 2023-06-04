#pragma once

#include "SDL2-2.0.22/include/SDL.h"
#include "SDL2_mixer-2.0.4/include/SDL_mixer.h"
#include "SDL2_image-2.0.5/include/SDL_image.h"
#include "SDL2_ttf-2.19.3/include/SDL_ttf.h"
#include "Texture.h"
#include "Window.h"
#include "Lines.h"
#include "Timer.h"

extern SDL_Renderer* Renderer;
extern TTF_Font* Font;
extern Texture PlayerTexture;
extern Window window;

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

extern int LEVEL_WIDTH;
extern int LEVEL_HEIGHT;

extern int TILE_WIDTH;
extern int TILE_HEIGHT;
extern int TILE_SNOW;
extern int TILE_ROAD;
extern int TILE_RED_FLAG;
extern int TILE_BLUE_FLAG;
extern int TILE_REDLINE;
extern int TILE_BLUELINE;

extern Texture TileTexture;
extern SDL_Rect TileClips[6];

extern SDL_Rect PlayerClips[3];
extern SDL_Rect* currentClip;
extern Texture PlayerTexture;

extern bool gameIsStarted;
extern bool gameIsPaused;
extern bool gameAboutGame;
extern bool gameSettings;
extern bool quitTheGame;
extern bool continueGame;
extern bool gameIsFinished;
extern bool enterIsPressed;

extern Lines all_lines[21];
extern Lines all_lines_copy[21];

extern Timer timer;
extern Texture TimeTextTexture;

extern std::stringstream timeText;
extern std::stringstream scoreText;

extern Lines start_line;
extern Lines start_line_copy;

extern Lines finish_line;
extern Lines finish_line_copy;

extern int score;

extern Mix_Chunk* ButtonDefault;
extern Mix_Chunk* ButtonPressed;
extern Mix_Chunk* PausePressed;
extern Mix_Chunk* GameFinished;
extern Mix_Chunk* ScoreEarned;