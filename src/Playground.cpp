/*
=============================================================================
 Name        : Playground.cpp
 Author      : InfinitePain
 License     : Apache License 2.0
 Description : Source file for Bomb-o-Woman
=============================================================================
*/

#include "Playground.h"
#include "GameObject.h"
#include "Configuration.h"
#include "Console.h"

Playground::Playground() {
	for (int i = 0; i < Configuration::PLAYGROUND_XSIZE; i++) {
		for (int j = 0; j < Configuration::PLAYGROUND_YSIZE; j++) {
			area[i][j] = nullptr;
		}
	}
}

void Playground::addGameObject(GameObject* go_ptr) {
	int x = go_ptr->getPosition().getX();
	int y = go_ptr->getPosition().getY();
	if (x >= 0 && x < Configuration::PLAYGROUND_XSIZE && y >= 0 && y < Configuration::PLAYGROUND_YSIZE) {
		if (area[x][y] == nullptr) {
			area[x][y] = go_ptr;
		}
		else {
			area[x][y]->setTop(go_ptr);
		}
	}
	go_ptr->draw(true);
}

void Playground::removeGameObject(GameObject* go_ptr) {
	int x = go_ptr->getPosition().getX();
	int y = go_ptr->getPosition().getY();
	GameObject* pGameObject = getGameObjectAtPos(x, y);
	if (pGameObject == nullptr) {
		return;
	}
	if (pGameObject->top_ptr == nullptr) {
		area[x][y] = nullptr;
		go_ptr->p.draw(true, ' ');
	}
	else {
		area[x][y] = pGameObject->top_ptr;
		pGameObject->top_ptr->draw(true);
		pGameObject->top_ptr = nullptr;
	}
}

GameObject* Playground::collectGameObject(Position* pos_ptr) {
	int x = pos_ptr->getX();
	int y = pos_ptr->getY();
	if (Playground::inbound(x, y)) {
		GameObject* pGameObject = area[x][y];
		if (pGameObject != nullptr) {
			removeGameObject(pGameObject);
			return pGameObject;
		}
	}
	return nullptr;
}

GameObject* Playground::getGameObjectAtPos(int x, int y) {
	if (Playground::inbound(x, y)) {
		return area[x][y];
	}
	return nullptr;
}

bool Playground::isFree(int x, int y) {
	if (!Playground::inbound(x, y) || area[x][y] == nullptr) {
		return true;
	}
	return false;
}

bool Playground::isPickup(int x, int y) {
	if (Playground::inbound(x, y)) {
		GameObject* pGameObject = getGameObjectAtPos(x, y);
		if (pGameObject != nullptr) {
			char type = pGameObject->getType();
			return type == Configuration::GAMEOBJECT_GHOST || type == Configuration::GAMEOBJECT_BOMB || type == Configuration::GAMEOBJECT_SUPERBOMB || type == Configuration::GAMEOBJECT_PORTAL || type == Configuration::GAMEOBJECT_TRAP;
		}
	}
	return false;
}

void Playground::draw() {
	for (int i = 0; i < Configuration::PLAYGROUND_XSIZE; i++) {
		for (int j = 0; j < Configuration::PLAYGROUND_YSIZE; j++) {
			GameObject* pGameObject = area[i][j];
			if (pGameObject != nullptr) {
				pGameObject->draw(true);
			}
			else {
				Position pos = Position(i, j);
				pos.draw(true, ' ');
			}
		}
	}
}

Position Playground::getRandomFreePosition() {
	int x = rand() % Configuration::PLAYGROUND_XSIZE;
	int y = rand() % Configuration::PLAYGROUND_YSIZE;
	while (!isFree(x, y)) {
		x = rand() % Configuration::PLAYGROUND_XSIZE;
		y = rand() % Configuration::PLAYGROUND_YSIZE;
	}
	return Position(x, y);
}

bool Playground::inbound(int x, int y) {
	return x >= 0 && x < Configuration::PLAYGROUND_XSIZE && y >= 0 && y < Configuration::PLAYGROUND_YSIZE;
}

bool Playground::isLOS(Position* pos1_ptr, Position* pos2_ptr) {
	int x1 = pos1_ptr->getX();
	int y1 = pos1_ptr->getY();
	int x2 = pos2_ptr->getX();
	int y2 = pos2_ptr->getY();

	if (x1 != x2 && y1 != y2) {
		return false;
	}

	int start, end;
	bool isVertical = x1 == x2;

	if (isVertical) {
		start = std::min(y1, y2) + 1;
		end = std::max(y1, y2);
	}
	else {
		start = std::min(x1, x2) + 1;
		end = std::max(x1, x2);
	}

	for (int i = start; i < end; i++) {
		if (!isFree(isVertical ? x1 : i, isVertical ? i : y1)) {
			return false;
		}
	}

	return true;
}


std::vector<GameObject*> Playground::neighbourhood(int distance, Position pos) {
	std::vector<GameObject*> neighbourhood;

	int startX = std::max(0, pos.getX() - distance);
	int endX = std::min(Configuration::PLAYGROUND_XSIZE, pos.getX() + distance);
	int startY = std::max(0, pos.getY() - distance);
	int endY = std::min(Configuration::PLAYGROUND_YSIZE, pos.getY() + distance);

	for (int i = startX; i <= endX; i++) {
		for (int j = startY; j <= endY; j++) {
			if (pos.distanceManhattan(Position(i, j)) > distance) {
				continue;
			}

			GameObject* pGameObject = getGameObjectAtPos(i, j);
			if (pGameObject == nullptr) {
				continue;
			}

			Position objectpos = pGameObject->getPosition();
			if (Playground::isLOS(&pos, &objectpos)) {
				neighbourhood.push_back(pGameObject);
			}
		}
	}
	return neighbourhood;
}
