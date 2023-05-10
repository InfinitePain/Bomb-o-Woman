/*
=============================================================================
 Name        : Position.h
 Author      : InfinitePain
 License     : Apache License 2.0
 Description : Header file for Bomb-o-Woman
=============================================================================
*/

#ifndef POSITION_H_
#define POSITION_H_

#include <iostream>
#include <string>
#include <math.h>
#include <iomanip>

class Position
{
protected:
	int x;
	int y;
public:
	void setX(int x);
	int getX() const;

	void setY(int y);
	int getY() const;

	int distanceManhattan(const Position& p);

	std::string to_string();

	Position operator+ (const Position& rhs);
	Position operator- (const Position& rhs);

	Position();
	Position(int x, int y);
	~Position();
};
std::ostream& operator<< (std::ostream& os, const Position& rhs);

#endif /*POSITION_H_*/
