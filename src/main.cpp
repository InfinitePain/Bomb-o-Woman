#include <iostream>
#include "Game.h"
#include "Console.h"
#include <cstdlib>
#include <ctime>


int main(int argc, char const* argv[]) {
	srand(time(NULL));
	Console::init();
	Game game = Game();
	if (game.run())
	{
		return 1;
	}
	Console::reset();
	return 0;
}
