#pragma once
class Configuration
{
public:
	inline static unsigned int _screen_width = 800;
	inline static unsigned int _screen_height = 600;

public:
	static int screen_width();
	static int screen_height();
	static void change_size(int new_width, int new_height);
};

