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

class Game {
private:
	Player* player[4];
	std::vector<GameObject*> rock;
	std::vector<GameObject*> wall;
	std::vector<GameObject*> bombs;
	Playground* pg = nullptr;
private:
	void drawLogo();
	void drawLegend();
	void drawPlayerStats(int player_number);
	void drawScoreboard();
	void init(std::string filename);
	bool allPlayersAlive();
public:
	void run();
	Game();
};


#endif /*GAME_H_*/
