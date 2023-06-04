#pragma once
#include "SDL2-2.0.22/include/SDL.h"

class Lines 
{
public:

    Lines();

    Lines(int X1, int Y1, int X2, int Y2);

    void set_flag(bool flag);   //set private variable "passed"
    bool get_flag();    //get private variable "passed"

    void set_counted(bool flag);
    bool get_counted();
    
    //start point
    int* get_start_x();
    int* get_start_y();

    //end point
    int* get_end_x();
    int* get_end_y();

    void setline(int X1, int Y1, int X2, int Y2);

private:

    SDL_Point start;
    SDL_Point end;

    bool passed;
    bool counted;
};