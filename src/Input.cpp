/*
=============================================================================
 Name        : Input.cpp
 Author      : InfinitePain
 License     : Apache License 2.0
 Description : Source file for Bomb-o-Woman
=============================================================================
*/

#include "Input.h"
#include "Configuration.h"
#include <iostream>
#include <sstream>
#include "Console.h"
#if defined(_WIN32)
#include <windows.h>
#elif defined(__linux__)
#include <termios.h>
#include <unistd.h>
#include <sys/poll.h>
#endif


int Input::findKeyMapping(int key) const {
	int result = -1;
	int i = 0;
	for (const auto& keyMapping : playerKeyMappings) {
		i++;
		auto it = keyMapping.find(key);
		if (it != keyMapping.end()) {
			result = it->second + (i * 10);
			break;
		}
	}
	return result;
}

#if defined(__linux__) //Linuxvariante

Input::Input() {
	// Get the terminal settings for stdin
	tcgetattr(STDIN_FILENO, &old_tio);

	struct termios new_tio = old_tio;

	// Disable canonical mode (buffered i/o) and local echo
	new_tio.c_lflag &= (~ICANON & ~ECHO);

	// Set the new settings immediately
	tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);

	// Initialize key mappings for each player
	playerKeyMappings.resize(Configuration::NUMBER_OF_PLAYERS);

	// Define key mappings for up to 4 players
	std::vector<std::unordered_map<int, int>> predefinedMappings = {
		// Player 1: 'W', 'A', 'S', 'D', 'E'
		{
			{int('w'), Configuration::GAMEOBJECT_MOVE_UP},
			{int('a'), Configuration::GAMEOBJECT_MOVE_LEFT},
			{int('s'), Configuration::GAMEOBJECT_MOVE_DOWN},
			{int('d'), Configuration::GAMEOBJECT_MOVE_RIGHT},
			{int('e'), Configuration::GAMEOBJECT_ACTION},
		},
		// Player 2: Arrow keys and Enter
		{
			{128, Configuration::GAMEOBJECT_MOVE_UP},     // Up arrow
			{131, Configuration::GAMEOBJECT_MOVE_LEFT},   // Left arrow
			{129, Configuration::GAMEOBJECT_MOVE_DOWN},   // Down arrow
			{130, Configuration::GAMEOBJECT_MOVE_RIGHT},  // Right arrow
			{10, Configuration::GAMEOBJECT_ACTION},       // Enter
		},
		// Player 3: 'I', 'J', 'K', 'L', 'O'
		{
			{int('I'), Configuration::GAMEOBJECT_MOVE_UP},
			{int('J'), Configuration::GAMEOBJECT_MOVE_LEFT},
			{int('K'), Configuration::GAMEOBJECT_MOVE_DOWN},
			{int('L'), Configuration::GAMEOBJECT_MOVE_RIGHT},
			{int('O'), Configuration::GAMEOBJECT_ACTION},
		},
		// Player 4: 'T', 'F', 'G', 'H', 'Y'
		{
			{int('T'), Configuration::GAMEOBJECT_MOVE_UP},
			{int('F'), Configuration::GAMEOBJECT_MOVE_LEFT},
			{int('G'), Configuration::GAMEOBJECT_MOVE_DOWN},
			{int('H'), Configuration::GAMEOBJECT_MOVE_RIGHT},
			{int('Y'), Configuration::GAMEOBJECT_ACTION},
		},
	};

	// Assign predefined mappings to players
	for (int i = 0; i < Configuration::NUMBER_OF_PLAYERS; ++i) {
		if (i < predefinedMappings.size()) {
			playerKeyMappings[i] = predefinedMappings[i];
		}
		else {
			// If there are more players than predefined mappings, assign a default mapping
			playerKeyMappings[i] = predefinedMappings[0];
		}
	}
}

Input::~Input() {
	// Restore the original terminal settings
	Console::leeren();
	tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);
}


int Input::processInput(int timeout) const {
	unsigned char c;

	struct pollfd fds[1];
	fds[0].fd = STDIN_FILENO;
	fds[0].events = POLLIN;

	auto start = std::chrono::high_resolution_clock::now();

	while (true) {
		int ret = poll(fds, 1, timeout);

		if (ret > 0) {
			int pollTimeout = timeout == 0 ? -1 : timeout;
			int ret = poll(fds, 1, pollTimeout);

			// Check if the first character is an escape character
			if (c == 27) {
				// Try to read the next two characters
				char seq[2];
				if (read(STDIN_FILENO, &seq, 2) == 2) {
					// Check if the sequence is a valid arrow key sequence
					if (seq[0] == '[' && (seq[1] >= 'A' && seq[1] <= 'D')) {
						// Convert the sequence to a key code
						c = seq[1] - 'A' + 128; // 128, 129, 130, 131 for Up, Down, Right, Left
					}
					// Check if the sequence is a valid F1 key sequence
					else if (seq[0] == 'O' && seq[1] == 'P') {
						c = 132; // Use 132 for F1
					}
				}
			}
		}
		else if (ret < 0) {
			// Error
			return -3;
		}
		else {
			// Timeout
			auto elapsed = std::chrono::high_resolution_clock::now() - start;
			if (std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count() >= timeout) {
				return -1;
			}
			continue;
		}

		if (c == 132) { // F1 key
			if (PauseGame()) {
				return -2; // Quit game
			}
		}

		int result = findKeyMapping(c);
		if (result == -1) {
			continue;
		}

		return result;
	}

	return -9; // Unknown error
}

bool Input::PauseGame() const {
	int startx = Configuration::PLAYGROUND_OFFSETX;
	int endx = Configuration::PLAYGROUND_XSIZE + Configuration::PLAYGROUND_OFFSETX;
	int starty = Configuration::PLAYGROUND_YSIZE + Configuration::PLAYGROUND_OFFSETY;
	std::string string = "Press F1 to continue or 'q' to quit";
	std::string erase(string.length(), ' ');

	Console::zeichne_text(startx, starty + 1, endx, string);

	unsigned char c;
	while (true) {
		read(STDIN_FILENO, &c, 1);

		// Check if the first character is an escape character
		if (c == 27) {
			// Try to read the next two characters
			char seq[2];
			if (read(STDIN_FILENO, &seq, 2) == 2) {
				// Check if the sequence is a valid F1 key sequence
				if (seq[0] == 'O' && seq[1] == 'P') {
					Console::zeichne_text(startx, starty + 1, endx, erase);
					string = "Press F1 to pause the game";
					Console::zeichne_text(startx, starty + 1, endx, string);
					return false;
				}
			}
		}
		else if (c == 'q') { //q
			return true;
		}
	}
	return false;
}


#elif defined(_WIN32) //Greift bei Windows 32-bit und 64-bit

Input::Input() {
	// Initialize key mappings for each player
	playerKeyMappings.resize(Configuration::NUMBER_OF_PLAYERS);

	// Define key mappings for up to 4 players
	std::vector<std::unordered_map<int, int>> predefinedMappings = {
		// Player 1: 'W', 'A', 'S', 'D', 'E'
		{
			{'W', Configuration::GAMEOBJECT_MOVE_UP},
			{'A', Configuration::GAMEOBJECT_MOVE_LEFT},
			{'S', Configuration::GAMEOBJECT_MOVE_DOWN},
			{'D', Configuration::GAMEOBJECT_MOVE_RIGHT},
			{'E', Configuration::GAMEOBJECT_ACTION},
		},
		// Player 2: Arrow keys and Enter
		{
			{VK_UP, Configuration::GAMEOBJECT_MOVE_UP},
			{VK_LEFT, Configuration::GAMEOBJECT_MOVE_LEFT},
			{VK_DOWN, Configuration::GAMEOBJECT_MOVE_DOWN},
			{VK_RIGHT, Configuration::GAMEOBJECT_MOVE_RIGHT},
			{VK_RETURN, Configuration::GAMEOBJECT_ACTION},
		},
		// Player 3: 'I', 'J', 'K', 'L', 'O'
		{
			{'I', Configuration::GAMEOBJECT_MOVE_UP},
			{'J', Configuration::GAMEOBJECT_MOVE_LEFT},
			{'K', Configuration::GAMEOBJECT_MOVE_DOWN},
			{'L', Configuration::GAMEOBJECT_MOVE_RIGHT},
			{'O', Configuration::GAMEOBJECT_ACTION},
		},
		// Player 4: 'T', 'F', 'G', 'H', 'Y'
		{
			{'T', Configuration::GAMEOBJECT_MOVE_UP},
			{'F', Configuration::GAMEOBJECT_MOVE_LEFT},
			{'G', Configuration::GAMEOBJECT_MOVE_DOWN},
			{'H', Configuration::GAMEOBJECT_MOVE_RIGHT},
			{'Y', Configuration::GAMEOBJECT_ACTION},
		},
	};

	// Assign predefined mappings to players
	for (int i = 0; i < Configuration::NUMBER_OF_PLAYERS; ++i) {
		if (i < predefinedMappings.size()) {
			playerKeyMappings[i] = predefinedMappings[i];
		}
		else {
			// If there are more players than predefined mappings, assign a default mapping
			playerKeyMappings[i] = predefinedMappings[0];
		}
	}
}

Input::~Input() {
}

/**
 * Reads input from the console and returns the corresponding player action.
 * Returns a negative integer if an error occurs. Timeout will be decreased by
 * the time the function took to execute.
 * Return values:
 *   >=0: Player action,
 *   -1: Timeout,
 *   -2: Quit Game,
 *   -3: Error: Invalid handle,
 *   -4: Error: Unable to get console mode,
 *   -5: Error: Unable to set console mode,
 *   -6: Error: Unable to flush console input buffer,
 *   -7: Error: Unable to read console input,
 *   -8: Error: Unable to restore console mode,
 *   -9: Error: Unknown,
 * @param timeout The timeout in milliseconds. If timeout is 0, no timeout is used.
 * @return The player action or an error code.
 */
int Input::processInput(int timeout) const {
	std::string inStr;
	DWORD fdwMode, fdwOldMode;
	HANDLE hStdIn = GetStdHandle(STD_INPUT_HANDLE);

	if (hStdIn == INVALID_HANDLE_VALUE) {
		return -3; // Error: Invalid handle
	}

	if (!GetConsoleMode(hStdIn, &fdwOldMode)) {
		return -4; // Error: Unable to get console mode
	}

	fdwMode = fdwOldMode ^ ENABLE_WINDOW_INPUT;
	if (!SetConsoleMode(hStdIn, fdwMode)) {
		return -5; // Error: Unable to set console mode
	}

	if (!FlushConsoleInputBuffer(hStdIn)) {
		return -6; // Error: Unable to flush console input buffer
	}

	DWORD waitResult;
	DWORD startTime = GetTickCount();
	DWORD elapsed;
	DWORD waitTime;

	auto start = std::chrono::high_resolution_clock::now();

	while (true) {
		elapsed = GetTickCount() - startTime;
		if (timeout != 0 && elapsed >= timeout) {
			return -1; // Timeout
		}

		waitTime = (timeout == 0) ? INFINITE : timeout - elapsed;
		if (waitTime > timeout) {
			waitTime = 0;
		}

		if (WaitForSingleObject(hStdIn, waitTime) != WAIT_OBJECT_0) {
			continue;
		}

		INPUT_RECORD inputRecord;
		DWORD events = 0;
		if (!ReadConsoleInput(hStdIn, &inputRecord, 1, &events)) {
			return -7; // Error: Unable to read console input
		}

		if (inputRecord.EventType != KEY_EVENT || !inputRecord.Event.KeyEvent.bKeyDown) {
			continue;
		}

		int key = inputRecord.Event.KeyEvent.wVirtualKeyCode;

		if (key == VK_ESCAPE && Input::PauseGame(hStdIn, inputRecord, events, key)) {
			return -2; // Quit game
		}

		int result = findKeyMapping(key);
		if (result == -1) {
			continue;
		}

		return result;
	}

	if (!SetConsoleMode(hStdIn, fdwOldMode)) {
		return -8; // Error: Unable to restore console mode
	}

	return -9; // Unknown error
}

bool Input::PauseGame(HANDLE hStdIn, INPUT_RECORD inputRecord, DWORD events, int key) const {
	int startx = Configuration::PLAYGROUND_OFFSETX;
	int endx = Configuration::PLAYGROUND_XSIZE + Configuration::PLAYGROUND_OFFSETX;
	int starty = Configuration::PLAYGROUND_YSIZE + Configuration::PLAYGROUND_OFFSETY;
	std::string string = "Press ESC to continue or 'q' to quit";
	std::string erase(string.length(), ' ');

	Console::zeichne_text(startx, starty + 1, endx, string);

	while (true) {
		WaitForSingleObject(hStdIn, INFINITE);
		ReadConsoleInput(hStdIn, &inputRecord, 1, &events);

		if (inputRecord.EventType == KEY_EVENT && inputRecord.Event.KeyEvent.bKeyDown) {
			key = inputRecord.Event.KeyEvent.wVirtualKeyCode;

			if (key == 0x51) { //q
				return true;
			}
			else if (key == VK_ESCAPE) {
				Console::zeichne_text(startx, starty + 1, endx, erase);
				string = "Press ESC to pause the game";
				Console::zeichne_text(startx, starty + 1, endx, string);
				return false;
			}
		}
	}
	return false;
}

#else //Betriebssystem unbekannt / andere Betriebssysteme
char Input::getSteuerung(long interval) {
	char eingabe;
	std::cin >> eingabe;
	return eingabe;
}
char Input::getWASD(long interval) {
	return getSteuerung();
}
char Input::getArrow(long interval) {
	return getSteuerung();
}

#endif
