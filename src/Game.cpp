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
#include "Input.h"
#include "Cursor.h"
#include <unistd.h>
#include <climits>

Game::Game(std::string filename) {
	std::ifstream file(filename);

	if (!file) {
		std::cerr << "Failed to open file: " << filename << std::endl;
		return;
	}
	drawLogo();
	usleep(5000000);
	timeSpent = std::chrono::milliseconds(0);
	lastUpdateTime = std::chrono::milliseconds(0);
	pg = new Playground();
	std::string line;
	int y = 0;
	while (std::getline(file, line)) {
		int x = 0;
		for (char c : line) {
			GameObject* obj = nullptr;
			switch (c) {
			case Configuration::GAMEOBJECT_ROCK:
				obj = new GameObject(Position(x, y), c, pg);
				rock.push_back(obj);
				break;
			case Configuration::GAMEOBJECT_WALL:
				obj = new GameObject(Position(x, y), c, pg);
				wall.push_back(obj);
				break;
			case Configuration::GAMEOBJECT_BOMB:
				obj = new GameObject(Position(x, y), c, pg);
				bombs.push_back(obj);
				break;
			case Configuration::GAMEOBJECT_PLAYER_1:
			case Configuration::GAMEOBJECT_PLAYER_2:
			case Configuration::GAMEOBJECT_PLAYER_3:
			case Configuration::GAMEOBJECT_PLAYER_4:
			{
				obj = new Player(c, pg, Position(x, y));
				Player* newPlayer = dynamic_cast<Player*>(obj);
				player[newPlayer->getPlayerNumber() - 1] = newPlayer;
				break;
			}
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

Game::~Game() {
	delete pg;
	for (GameObject* obj : rock) {
		delete obj;
	}
	for (GameObject* obj : wall) {
		delete obj;
	}
	for (GameObject* obj : bombs) {
		delete obj;
	}
	for (Player* obj : player) {
		delete obj;
	}
}

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
	startx = centerx - width / 2;
	starty = centery - height / 2;

	Console::zeichne_text(startx, starty, -1, ss.str());
}

void Game::drawLegend() {
	int startx = Configuration::PLAYGROUND_XSIZE + Configuration::PLAYGROUND_OFFSETX + 2;
	std::stringstream ss;
	ss << "Player 1  " << Configuration::GAMEOBJECT_PLAYER_1 << "\n";
	ss << "Player 2  " << Configuration::GAMEOBJECT_PLAYER_2 << "\n";
	ss << "Player 3  " << Configuration::GAMEOBJECT_PLAYER_3 << "\n";
	ss << "Player 4  " << Configuration::GAMEOBJECT_PLAYER_4 << "\n";
	ss << "Monster   " << Configuration::GAMEOBJECT_MONSTER << "\n";
	ss << "Rock      " << Configuration::GAMEOBJECT_ROCK << "\n";
	ss << "Wall      " << Configuration::GAMEOBJECT_WALL << "\n";
	ss << "Ghost     " << Configuration::GAMEOBJECT_GHOST << "\n";
	ss << "Bomb      " << Configuration::GAMEOBJECT_BOMB << "\n";
	ss << "Superbomb " << Configuration::GAMEOBJECT_SUPERBOMB << "\n";
	ss << "Portal    " << Configuration::GAMEOBJECT_PORTAL << "\n";
	ss << "Trap      " << Configuration::GAMEOBJECT_TRAP << "\n";

	Console::zeichne_text(startx, 1, -1, ss.str());
}

void Game::drawPlayerStats(int player_number) {
	std::string string = player[player_number - 1]->to_string();
	int width = string.find('\n');
	int height = string.length() / width;
	int y = height * (player_number - 1) + 1;

	Console::zeichne_text(1, y, -1, string);
}

void Game::drawScoreboard() {
	int startx = Configuration::PLAYGROUND_OFFSETX;
	int starty = Configuration::PLAYGROUND_OFFSETY;
	int endx = Configuration::PLAYGROUND_XSIZE + Configuration::PLAYGROUND_OFFSETX;
	int endy = Configuration::PLAYGROUND_YSIZE + Configuration::PLAYGROUND_OFFSETY;
	int centerx = (startx + endx) / 2;
	int centery = (starty + endy) / 2;
	std::stringstream ss;
	ss << "****************\n";
	ss << "*  Scoreboard  *\n";
	ss << "****************\n";
	for (int i = 0; i < Configuration::NUMBER_OF_PLAYERS; i++) {
		if (player[i] != nullptr) {
			ss << "* Player " << (i + 1) << ": " << player[i]->getScore() << std::string(3 - std::to_string(player[i]->getScore()).length(), ' ') << "*\n";
		}
	}
	ss << "****************\n";

	int width = ss.str().find('\n');
	int height = ss.str().length() / width;
	startx = centerx - width / 2;
	starty = centery - height / 2;
	Console::zeichne_text(startx, starty, -1, ss.str());
}

void Game::drawPauseInfo() {
	int startx = Configuration::PLAYGROUND_OFFSETX;
	int endx = Configuration::PLAYGROUND_XSIZE + Configuration::PLAYGROUND_OFFSETX;
	int starty = Configuration::PLAYGROUND_YSIZE + Configuration::PLAYGROUND_OFFSETY;
#if defined(_WIN32)
	std::string string = "Press ESC to pause the game";
#elif defined(__linux__)
	std::string string = "Press F1 to pause the game";
#endif

	Console::zeichne_text(startx, starty + 1, endx, string);
}

bool Game::is_game_over() {
	int alive = 0;
	for (int i = 0; i < Configuration::NUMBER_OF_PLAYERS; i++) {
		if (player[i] != nullptr && player[i]->getLives() > 0) {
			alive++;
		}
	}
	return alive <= 1;
}

int Game::run() {
	int timer = 0;
	int direction = -1;
	auto lastUpdateTime = std::chrono::high_resolution_clock::now();

	try {
		drawPlayerStatsForAll();
		while (!is_game_over() && direction != Configuration::QUIT) {
			drawPauseInfo();
			pg->draw();

			timer = std::min(getMinBombTimer(), 1000);


			int direction = getDirectionFromInput(timer);

			if (direction == Configuration::QUIT) {
				break;
			}

			if (timeToUpdate(lastUpdateTime, timer) || direction == Configuration::TIMEOUT) {
				updateGame(lastUpdateTime);
			}
			if (direction == Configuration::TIMEOUT) {
				continue;
			}

			processPlayerInput(direction);
		}

		drawScoreboard();
		usleep(5000000);
	}
	catch (const std::runtime_error& e) {
		Console::leeren();
		Cursor::bewegen(0, 0);
		std::cerr << e.what() << std::endl;
		usleep(5000000);
		return 1;
	}

	return 0;
}



void Game::drawPlayerStatsForAll() {
	for (int i = 0; i < Configuration::NUMBER_OF_PLAYERS; i++) {
		if (player[i] != nullptr) {
			drawPlayerStats(i + 1);
		}
	}
}

bool Game::proceedForAllPlayers() {
	bool shouldProceed = false;
	for (int i = 0; i < Configuration::NUMBER_OF_PLAYERS; i++) {
		if (player[i] != nullptr && player[i]->getTimer() > 0) {
			shouldProceed = player[i]->proceed() || shouldProceed;
		}
	}
	return shouldProceed;
}

int Game::getPlayerIndexFromDirection(int direction) {
	int i = direction / 10 - 1;
	if (direction == 0) {
		i = 0;
	}
	return i;
}

int Game::getMinBombTimer() {
	int minBombTimer = INT_MAX;
	int count = 0;
	for (int i = 0; i < Configuration::NUMBER_OF_PLAYERS; i++) {
		if (player[i] != nullptr && player[i]->getTimer() > 0) {
			minBombTimer = std::min(minBombTimer, player[i]->getTimer());
		}
		else if (player[i] != nullptr) {
			count++;
		}
	}
	if (count == Configuration::NUMBER_OF_PLAYERS) {
		minBombTimer = 0;
	}
	return minBombTimer;
}


int Game::getDirectionFromInput(int timer) {
	auto start = std::chrono::high_resolution_clock::now();
	int direction = input.processInput(timer);
	auto end = std::chrono::high_resolution_clock::now();
	timeSpent += std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	if (direction < Configuration::QUIT) {
		throw std::runtime_error("Error while reading input: direction = " + std::to_string(direction));
	}
	return direction;
}


void Game::updateGame(std::chrono::high_resolution_clock::time_point& lastUpdateTime) {
	if (proceedForAllPlayers()) {
		drawPlayerStatsForAll();
	}
	lastUpdateTime = std::chrono::high_resolution_clock::now();
	timeSpent = std::chrono::milliseconds(0);
}


bool Game::timeToUpdate(std::chrono::high_resolution_clock::time_point& lastUpdateTime, int timer) {
	auto currentTime = std::chrono::high_resolution_clock::now();
	auto timeSinceLastProceed = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastUpdateTime);
	return timeSinceLastProceed.count() >= 1000 && timer > 0;
}

void Game::processPlayerInput(int direction) {
	int playerIndex = getPlayerIndexFromDirection(direction);
	direction %= 10;

	if (!player[playerIndex]->move(direction) && direction == Configuration::GAMEOBJECT_ACTION) {
		player[playerIndex]->action();
	}
}
