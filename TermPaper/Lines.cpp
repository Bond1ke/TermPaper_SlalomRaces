#include "Lines.h"

Lines::Lines()
{
	start.x = 0;
	start.y = 0;

	end.x = 0;
	end.y = 0;

	passed = false;
	counted = false;
}

Lines::Lines(int X1, int Y1, int X2, int Y2)
{
	start.x = X1;
	start.y = Y1;

	end.x = X2;
	end.y = Y2;

	passed = false;
	counted = false;
}

int* Lines::get_start_x()
{
	return &start.x;
}
int* Lines::get_start_y()
{
	return &start.y;
}
int* Lines::get_end_x()
{
	return &end.x;
}
int* Lines::get_end_y()
{
	return &end.y;
}

void Lines::set_flag(bool flag)
{
	passed = flag;
}
bool Lines::get_flag()
{
	return passed;
}

void Lines::set_counted(bool flag)
{
	counted = flag;
}
bool Lines::get_counted()
{
	return counted;
}

void Lines::setline(int X1, int Y1, int X2, int Y2)
{
	start.x = X1;
	start.y = Y1;

	end.x = X2;
	end.y = Y2;
}