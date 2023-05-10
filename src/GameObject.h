#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Position.h"
#include "Configuration.h"

class Playground;

class GameObject
{
protected:
	//Position innerhalb des Spielfeldes
	Position p;
	//Zeichen repräsentiert das Symbol
	char symbol;
	//Zeiger auf das Spielfeld 
	Playground* playground = NULL;

public:
	//Leerer Konstruktor (setzt nur das symbol auf ' ')
	GameObject();
	//3-Param Konstruktor setzt die Attribute auf die übergebenen Parameter
	GameObject(Position p, char symbol, Playground* playground = NULL);
	//Funktion zeichnet das Symbol an der entsprechenden Stelle (mit oder ohne offset)
	virtual void draw(bool offset);
	//Setzt die Position auf die übergebene Stelle
	void setPosition(Position p);
	//Übernimmt den übergebenen Zeiger 
	void setPlayground(Playground* pg);
	//Gibt das Attribut zurück
	Position getPosition();
	//Gibt das Zeichen des GameObjektes zurück
	char getType();
};
#endif 
