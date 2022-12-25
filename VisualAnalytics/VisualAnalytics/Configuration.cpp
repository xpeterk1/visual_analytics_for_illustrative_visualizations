#include "Configuration.h"

int Configuration::screen_height() 
{
	return _screen_height;
}

int Configuration::screen_width()
{
	return _screen_width;
}

void Configuration::change_size(int new_w, int new_h)
{
	_screen_width = new_w;
	_screen_height = new_h;
}