/*
=============================================================================
 Name        : Playground.h
 Author      : InfinitePain
 License     : Apache License 2.0
 Description : Header file for Bomb-o-Woman
=============================================================================
*/

#ifndef PLAYGROUND_H_
#define PLAYGROUND_H_

#include "Configuration.h"
#include "GameObject.h"
#include <cstdlib>
#include <ctime>
#include <vector>
class GameObject;

class Playground
{
	friend class GameObject;
private:
	//Index represents coordinates within playground
	GameObject* area[(int)Configuration::PLAYGROUND_XSIZE][(int)Configuration::PLAYGROUND_YSIZE];
public:

	Playground();

	//F�gt den Zeiger in das area - Attribut an Index [x] [y] der Position ein.
	void addGameObject(GameObject* go_ptr);

	//"Nullt" den Zeiger im area - Attribut an Index [x] [y] der Position.
	void removeGameObject(GameObject* go_ptr);

	GameObject* collectGameObject(Position* pos_ptr);

	GameObject* getGameObjectAtPos(int x, int y);

	bool isFree(int x, int y);
	bool isPickup(int x, int y);

	//Optional Aufteilung in Moveable und solid
	void draw();

	Position getRandomFreePosition();
	
	bool inbound(int x, int y);
	bool isLOS(Position* pos1_ptr, Position* pos2_ptr);

	std::vector <GameObject*> neighbourhood(int distance, Position pos);

};

#endif /*PLAYGROUND_H_*/
