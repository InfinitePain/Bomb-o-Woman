/*
=============================================================================
 Name        : Player.cpp
 Author      : InfinitePain
 License     : Apache License 2.0
 Description : Source file for Bomb-o-Woman
=============================================================================
*/

#include "Player.h"
#include "Console.h"
#include "Playground.h"

bool Player::move(int direction) {
	if (!isMoveValid(direction)) {
		return false;
	}
	Console::zeichne_punkt(p.getX(), p.getY(), ' ');
	playground->removeGameObject(this);
	switch (direction) {
	case 0:
		p.setY(p.getY() + 1);
		break;
	case 1:
		p.setX(p.getX() - 1);
		break;
	case 2:
		p.setY(p.getY() + 1);
		break;
	case 3:
		p.setX(p.getX() - 1);
		break;
	}
	if (playground->isPickup(p.getX(), p.getY())) {
		processPickup();
	}
	playground->addGameObject(this);
	return true;
}

void Player::processPickup() {
	GameObject* pGameObject = playground->collectGameObject(&p);
	switch (pGameObject->getType()) {
	case Configuration::GAMEOBJECT_BOMB:
		bomb_ptr = pGameObject;
		playground->removeGameObject(pGameObject);
		break;
	case Configuration::GAMEOBJECT_SUPERBOMB:
		bomb_ptr = pGameObject;
		superbomb = true;
		playground->removeGameObject(pGameObject);
		break;
	case Configuration::GAMEOBJECT_PORTAL:
		pGameObject->setPosition(playground->getRandomFreePosition());
		break;
	case Configuration::GAMEOBJECT_TRAP:
		trap = true;
		playground->removeGameObject(pGameObject);
		break;
	case Configuration::GAMEOBJECT_GHOST:
		ghost = true;
		playground->removeGameObject(pGameObject);
		break;
	}
	delete pGameObject;
}

bool Player::isMoveValid(int direction) {
	int new_x = p.getX();
	int new_y = p.getY();
	switch (direction) {
	case Configuration::GAMEOBJECT_MOVE_UP:
		new_y--;
		break;
	case Configuration::GAMEOBJECT_MOVE_LEFT:
		new_x--;
		break;
	case Configuration::GAMEOBJECT_MOVE_DOWN:
		new_y++;
		break;
	case Configuration::GAMEOBJECT_MOVE_RIGHT:
		new_x++;
		break;
	}
	GameObject* pGameObject = playground->getGameObjectAtPos(new_x, new_y);
	if (playground->isFree(new_x, new_y) || playground->isPickup(new_x, new_y)) {
		return true;
	}
	return false;
}

bool Player::action() {
	if (!explosive && bomb_ptr != nullptr) {
		explosive = true;
		timer = Configuration::BOMB_TIMER;
		playground->addGameObject(bomb_ptr);
		setTop(bomb_ptr);
		return true;
	}
	return false;
}

void Player::proceed() {
	if (!explosive) {
		return;
	}
	timer--;
	if (timer != 0) {
		return;
	}
	int radius = superbomb ? Configuration::SUPERBOMB_RADIUS : Configuration::BOMB_RADIUS;
	Position bomb_position = bomb_ptr->getPosition();
	bomb_ptr->setPosition(playground->getRandomFreePosition());
	playground->addGameObject(bomb_ptr);
	bomb_ptr = NULL;
	explosive = false;

	std::vector<GameObject*> affected_objects = playground->neighbourhood(radius, this);
	for (GameObject* pGameObject : affected_objects) {
		Player* other_player = dynamic_cast<Player*>(pGameObject);
		if (other_player != nullptr) {
			if (other_player != this) {
				score += Configuration::POINTS_BLAST_PLAYER;
			}
			other_player->decreaseLives();
		}
		else if (pGameObject->getType() == Configuration::GAMEOBJECT_ROCK) {
			playground->removeGameObject(pGameObject);
			score += Configuration::POINTS_BLAST_ROCK;
		}
    }
	explosionAnimation(radius, bomb_position);
}

void Player::decreaseLives() {
	lives--;
}

std::string Player::to_string() {
	std::stringstream ss;
	ss << "***********************\n";
	ss << "* Player " << player_number << "  -  " << this->getType() << "      *\n";
	ss << "***********************\n";
    ss << "* Status:   " << (explosive ? "Explosive" : "Normal") << std::string(11 - (explosive ? 9 : 6), ' ') << "*\n";
    ss << "* Timer :   " << timer << std::string(10 - std::to_string(timer).length(), ' ') << "*\n";
    ss << "* Lives :   " << lives << std::string(10 - std::to_string(lives).length(), ' ') << "*\n";
    ss << "* Score :   " << score << std::string(10 - std::to_string(score).length(), ' ') << "*\n";
	ss << "***********************";
	return ss.str();
}

void Player::explosionAnimation(int radius, Position pos) {
	//TODO Implement
}
