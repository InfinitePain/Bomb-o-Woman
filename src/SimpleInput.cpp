/*
=============================================================================
 Name        : SimpleInput.cpp
 Author      : InfinitePain
 License     : Apache License 2.0
 Description : Source file for Bomb-o-Woman
=============================================================================
*/

#include "SimpleInput.h"
#include "Configuration.h"
#include <windows.h>
#include <iostream>
//Microsoft Codetabelle
//https://msdn.microsoft.com/de-de/library/windows/desktop/dd375731(v=vs.85).aspx

SimpleInput::SimpleInput()
{
}


SimpleInput::~SimpleInput()
{
}

char SimpleInput::getSteuerung(long interval)
{
	Sleep(interval);
	const int KEY_UP = 0x1;
	if ((GetAsyncKeyState(0x57) & KEY_UP) == KEY_UP)
		return 'w';
	else if ((GetAsyncKeyState(0x41) & KEY_UP) == KEY_UP)
		return 'a';
	else if ((GetAsyncKeyState(0x53) & KEY_UP) == KEY_UP)
		return 's';
	else if ((GetAsyncKeyState(0x44) & KEY_UP) == KEY_UP)
		return 'd';
	else if ((GetAsyncKeyState(0x45) & KEY_UP) == KEY_UP)
		return 'e';
	else
		return 'x';
}

char SimpleInput::getWASD(long interval)
{
	Sleep(interval);
	const int KEY_UP = 0x1;
	if ((GetAsyncKeyState(0x57) & KEY_UP) == KEY_UP)
		return Configuration::GAMEOBJECT_MOVE_UP;// 'w';
	else if ((GetAsyncKeyState(0x41) & KEY_UP) == KEY_UP)
		return Configuration::GAMEOBJECT_MOVE_LEFT;// 'a';
	else if ((GetAsyncKeyState(0x53) & KEY_UP) == KEY_UP)
		return Configuration::GAMEOBJECT_MOVE_DOWN;// 's';
	else if ((GetAsyncKeyState(0x44) & KEY_UP) == KEY_UP)
		return Configuration::GAMEOBJECT_MOVE_RIGHT;// 'd';
	else if ((GetAsyncKeyState(0x45) & KEY_UP) == KEY_UP)
		return Configuration::GAMEOBJECT_ACTION;// 'e';
	else
		return 'x';
}

char SimpleInput::getArrow(long interval)
{
	Sleep(interval);
	const int KEY_UP = 0x1;
	//VK UP
	if ((GetAsyncKeyState(0x26) & KEY_UP) == KEY_UP)
		return Configuration::GAMEOBJECT_MOVE_UP;// '8';
	//VK LEFT
	else if ((GetAsyncKeyState(0x25) & KEY_UP) == KEY_UP)
		return Configuration::GAMEOBJECT_MOVE_LEFT;// '4';
	//VK DOWN
	else if ((GetAsyncKeyState(0x28) & KEY_UP) == KEY_UP)
		return Configuration::GAMEOBJECT_MOVE_DOWN;// '5';
	//VK RIGHT
	else if ((GetAsyncKeyState(0x27) & KEY_UP) == KEY_UP)
		return Configuration::GAMEOBJECT_MOVE_RIGHT;// '6';
	//Enter
	else if ((GetAsyncKeyState(0x0D) & KEY_UP) == KEY_UP)
		return Configuration::GAMEOBJECT_ACTION;// 'e';
	else
		return 'x';
}
