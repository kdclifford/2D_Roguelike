#pragma once

#include "KeyManager.h"
#include <allegro5/joystick.h>

//Xbox Controller

enum EXboxButton
{
	A = 0,
	B,
	X,
	Y,
	RightBumper,
	LeftBumper,
	RightStick,
	LeftStick,
	Share,
	Start,
	DpadRight,
	DpadLeft,
	DpadDown,
	DpadUp,
	XboxCount
};

class Controller : public KeyManager
{
public:
	Controller(ALLEGRO_EVENT_QUEUE* queue) 
	: KeyManager(EXboxButton::DpadUp, EXboxButton::DpadDown, EXboxButton::DpadLeft, EXboxButton::DpadRight, EXboxButton::Y) 
	{ 
		al_install_joystick(); 
		al_register_event_source(queue, al_get_joystick_event_source());
	};
};