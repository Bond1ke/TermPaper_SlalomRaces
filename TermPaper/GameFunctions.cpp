#include "GameFunctions.h"

bool init()
{
    std::cout << "Initialising SDL..." << std::endl;

    //Initialization flag
    bool success = true;

    //Initialize SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1)//check all subsystems
    {
        printf("Could not initialize SDL! SDL_Error: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        //Set texture filtering to linear
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
        {
            printf("Warning: Linear texture filtering not enabled!");
        }

        //Create window
        if (!window.init())
        {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            //Create vsynced renderer for window
            Renderer = window.createRenderer();
            if (Renderer == NULL)
            {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                success = false;
            }
            else
            {
                //Initialize renderer color
                SDL_SetRenderDrawColor(Renderer, 0xFF, 0xFF, 0xFF, 0xFF);

                //Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags))
                {
                    printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                    success = false;
                }

                //Initialize SDL_ttf
                if (TTF_Init() == -1)
                {
                    printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
                    success = false;
                }

                //Initialize SDL_mixer
                if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
                {
                    printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
                    success = false;
                }
            }
        }
    }

    return success;
}

bool loadMedia(Tile* tiles[], const int TOTAL_TILES, const int TOTAL_TILE_SPRITES)
{
    //Loading success flag
    bool success = true;

    //Loading Font
    Font = TTF_OpenFont("Assets/ProggyClean.ttf", 50);
    if (Font == NULL)
    {
        printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
        success = false;
    }

    //Load music
    ButtonDefault = Mix_LoadWAV("Assets/button_default.wav");
    if (ButtonDefault == NULL)
    {
        printf("Failed to load default button sound! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }
    //button_pressed
    ButtonPressed = Mix_LoadWAV("Assets/button_pressed.wav");
    if (ButtonDefault == NULL)
    {
        printf("Failed to load default button sound! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }

    PausePressed = Mix_LoadWAV("Assets/pause.wav");
    if (PausePressed == NULL)
    {
        printf("Failed to load default button sound! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }

    GameFinished = Mix_LoadWAV("Assets/game_finished.wav");
    if (GameFinished == NULL)
    {
        printf("Failed to load default button sound! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }

    ScoreEarned = Mix_LoadWAV("Assets/score.wav");
    if (ScoreEarned == NULL)
    {
        printf("Failed to load default button sound! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }

        //Load sprite sheet texture
    if (!PlayerTexture.loadFromFile("Assets/skier.png"))
    {
        printf("Failed to load walking animation texture!\n");
        success = false;
    }
    else
    {
        //Set sprite clips
        PlayerClips[0].x = 0;
        PlayerClips[0].y = 0;
        PlayerClips[0].w = 90;
        PlayerClips[0].h = 90;

        PlayerClips[1].x = 90;
        PlayerClips[1].y = 0;
        PlayerClips[1].w = 90;
        PlayerClips[1].h = 90;

        PlayerClips[2].x = 180;
        PlayerClips[2].y = 0;
        PlayerClips[2].w = 90;
        PlayerClips[2].h = 90;
    }

    //Load tile texture
    if (!TileTexture.loadFromFile("Assets/spritelist.png"))
    {
        printf("Failed to load tile set texture!\n");
        success = false;
    }

    //Load tile map
    if (!setTiles(tiles, TOTAL_TILES, TOTAL_TILE_SPRITES))
    {
        printf("Failed to load tile set!\n");
        success = false;
    }

    return success;
}

bool setTiles(Tile* tiles[], const int TOTAL_TILES, const int TOTAL_TILE_SPRITES)
{
    //Success flag
    bool tilesLoaded = true;

    //The tile offsets
    int x = 0, y = 0;

    int bstartx, bstarty, bendx, bendy;
    int rstartx, rstarty, rendx, rendy;

    int index = 0;

    bool first_flag_red = false;
    bool first_flag_blue = false;

    bool red_flag = false;
    bool blue_flag = false;

    //Open the map
    std::ifstream map("Assets/slalom.map");

    //If the map couldn't be loaded
    if (map.fail())
    {
        printf("Unable to load map file!\n");
        tilesLoaded = false;
    }
    else
    {
        //Initialize the tiles
        for (int i = 0; i < TOTAL_TILES; ++i)
        {
            //Determines what kind of tile will be made
            int tileType = -1;

            //Read type of tile from map file
            map >> tileType;

            //If the was a problem in reading the map
            if (map.fail())
            {
                //Stop loading map
                printf("Error loading map: Unexpected end of file!\n");
                tilesLoaded = false;
                break;
            }

            //If the number is a valid tile number
            if ((tileType >= 0) && (tileType < TOTAL_TILE_SPRITES))
            {
                //allocate memory and store the address in a pointer
                tiles[i] = new Tile(x, y, tileType);//setting coordinates and type of the tile    

                if (tileType == 2 && first_flag_red == false)//red flag
                {
                    rstartx = x + TILE_WIDTH/2;
                    rstarty = y + TILE_WIDTH/2;
                    first_flag_red = true;
                }
                else if(tileType == 2 && first_flag_red == true)
                {
                    rendx = x + TILE_WIDTH/2;
                    rendy = y + TILE_WIDTH/2;
                    first_flag_red = false;

                    all_lines[index] = Lines(rstartx, rstarty, rendx, rendy);
                    ++index;
                }
                else if (tileType == 3 && first_flag_blue == false)//blue flag
                {
                    bstartx = x + TILE_WIDTH/2;
                    bstarty = y + TILE_WIDTH/2;
                    first_flag_blue = true;
                }
                else if (tileType == 3 && first_flag_blue == true)
                {
                    bendx = x + TILE_WIDTH/2;
                    bendy = y + TILE_WIDTH/2;
                    first_flag_blue = false;

                    all_lines[index] = Lines(bstartx, bstarty, bendx, bendy);
                    ++index;
                }
            }
            //If we don't recognize the tile type
            else
            {
                //Stop loading map
                printf("Error loading map: Invalid tile type at %d!\n", i);
                tilesLoaded = false;
                break;
            }

            //Move to next tile spot
            x += TILE_WIDTH;

            //If we've gone too far
            if (x >= LEVEL_WIDTH)
            {
                //Move back
                x = 0;

                //Move to the next row
                y += TILE_HEIGHT;
            }
        }

        //Clip the sprite sheet
        if (tilesLoaded)
        {
            //after we succesfully loaded our tiles (pointers on objects) in array, 
            //we need to set dimensions (size and coordinates) for our sprites texture
            //we actually have a global variable for that - TileClips[type]

            TileClips[TILE_ROAD].x = 0;
            TileClips[TILE_ROAD].y = 0;
            TileClips[TILE_ROAD].w = TILE_WIDTH;
            TileClips[TILE_ROAD].h = TILE_HEIGHT;

            TileClips[TILE_SNOW].x = 120;
            TileClips[TILE_SNOW].y = 0;
            TileClips[TILE_SNOW].w = TILE_WIDTH;
            TileClips[TILE_SNOW].h = TILE_HEIGHT;

            TileClips[TILE_BLUE_FLAG].x = 0;
            TileClips[TILE_BLUE_FLAG].y = 120;
            TileClips[TILE_BLUE_FLAG].w = TILE_WIDTH;
            TileClips[TILE_BLUE_FLAG].h = TILE_HEIGHT;

            TileClips[TILE_RED_FLAG].x = 120;
            TileClips[TILE_RED_FLAG].y = 120;
            TileClips[TILE_RED_FLAG].w = TILE_WIDTH;
            TileClips[TILE_RED_FLAG].h = TILE_HEIGHT;

            TileClips[TILE_REDLINE].x = 240;
            TileClips[TILE_REDLINE].y = 0;
            TileClips[TILE_REDLINE].w = TILE_WIDTH;
            TileClips[TILE_REDLINE].h = TILE_HEIGHT;

            TileClips[TILE_BLUELINE].x = 240;
            TileClips[TILE_BLUELINE].y = 120;
            TileClips[TILE_BLUELINE].w = TILE_WIDTH;
            TileClips[TILE_BLUELINE].h = TILE_HEIGHT;
        }
    }

    //Close the file
    map.close();

    //If the map was loaded fine
    return tilesLoaded;
}

void updateTheGame(Menu& menu)
{
    struct start_Line   //set default start line values
    {
        int start_LineX1 = 0;
        int start_LineY1 = 300;
        int start_LineX2 = LEVEL_WIDTH;
        int start_LineY2 = 300;
    };
    start_Line start_model; //create defalut start line

    struct finish_Line  //set default finish line values
    {
        int start_LineX1 = 0;
        int start_LineY1 = 10620;
        int start_LineX2 = LEVEL_WIDTH;
        int start_LineY2 = 10620;
    };
    finish_Line finish_model;   //create defalut finish line

    //update the variables
    menu.show_menu_setter(true);
    gameIsStarted = false;
    gameIsPaused = false;
    gameAboutGame = false;
    gameSettings = false;
    quitTheGame = false;
    continueGame = false;
    gameIsFinished = false;
    enterIsPressed = false;

    //clear the time and score text
    timeText.str(std::string());
    scoreText.str(std::string());

    score = 0;
    timer.stop();

    for (int i = 0; i < 21; ++i)
    {
        all_lines[i].set_flag(false);
        all_lines[i].set_counted(false);
    }

    finish_line.set_flag(false);
    start_line.set_flag(false);

    set_array(all_lines, all_lines_copy, 2160, 6);
    
    start_line.setline(start_model.start_LineX1, start_model.start_LineY1, start_model.start_LineX2, start_model.start_LineY2);
    finish_line.setline(finish_model.start_LineX1, finish_model.start_LineY1, finish_model.start_LineX2, finish_model.start_LineY2);
}

void copy_array(Lines original[], Lines copied[], const int TOTAL_TILES, const int TOTAL_TILE_SPRITES)
{
    for (int i = 0; i < 21; ++i)
    {
        copied[i] = original[i];
    }
}

void set_array(Lines original[], Lines copied[], const int TOTAL_TILES, const int TOTAL_TILE_SPRITES)
{
    for (int i = 0; i < 21; ++i)
    {
        original[i] = copied[i];
    }
}

void close()
{
    std::cout << "Quiting SDL..." << std::endl;

    //Destroy window    
    SDL_DestroyRenderer(Renderer);
    Renderer = NULL;

    PlayerTexture.free();
    TileTexture.free();

    Mix_FreeChunk(ButtonDefault);
    Mix_FreeChunk(ButtonPressed);
    ButtonDefault = NULL;
    ButtonPressed = NULL;

    window.free();
        
    //Quit SDL subsystems
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    Mix_Quit();
}