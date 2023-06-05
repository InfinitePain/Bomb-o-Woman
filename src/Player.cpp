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
#include <unistd.h>

int Player::getPlayerNumber() {
	switch (this->symbol) {
	case Configuration::GAMEOBJECT_PLAYER_1:
		return 1;
	case Configuration::GAMEOBJECT_PLAYER_2:
		return 2;
	case Configuration::GAMEOBJECT_PLAYER_3:
		return 3;
	case Configuration::GAMEOBJECT_PLAYER_4:
		return 4;
	default:
		return -1;
	}
}

Player::Player(char symbol, Playground* playground, Position p) : GameObject(p, symbol, playground) {
	this->lives = Configuration::PLAYER_LIVES;
	this->player_number = getPlayerNumber();
	this->score = 0;
	this->timer = 0;
	this->ghost = false;
	this->trap = false;
	this->explosive = false;
	this->superbomb = false;
	this->bomb_ptr = nullptr;
}

bool Player::move(int direction) {
	if (!isMoveValid(direction)) {
		return false;
	}
	playground->removeGameObject(this);
	switch (direction) {
	case Configuration::GAMEOBJECT_MOVE_UP:
		p.setY(p.getY() - 1);
		break;
	case Configuration::GAMEOBJECT_MOVE_LEFT:
		p.setX(p.getX() - 1);
		break;
	case Configuration::GAMEOBJECT_MOVE_DOWN:
		p.setY(p.getY() + 1);
		break;
	case Configuration::GAMEOBJECT_MOVE_RIGHT:
		p.setX(p.getX() + 1);
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
		break;
	case Configuration::GAMEOBJECT_SUPERBOMB:
		bomb_ptr = pGameObject;
		superbomb = true;
		break;
	case Configuration::GAMEOBJECT_PORTAL:
		pGameObject->setPosition(playground->getRandomFreePosition());
		break;
	case Configuration::GAMEOBJECT_TRAP:
		trap = true;
		break;
	case Configuration::GAMEOBJECT_GHOST:
		ghost = true;
		break;
	}
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
	default:
		return false;
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
		bomb_ptr->setPosition(this->p);
		playground->addGameObject(bomb_ptr);
		return true;
	}
	return false;
}

bool Player::proceed() {
	if (!explosive) {
		return false;
	}
	timer -= 1000;
	if (timer > 0) {
		return true;
	}
	timer = 0;
	int radius = superbomb ? Configuration::SUPERBOMB_RADIUS : Configuration::BOMB_RADIUS;
	Position bomb_position = bomb_ptr->getPosition();
	playground->removeGameObject(bomb_ptr);
	bomb_ptr->setPosition(playground->getRandomFreePosition());
	playground->addGameObject(bomb_ptr);
	bomb_ptr = NULL;
	explosive = false;

	std::vector<GameObject*> affected_objects = playground->neighbourhood(radius, bomb_position);
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
	return true;
}

void Player::decreaseLives() {
	lives--;
}

std::string Player::to_string() {
	std::stringstream ss;
	ss << "***********************\n";
	ss << "* Player " << player_number << "  -  " << this->getType() << "      *\n";
	ss << "***********************\n";
	ss << "* Status:   " << (explosive ? "Explosive" : "Normal") << std::string(10 - (explosive ? 9 : 6), ' ') << "*\n";
	ss << "* Timer :   " << timer / 1000 << std::string(10 - std::to_string(timer / 1000).length(), ' ') << "*\n";
	ss << "* Lives :   " << lives << std::string(10 - std::to_string(lives).length(), ' ') << "*\n";
	ss << "* Score :   " << score << std::string(10 - std::to_string(score).length(), ' ') << "*\n";
	ss << "***********************";
	return ss.str();
}

void Player::explosionAnimation(int radius, Position pos) {
	for (int dir = 0; dir < 4; dir++) {
		for (int offset = 1; offset <= radius; offset++) {
			int x = pos.getX() + (dir == 1 ? offset : dir == 3 ? -offset : 0);
			int y = pos.getY() + (dir == 0 ? -offset : dir == 2 ? offset : 0);

			if (!playground->inbound(x, y)) break;

			Position newPos(x, y);
			GameObject* pGameObject = playground->getGameObjectAtPos(newPos.getX(), newPos.getY());
			if (pGameObject != nullptr && pGameObject->getType() == Configuration::GAMEOBJECT_WALL) break;
			if (!playground->isLOS(&pos, &newPos)) break;

			newPos.draw(true, 'C');
		}
	}

	usleep(500000);
}

int Player::getLives() {
	return lives;
}

int Player::getScore() {
	return score;
}

int Player::getTimer() {
	return timer;
}

void Player::draw(bool offset) {
	char symbol = getType();
	if (ghost) {
		symbol = Configuration::GAMEOBJECT_GHOST;
	}
	if (offset) {
		Console::zeichne_punkt(p.getX() + Configuration::PLAYGROUND_OFFSETX, p.getY() + Configuration::PLAYGROUND_OFFSETY, symbol);
	}
	else {
		Console::zeichne_punkt(p.getX(), p.getY(), symbol);
	}
	if (top_ptr != NULL) {
		top_ptr->draw(offset);
	}
}
