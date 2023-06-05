/*
=============================================================================
 Name        : Console.cpp
 Author      : InfinitePain
 License     : Apache License 2.0
 Description : Source file for Bomb-o-Woman
=============================================================================
*/

#include "Console.h"
#include <iostream>
#include "Cursor.h"

//Leere Bildschirm, verstecke Cursor, Position 0,0
void Console::init()
{
	Console::leeren();
	Console::verstecken();
}
void Console::reset()
{
	Console::zeigen();
	Console::leeren();
}
//Leere Terminal
void Console::leeren()
{
	std::cout << "\33[2J";
}
//Verstecke Cursor
void Console::verstecken()
{
	std::cout << "\33[?25l";
}
//Zeigt den Cursor
void Console::zeigen()
{
	std::cout << "\33[?25h";
}

//Gebe Zeichen c an Position x, y aus.
void Console::zeichne_punkt(int x, int y, char c)
{
	//printf("\33[%d;%dH%c", y, x, c);
	Cursor::bewegen(x, y);
	std::cout << c;
	std::cout.flush();
}

//Gebe Text an Position anfang_x, anfang_y, bis Position ende_x, ende_y aus.
void Console::zeichne_text(int anfang_x, int anfang_y, int ende_x, std::string text)
{
	int x = anfang_x;
	int y = anfang_y;
	
	for (char c : text) {
		zeichne_punkt(x, y, c);
		x++;
		if (ende_x != -1 && x == ende_x) {
			x = anfang_x;
			y++;
		}
		if (c == '\n') {
			x = anfang_x;
			y++;
		}
	}
	std::cout.flush();
}
