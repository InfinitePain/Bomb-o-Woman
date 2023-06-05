/*
=============================================================================
 Name        : Input.h
 Author      : InfinitePain
 License     : Apache License 2.0
 Description : Header file for Bomb-o-Woman
=============================================================================
*/

#ifndef INPUT_H_
#define INPUT_H_

#include <unordered_map>
#include <vector>
#if defined(_WIN32)
#include <windows.h>
#elif defined(__linux__)
#include <termios.h>
#endif
#include <chrono>

class Input {
public:
	Input();
	~Input();
	int processInput(int timeout) const;

private:
	int findKeyMapping(int key) const;
#if defined(_WIN32)
	bool PauseGame(HANDLE hStdIn, INPUT_RECORD inputRecord, DWORD events, int key) const;
#elif defined(__linux__)
	bool PauseGame() const;
	struct termios old_tio;
#endif
	std::vector<std::unordered_map<int, int>> playerKeyMappings;
};

#endif /*INPUT_H_*/
