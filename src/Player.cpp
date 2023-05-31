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

Player::Player(int player_number): GameObject() {
	this->player_number = player_number;
	this->lives = Configuration::PLAYER_LIVES;
	this->score = 0;
	this->timer = 0;
	this->ghost = false;
	this->trap = false;
	this->explosive = false;
	this->superbomb = false;
	this->bomb_ptr = nullptr;
	switch (player_number) {
		case 1:
			this->symbol = Configuration::GAMEOBJECT_PLAYER_1;
			break;
		case 2:
			this->symbol = Configuration::GAMEOBJECT_PLAYER_2;
			break;
		case 3:
			this->symbol = Configuration::GAMEOBJECT_PLAYER_3;
			break;
		case 4:
			this->symbol = Configuration::GAMEOBJECT_PLAYER_4;
			break;
	}
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
    ss << "* Status:   " << (explosive ? "Explosive" : "Normal") << std::string(10 - (explosive ? 9 : 6), ' ') << "*\n";
    ss << "* Timer :   " << timer << std::string(10 - std::to_string(timer).length(), ' ') << "*\n";
    ss << "* Lives :   " << lives << std::string(10 - std::to_string(lives).length(), ' ') << "*\n";
    ss << "* Score :   " << score << std::string(10 - std::to_string(score).length(), ' ') << "*\n";
	ss << "***********************";
	return ss.str();
}

void Player::explosionAnimation(int radius, Position pos) {
	//TODO Implement
}

int Player::getLives() {
	return lives;
}

int Player::getScore() {
	return score;
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
