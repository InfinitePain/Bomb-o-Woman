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
	init();
}

void Playground::init() {
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
			area[x][y]->top_ptr = go_ptr;
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
		GameObject::Space.setPosition(Position(x, y));
		GameObject::Space.draw(true);
	}
	else {
		pGameObject->top_ptr = nullptr;
		pGameObject->draw(true);
	}
}

void Playground::removeGameObject(Position pos) {
	GameObject* pGameObject = getGameObjectAtPos(pos.getX(), pos.getY());
	if (pGameObject != nullptr) {
		Playground::removeGameObject(pGameObject);
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
	if (Playground::inbound(x, y)) {
		return area[x][y] == nullptr;
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
				GameObject::Space.setPosition(Position(i, j));
				GameObject::Space.draw(true);
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
	return x >= 0 && x <= Configuration::PLAYGROUND_XSIZE && y >= 0 && y <= Configuration::PLAYGROUND_YSIZE;
}

bool Playground::isLOS(Position* pos1_ptr, Position* pos2_ptr) {
	int x1 = pos1_ptr->getX();
	int y1 = pos1_ptr->getY();
	int x2 = pos2_ptr->getX();
	int y2 = pos2_ptr->getY();
	if (x1 == x2) {
		if (y1 < y2) {
			for (int i = y1 + 1; i < y2; i++) {
				GameObject* pGameObject = getGameObjectAtPos(x1, i);
				if (!isFree(x1, i) && pGameObject->getType() == Configuration::GAMEOBJECT_WALL) {
					return false;
				}
			}
		}
		else {
			for (int i = y2 + 1; i < y1; i++) {
				GameObject* pGameObject = getGameObjectAtPos(x1, i);
				if (!isFree(x1, i) && pGameObject->getType() == Configuration::GAMEOBJECT_WALL) {
					return false;
				}
			}
		}
		return true;
	}
	else if (y1 == y2) {
		if (x1 < x2) {
			for (int i = x1 + 1; i < x2; i++) {
				GameObject* pGameObject = getGameObjectAtPos(i, y1);
				if (!isFree(i, y1) && pGameObject->getType() == Configuration::GAMEOBJECT_WALL) {
					return false;
				}
			}
		}
		else {
			for (int i = x2 + 1; i < x1; i++) {
				GameObject* pGameObject = getGameObjectAtPos(i, y1);
				if (!isFree(i, y1) && pGameObject->getType() == Configuration::GAMEOBJECT_WALL) {
					return false;
				}
			}
		}
		return true;
	}
	else {
		return false;
	}
}

std::vector<GameObject*> Playground::neighbourhood(int distance, GameObject* go_ptr) {
	std::vector<GameObject*> neighbourhood;
	Position pos = go_ptr->getPosition();
	int startX = std::max(0, pos.getX() - distance);
	int endX = std::min(Configuration::PLAYGROUND_XSIZE, pos.getX() + distance);
	int startY = std::max(0, pos.getY() - distance);
	int endY = std::min(Configuration::PLAYGROUND_YSIZE, pos.getY() + distance);
	for (int i = startX; i <= endX; i++) {
		for (int j = startY; j <= endY; j++) {
			if (pos.distanceManhattan(Position(i, j)) <= distance) {
				GameObject* pGameObject = getGameObjectAtPos(i, j);
				Position objectpos = go_ptr->getPosition();
				if (pGameObject != nullptr && Playground::isLOS(&pos, &objectpos)) {
					neighbourhood.push_back(pGameObject);
				}
			}
		}
	}
	return neighbourhood;
}

std::vector<Position> Playground::neighbourhood(int distance, Position* pos_ptr) {
	std::vector<Position> neighbourhood;
	int startX = std::max(0, pos_ptr->getX() - distance);
	int endX = std::min(Configuration::PLAYGROUND_XSIZE, pos_ptr->getX() + distance);
	int startY = std::max(0, pos_ptr->getY() - distance);
	int endY = std::min(Configuration::PLAYGROUND_YSIZE, pos_ptr->getY() + distance);
	for (int i = startX; i <= endX; i++) {
		for (int j = startY; j <= endY; j++) {
			if (pos_ptr->distanceManhattan(Position(i, j)) <= distance) {
				GameObject* pGameObject = getGameObjectAtPos(i, j);
				Position objectpos = pGameObject->getPosition();
				if (pGameObject != nullptr && Playground::isLOS(pos_ptr, &objectpos)) {
					neighbourhood.push_back(Position(i, j));
				}
			}
		}
	}
	return neighbourhood;
}

