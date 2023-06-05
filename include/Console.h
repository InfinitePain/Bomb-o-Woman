/*
=============================================================================
 Name        : Console.h
 Author      : InfinitePain
 License     : Apache License 2.0
 Description : Header file for Bomb-o-Woman
=============================================================================
*/

#ifndef CONSOLE_H_
#define CONSOLE_H_

#include <string>

namespace Console
{
	void init();				//Leere Terminal, verstecke Cursor, Position 0,0
	void reset();
	void leeren();				//Leere Terminal

	void verstecken();			//Verstecke Cursor 	
	void zeigen();				//Zeige Cursor 

	void zeichne_punkt(int x, int y, char c); //Zeichne Zeichen c an Position x, y
	void zeichne_text(int anfang_x, int anfang_y, int ende_x, std::string text); //Zeichne Text an Position anfang_x, anfang_y, bis Position ende_x
}

#endif /*CONSOLE_H_*/
