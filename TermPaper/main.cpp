#include "GameFunctions.h"
#include "Player.h"
#include "Timer.h"
#include "Menu.h"
#include "Pause.h"

int SCREEN_WIDTH = 1920, SCREEN_HEIGHT = 1080;  //Screen dimension constants
int LEVEL_WIDTH = 2880, LEVEL_HEIGHT = 10800;   //The dimensions of the level

SDL_Renderer* Renderer = nullptr;   //The window renderer
TTF_Font* Font = nullptr;   //Globally used font
Texture DotTexture, TimeTextTexture, ScoreTextTexture;
Window window;  //Custom window

SDL_Rect camera = { 0, 0, 1920, 1080};  //initialize the camera

int TILE_WIDTH = 120, TILE_HEIGHT = 120;    //set tile size
const int TOTAL_TILES = 2160, TOTAL_TILE_SPRITES = 6;

//The different tile sprites
int TILE_SNOW = 0;
int TILE_ROAD = 1;
int TILE_RED_FLAG = 2;
int TILE_BLUE_FLAG = 3;
int TILE_REDLINE = 4;
int TILE_BLUELINE = 5;

Texture TileTexture;   
SDL_Rect TileClips[TOTAL_TILE_SPRITES];

Tile* tileSet[TOTAL_TILES]; //declare a massive of pointers on tiles

SDL_Rect PlayerClips[3];
SDL_Rect* currentClip;
Texture PlayerTexture;

Lines start_line(0, 300, LEVEL_WIDTH, 300);
Lines finish_line(0, 10620, LEVEL_WIDTH, 10620);
Lines all_lines[21];    //lines for gates
Lines all_lines_copy[21];


bool gameIsStarted = false;
bool gameIsPaused = false;
bool gameAboutGame = false;
bool gameSettings = false;
bool quitTheGame = false;
bool continueGame = false;
bool gameIsFinished = false;
bool enterIsPressed = false;

int score = 0;
Timer timer;

std::stringstream timeText, scoreText;  //In memory text stream

//The sound effects that will be used
Mix_Chunk* ButtonDefault = NULL;
Mix_Chunk* ButtonPressed = NULL;
Mix_Chunk* PausePressed = NULL;
Mix_Chunk* GameFinished = NULL;
Mix_Chunk* ScoreEarned = NULL;

int main(int argc, char* args[])
{
    if (!init())    //Start up SDL and create window
    {
        printf("Failed to initialize SDL!\n");
    }
    else
    {
        std::cout << "SDL initialised succesfully!" << std::endl;

        if (!loadMedia(tileSet, TOTAL_TILES, TOTAL_TILE_SPRITES)) //Load media (this function also does include setting of our map)
        {
            printf("Failed to load media!\n");
        }
        else
        {
            copy_array(all_lines, all_lines_copy, TOTAL_TILES, TOTAL_TILE_SPRITES);

            bool quit = false;  //Main loop flag
            
            SDL_Event e;    //Event handler

            Menu menu;  //Main menu

            SDL_Color textColor = { 0, 0, 0, 255 };

            AboutGame aboutGameMenu;    //Object that includes information about game

            Pause pause;    //Pause menu

            int currentData = 0;

            Player skier;   //The dot that will be moving around on the screen

            Mix_Volume(-1, 30);

            while (!quit)   //While application is running
            {
                if (enterIsPressed == true) //if game has been started -> count score
                {
                    for (int i = 0; i < 21; ++i)
                    {
                        if (all_lines[i].get_flag() == true && all_lines[i].get_counted() == false) //if gate has been passed and has not been counted yet
                        {
                            score += 100;
                            all_lines[i].set_counted(true);
                            
                        }
                    }
                }
                
                while (SDL_PollEvent(&e) != 0)  //Handle events on queue
                {
                    if (e.type == SDL_QUIT || quitTheGame == true)  //If user requests quit
                        quit = true; 

                    window.handleEvent(e);  //Handle window events

                    if (menu.show_menu() == true && enterIsPressed == false) 
                        menu.handle_Menu(e, currentData);   //Menu events

                    else if (gameAboutGame == true) 
                        aboutGameMenu.handle_Menu(e, menu); //"About game" events

                    //if user requests to play the game
                    else if (enterIsPressed == true && gameIsFinished == false && (gameIsStarted == true && gameIsPaused == false || continueGame == true && gameIsPaused == false))
                    {
                        skier.handleEvent(e);   //move player
                        pause.handle_Pause(e, menu, skier); //check "Escape" button
                    }

                    else if (enterIsPressed == true && gameIsStarted == true && gameIsFinished == false && gameIsPaused == true)
                    {
                        pause.handle_Menu(e, currentData);  //"Pause" events
                    }

                    else if (enterIsPressed == false)
                    {
                        pause.handle_Pause(e, menu, skier); //PRESS ENTER TO START THE GAME
                    }

                    else if (gameIsFinished == true)
                    {
                        pause.handle_Pause(e,menu, skier);  //PRESS ENTER TO EXIT TO THE MAIN MENU
                    }
                }

                
                if (!window.isMinimized() && quitTheGame == false)  //Only draw when not minimized
                {
                    //if user requests to play the game
                    if (enterIsPressed == true && gameIsFinished == false && gameIsStarted == true && gameIsPaused == false)
                    {

                        //move the player and check if he touches the wall
                        skier.move(tileSet, TOTAL_TILES);
                        skier.setCamera(camera);

                        //Clear screen
                        SDL_SetRenderDrawColor(Renderer, 0xFF, 0xFF, 0xFF, 0xFF);
                        SDL_RenderClear(Renderer);

                        //Render level
                        for (int i = 0; i < TOTAL_TILES; ++i)
                        {
                            tileSet[i]->render(camera);
                        }

                        skier.render(camera);

                        //Set text to be rendered
                        timeText.str("");
                        scoreText.str("");

                        timeText << "Time: " << (timer.getTicks() / 1000.f);
                        scoreText << "Score: " << score;

                        if (!TimeTextTexture.loadFromRenderedText(timeText.str().c_str(), textColor))
                        {
                            printf("Unable to render time texture!\n");
                        }

                        if (!ScoreTextTexture.loadFromRenderedText(scoreText.str().c_str(), textColor))
                        {
                            printf("Unable to render time texture!\n");
                        }

                        TimeTextTexture.render(35, 35);
                        ScoreTextTexture.render(35, 105);

                        SDL_RenderPresent(Renderer);
                    }

                    else if (menu.show_menu() == true && enterIsPressed == false)   //render main menu
                        menu.render();

                    //PRESS ENTER TO START THE GAME
                    else if (enterIsPressed == false && gameAboutGame == false)
                    {
                        //Clear screen
                        SDL_SetRenderDrawColor(Renderer, 0xFF, 0xFF, 0xFF, 0xFF);
                        SDL_RenderClear(Renderer);

                        skier.setCamera(camera);

                        //Render level
                        for (int i = 0; i < TOTAL_TILES; ++i)
                        {
                            tileSet[i]->render(camera);
                        }

                        skier.render(camera);

                        //Set text to be rendered
                        timeText.str("");
                        scoreText.str("");

                        timeText << "Time: " << (timer.getTicks() / 1000.f);
                        scoreText << "Score: " << score;

                        if (!TimeTextTexture.loadFromRenderedText(timeText.str().c_str(), textColor))
                        {
                            printf("Unable to render time texture!\n");
                        }

                        if (!ScoreTextTexture.loadFromRenderedText(scoreText.str().c_str(), textColor))
                        {
                            printf("Unable to render time texture!\n");
                        }

                        TimeTextTexture.render(35, 35);
                        ScoreTextTexture.render(35, 105);

                        pause.render_press_to_start();

                        SDL_RenderPresent(Renderer);
                    }

                    //render "Pause" menu
                    else if (gameIsStarted == true && gameIsPaused == true && continueGame == false && gameAboutGame == false && enterIsPressed == true)
                    {
                        //Clear screen
                        SDL_SetRenderDrawColor(Renderer, 0xFF, 0xFF, 0xFF, 0xFF);
                        SDL_RenderClear(Renderer);

                        skier.setCamera(camera);

                        //Render level
                        for (int i = 0; i < TOTAL_TILES; ++i)
                        {
                            tileSet[i]->render(camera);
                        }

                        skier.render(camera);

                        //Set text to be rendered
                        timeText.str("");
                        scoreText.str("");

                        timeText << "Time: " << (timer.getTicks() / 1000.f);
                        scoreText << "Score: " << score;

                        if (!TimeTextTexture.loadFromRenderedText(timeText.str().c_str(), textColor))
                        {
                            printf("Unable to render time texture!\n");
                        }

                        if (!ScoreTextTexture.loadFromRenderedText(scoreText.str().c_str(), textColor))
                        {
                            printf("Unable to render time texture!\n");
                        }

                        TimeTextTexture.render(35, 35);
                        ScoreTextTexture.render(35, 105);

                        pause.render();

                        SDL_RenderPresent(Renderer);
                    }

                    else if (gameAboutGame == true)
                        aboutGameMenu.render();

                    else if (gameIsFinished == true)
                    {
                        //Clear screen
                        SDL_SetRenderDrawColor(Renderer, 0xFF, 0xFF, 0xFF, 0xFF);
                        SDL_RenderClear(Renderer);
                        
                        skier.move(tileSet, TOTAL_TILES);
                        skier.setCamera(camera);

                        //Render level
                        for (int i = 0; i < TOTAL_TILES; ++i)
                        {
                            tileSet[i]->render(camera);
                        }

                        skier.render(camera);

                        std::string congratsText = "CONGRATULATIONS!";
                        std::string pressEnter = "press ENTER to exit to the main menu";

                        Texture CongratsTextTexture;
                        Texture pressEnterTexture;

                        //Set text to be rendered
                        timeText.str(std::string());
                        scoreText.str(std::string());

                        timeText << "Your time is: " << (timer.getTicks() / 1000.f);
                        scoreText << "Your score is: " << score;

                        if (!TimeTextTexture.loadFromRenderedText(timeText.str().c_str(), textColor))
                        {
                            printf("Unable to render time texture!\n");
                        }

                        if (!ScoreTextTexture.loadFromRenderedText(scoreText.str().c_str(), textColor))
                        {
                            printf("Unable to render time texture!\n");
                        }

                        if (!CongratsTextTexture.loadFromRenderedText(congratsText.c_str(), textColor))
                        {
                            printf("Unable to render time texture!\n");
                        }

                        if (!pressEnterTexture.loadFromRenderedText(pressEnter.c_str(), textColor))
                        {
                            printf("Unable to render time texture!\n");
                        }

                        //Render textures
                        TimeTextTexture.render(675, 500);
                        ScoreTextTexture.render(675, 600);

                        CongratsTextTexture.render(750, 400);
                        pressEnterTexture.render(500, 1000);

                        //Update screen
                        SDL_RenderPresent(Renderer);
                    }
                }
            }
        }
    }
    //Free resources and close SDL
    close();
    return 0;
}