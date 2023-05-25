/*
=============================================================================
 Name        : Position.cpp
 Author      : InfinitePain
 License     : Apache License 2.0
 Description : Source file for Bomb-o-Woman
=============================================================================
*/

#include "Position.h"
#include <sstream>
#include <iostream>
#include <string>

void Position::setX(int x) {
	this->x = x;
}

int Position::getX() const {
	return x;
}

void Position::setY(int y) {
	this->y = y;
}

int Position::getY() const {
	return y;
}

int Position::distanceManhattan(const Position& p) {
	return abs(this->x - p.x) + abs(this->y - p.y);
}

std::string Position::to_string() {
	std::stringstream ss;
	ss << "x = " << x << ", y = " << y << std::endl;
	return ss.str();
}

Position::Position() : x(0), y(0) {}

Position::Position(int x, int y) : x(x), y(y) {}

Position Position::operator+ (const Position& rhs) {
	return Position(this->x + rhs.x, this->y + rhs.y);
}

Position Position::operator- (const Position& rhs) {
	return Position(this->x - rhs.x, this->y - rhs.y);
}

std::ostream& operator<< (std::ostream& os, const Position& rhs) {
	Position p = rhs;
	os << p.to_string();
	return os;
}

Position::~Position() {}
