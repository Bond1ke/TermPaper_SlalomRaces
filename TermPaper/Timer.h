    #pragma once

#include "SDL2-2.0.22/include/SDL.h"
#include "SDL2_image-2.0.5/include/SDL_image.h"
typedef unsigned __int32 Uint32;

class Timer
{
public:
    //Initializes variables
    Timer();

    //The various clock actions
    void start();
    void stop();
    void pause();
    void unpause();

    //Gets the timer's time
    Uint32 getTicks();

    //Checks the status of the timer
    bool isStarted();
    bool isPaused();

private:
    //The clock time when the timer started
    Uint32 StartTicks;

    //The ticks stored when the timer was paused
    Uint32 PausedTicks;

    //The timer status
    bool Paused;
    bool Started;
};