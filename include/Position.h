#ifndef POSITION_H
#define POSITION_H

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
#endif // !POSITION_H