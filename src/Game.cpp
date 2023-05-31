/*
=============================================================================
 Name        : Game.cpp
 Author      : InfinitePain
 License     : Apache License 2.0
 Description : Source file for Bomb-o-Woman
=============================================================================
*/

#include "Game.h"
#include "Configuration.h"
#include "Console.h"
#include <fstream>
#include <iostream>
#include "SimpleInput.h"

void Game::drawLogo() {
	int startx = Configuration::PLAYGROUND_OFFSETX;
	int starty = Configuration::PLAYGROUND_OFFSETY;
	int endx = Configuration::PLAYGROUND_XSIZE + Configuration::PLAYGROUND_OFFSETX;
	int endy = Configuration::PLAYGROUND_YSIZE + Configuration::PLAYGROUND_OFFSETY;
	std::stringstream ss;
	ss << "      _|  \n";
	ss << "     |    \n";
	ss << "   ****   \n";
	ss << " **    ** \n";
	ss << "*        *\n";
	ss << "*        *\n";
	ss << " **    ** \n";
	ss << "   ****   \n";

	int centerx = (startx + endx) / 2;
	int centery = (starty + endy) / 2;
	int width = ss.str().find('\n');
	int height = ss.str().length() / width;
	int shapex = centerx - width / 2;
	int shapey = centery - height / 2;

	for (char c : ss.str()) {
		switch (c) {
		case ' ':
			shapex++;
			break;
		case '\n':
			shapey++;
			shapex = centerx - width / 2;
			break;
		default:
			Console::zeichne_punkt(shapex, shapey, c);
			shapex++;
			break;
		}
	}
}

void Game::drawLegend() {
	int endx = Configuration::PLAYGROUND_XSIZE + Configuration::PLAYGROUND_OFFSETX + 2;
	std::stringstream ss;
	ss << "Player 1 " << Configuration::GAMEOBJECT_PLAYER_1 << "\n";
	ss << "Player 2 " << Configuration::GAMEOBJECT_PLAYER_2 << "\n";
	ss << "Player 3 " << Configuration::GAMEOBJECT_PLAYER_3 << "\n";
	ss << "Player 4 " << Configuration::GAMEOBJECT_PLAYER_4 << "\n";
	ss << "Monster  " << Configuration::GAMEOBJECT_MONSTER << "\n";
	ss << "Rock     " << Configuration::GAMEOBJECT_ROCK << "\n";
	ss << "Wall     " << Configuration::GAMEOBJECT_WALL << "\n";
	ss << "Ghost    " << Configuration::GAMEOBJECT_GHOST << "\n";
	ss << "Bomb     " << Configuration::GAMEOBJECT_BOMB << "\n";
	ss << "Superbomb" << Configuration::GAMEOBJECT_SUPERBOMB << "\n";
	ss << "Portal   " << Configuration::GAMEOBJECT_PORTAL << "\n";
	ss << "Trap     " << Configuration::GAMEOBJECT_TRAP << "\n";

	int x = endx;
	int y = 1;
	std::string string = ss.str();
	for (char c : ss.str()) {
		if (c == '\n') {
			y++;
			x = endx;
		}
		else {
			Console::zeichne_punkt(x, y, c);
			x++;
		}
	}
}

void Game::drawPlayerStats(int player_number) {
	std::string string = player[player_number - 1]->to_string();
	int width = string.find('\n');
	int height = string.length() / width;
	int x = 1;
	int y = height * (player_number - 1) + 1;

	for (char c : string) {
		if (c == '\n') {
			y++;
			x = 1;
		}
		else {
			Console::zeichne_punkt(x, y, c);
			x++;
		}
	}
}

void Game::drawScoreboard() {
	int startx = Configuration::PLAYGROUND_OFFSETX;
	int starty = Configuration::PLAYGROUND_OFFSETY;
	int endx = Configuration::PLAYGROUND_XSIZE + Configuration::PLAYGROUND_OFFSETX;
	int endy = Configuration::PLAYGROUND_YSIZE + Configuration::PLAYGROUND_OFFSETY;
	int centerx = (startx + endx) / 2;
	int centery = (starty + endy) / 2;
	std::stringstream ss;
	ss << "*************************\n";
	ss << "*      Scoreboard       *\n";
	ss << "*************************\n";
	for (int i = 0; i < Configuration::NUMBER_OF_PLAYERS; i++) {
		if (player[i] != nullptr) {
			ss << "* Player " << (i + 1) << ": " << player[i]->getScore() << std::string(12 - std::to_string(player[i]->getScore()).length(), ' ') << "*\n";
		}
	}
	ss << "*************************\n";

	int width = ss.str().find('\n');
	int height = ss.str().length() / width;
	int shapex = centerx - width / 2;
	int shapey = centery - height / 2;

	for (char c : ss.str()) {
		if (c == '\n') {
			shapey++;
			shapex = centerx - width / 2;
		}
		else {
			Console::zeichne_punkt(shapex, shapey, c);
			shapex++;
		}
	}
}

void Game::init(std::string filename) {
	std::ifstream file(filename);

	if (!file) {
		std::cerr << "Failed to open file: " << filename << std::endl;
		return;
	}

	pg = new Playground();
	std::string line;
	int y = 0;
	while (std::getline(file, line)) {
		int x = 0;
		for (char c : line) {
			GameObject* obj = nullptr;
			switch (c) {
			case Configuration::GAMEOBJECT_PLAYER_1:
				obj = new Player(1);
				obj->setPosition(Position(x, y));
				obj->setPlayground(pg);
				player[0] = (Player*)obj;
				break;
			case Configuration::GAMEOBJECT_PLAYER_2:
				obj = new Player(2);
				obj->setPosition(Position(x, y));
				obj->setPlayground(pg);
				player[1] = (Player*)obj;
				break;
			case Configuration::GAMEOBJECT_PLAYER_3:
				obj = new Player(3);
				obj->setPosition(Position(x, y));
				obj->setPlayground(pg);
				player[2] = (Player*)obj;
				break;
			case Configuration::GAMEOBJECT_PLAYER_4:
				obj = new Player(4);
				obj->setPosition(Position(x, y));
				obj->setPlayground(pg);
				player[3] = (Player*)obj;
				break;
			case Configuration::GAMEOBJECT_ROCK:
				obj = new GameObject(Position(x, y), Configuration::GAMEOBJECT_ROCK, pg);
				rock.push_back(obj);
				break;
			case Configuration::GAMEOBJECT_WALL:
				obj = new GameObject(Position(x, y), Configuration::GAMEOBJECT_WALL, pg);
				wall.push_back(obj);
				break;
			case Configuration::GAMEOBJECT_BOMB:
				obj = new GameObject(Position(x, y), Configuration::GAMEOBJECT_BOMB, pg);
				bombs.push_back(obj);
				break;
			default:
				break;
			}
			if (obj != nullptr) {
				pg->addGameObject(obj);
			}
			x++;
		}
		y++;
	}

	file.close();
	drawLegend();
	pg->draw();
}

bool Game::allPlayersAlive() {
	for (int i = 0; i < Configuration::NUMBER_OF_PLAYERS; i++) {
		if (player[i] != nullptr && player[i]->getLives() <= 0) {
			return false;
		}
	}
	return true;
}

void Game::run() {
	char (*inputFunc[Configuration::NUMBER_OF_PLAYERS])(long intervall);
	inputFunc[0] = &SimpleInput::getArrow;
	inputFunc[1] = &SimpleInput::getWASD;
	while (allPlayersAlive()) {
		for (int i = 0; i < Configuration::NUMBER_OF_PLAYERS; i++) {
			if (player[i] == nullptr) {
				continue;
			}
			char input = inputFunc[i](100);
			if (input == Configuration::GAMEOBJECT_ACTION) {
				player[i]->action();
			}
			else {
				player[i]->move(input);
			}
			player[i]->proceed();
			drawPlayerStats(i + 1);
		}
	}
	drawScoreboard();
}

Game::Game() {
	init("stage1.txt");
}
