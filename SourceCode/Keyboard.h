#pragma once

#include "KeyManager.h"
#include <allegro5/keyboard.h>
#include <allegro5/mouse.h>

class Keyboard : public KeyManager
{
public:
	Keyboard(ALLEGRO_EVENT_QUEUE* queue)
	: KeyManager() 
	{ 
		al_install_keyboard();
		al_install_mouse();
		al_register_event_source(queue, al_get_keyboard_event_source());
		al_register_event_source(queue, al_get_mouse_event_source());

	};
};