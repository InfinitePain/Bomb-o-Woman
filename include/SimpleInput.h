/*
=============================================================================
 Name        : SimpleInput.h
 Author      : InfinitePain
 License     : Apache License 2.0
 Description : Header file for Bomb-o-Woman
=============================================================================
*/

#ifndef SIMPLEINPUT_H_
#define SIMPLEINPUT_H_

class SimpleInput
{

public:
	SimpleInput();
	~SimpleInput();
	static char getWASD(long interval);
	static char getArrow(long interval);
	static char getSteuerung(long interval);
};

#endif /*SIMPLEINPUT_H_*/
