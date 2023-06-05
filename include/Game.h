/*
=============================================================================
 Name        : Game.h
 Author      : InfinitePain
 License     : Apache License 2.0
 Description : Header file for Bomb-o-Woman
=============================================================================
*/

#ifndef GAME_H_
#define GAME_H_

#include "Player.h"
#include "Playground.h"
#include "GameObject.h"
#include "Input.h"

class Game {
private:
	Player* player[Configuration::NUMBER_OF_PLAYERS];
	std::vector<GameObject*> rock;
	std::vector<GameObject*> wall;
	std::vector<GameObject*> bombs;
	Playground* pg = nullptr;
	Input input;
	std::chrono::milliseconds timeSpent;
	std::chrono::milliseconds lastUpdateTime ;
private:
	void drawLogo();
	void drawLegend();
	void drawPlayerStats(int player_number);
	void drawScoreboard();
	void drawPauseInfo();
	bool is_game_over();
	void drawPlayerStatsForAll();
	bool proceedForAllPlayers();
	int getPlayerIndexFromDirection(int direction);
	int getMinBombTimer();
	int getDirectionFromInput(int timer);
	void updateGame(std::chrono::high_resolution_clock::time_point& lastUpdateTime);
	bool timeToUpdate(std::chrono::high_resolution_clock::time_point& lastUpdateTime, int timer);
	void processPlayerInput(int direction);
public:
	int run();
	Game(std::string filename = "stage1.txt");
	~Game();
};


#endif /*GAME_H_*/
