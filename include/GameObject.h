/*
=============================================================================
 Name        : GameObject.h
 Author      : InfinitePain
 License     : Apache License 2.0
 Description : Header file for Bomb-o-Woman
=============================================================================
*/

#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_

#include "Position.h"
#include "Configuration.h"

class Playground;

class GameObject
{
protected:
	//Position innerhalb des Spielfeldes
	Position p;
	//Zeichen repr�sentiert das Symbol
	char symbol;
	//Zeiger auf das Spielfeld 
	Playground* playground = NULL;

public:
	//Leerer Konstruktor (setzt nur das symbol auf ' ')
	GameObject();
	//3-Param Konstruktor setzt die Attribute auf die �bergebenen Parameter
	GameObject(Position p, char symbol, Playground* playground = NULL);
	//Funktion zeichnet das Symbol an der entsprechenden Stelle (mit oder ohne offset)
	virtual void draw(bool offset);
	//Setzt die Position auf die �bergebene Stelle
	void setPosition(Position p);
	//�bernimmt den �bergebenen Zeiger 
	void setPlayground(Playground* pg);
	//Gibt das Attribut zur�ck
	Position getPosition();
	//Gibt das Zeichen des GameObjektes zur�ck
	char getType();
};

#endif /*GAMEOBJECT_H_*/
