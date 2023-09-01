#pragma once

#include <allegro5/keyboard.h>
#include <unordered_map>
#define KEY_SEEN     1
#define KEY_RELEASED 2

enum EButton
{
	Up = 0,
	Down,
	Left,
	Right,
	Exit,
	Count
};

class KeyManager
{
public:
	KeyManager(int up, int down, int left, int right, int exit = ALLEGRO_KEY_ESCAPE);
	void setButton(EButton button, int code);
	KeyManager() : KeyManager(ALLEGRO_KEY_UP, ALLEGRO_KEY_DOWN, ALLEGRO_KEY_LEFT, ALLEGRO_KEY_RIGHT, ALLEGRO_KEY_ESCAPE) {};

	void resetButtonState();
	unsigned char buttonPress(int button, unsigned char state);
	unsigned char buttonPress(EButton button);
	unsigned char buttonRelease(int button, unsigned char state);
	bool exitGame();

private:

	std::unordered_map<EButton, unsigned char> buttonPressed_;
	std::unordered_map<int, EButton> buttonMap_;
};