#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include "Controller.h"
#include "Keyboard.h"
#include "Settings.h"

#define AxisDeadZone 0.05f

enum XboxAxis {
	LeftStickAxis = 0,
	RightStickAxis,
	LeftTrigger,
	RightTrigger,
};


void must_init(bool test, const char* description)
{
	if (test) return;

	printf("couldn't initialize %s\n", description);
	exit(1);
}

int main()
{
	must_init(al_init(), "allegro");

	ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
	must_init(timer, "timer");

	ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
	must_init(queue, "queue");

	al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
	al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
	al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

	ALLEGRO_DISPLAY* disp = al_create_display(640, 480);
	must_init(disp, "display");

	ALLEGRO_FONT* font = al_create_builtin_font();
	must_init(font, "font");

	must_init(al_init_primitives_addon(), "primitives");

	al_register_event_source(queue, al_get_display_event_source(disp));
	al_register_event_source(queue, al_get_timer_event_source(timer));

	bool done = false;
	bool redraw = true;
	ALLEGRO_EVENT event;

	float x, y;
	float i, j;
	x = 100;
	y = 100;

	i = 100;
	j = 100;

	Settings::Settings* settings = new Settings::Settings();

	KeyManager* key;
	
	if (!settings->controllerType)
	{
		key = new Keyboard(queue);
	}
	else
	{
		key = new Controller(queue);
	}

	al_start_timer(timer);

	float joys[3][2]{0};

	while (1)
	{
		al_wait_for_event(queue, &event);

		switch (event.type)
		{
		case ALLEGRO_EVENT_TIMER:
			if (key->buttonPress(EButton::Up))
			{
				y--;
			}
			if (key->buttonPress(EButton::Down))
			{
				y++;
			}
			if (key->buttonPress(EButton::Left))
			{
				x--;
			}
			if (key->buttonPress(EButton::Right))
			{
				x++;
			}

			done = key->exitGame();

			x += joys[0][0];
			y += joys[0][1];

			i += joys[1][0];
			j += joys[1][1];

			key->resetButtonState();

			redraw = true;
			break;

		case ALLEGRO_EVENT_KEY_DOWN:
			key->buttonPress(event.keyboard.keycode, KEY_SEEN | KEY_RELEASED);
			break;
		case ALLEGRO_EVENT_KEY_UP:
			key->buttonRelease(event.keyboard.keycode, KEY_RELEASED);
			break;
		case ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN:
			key->buttonPress(event.joystick.button, KEY_SEEN | KEY_RELEASED);
			break;
		case ALLEGRO_EVENT_JOYSTICK_BUTTON_UP:
			key->buttonRelease(event.joystick.button, KEY_RELEASED);
			break;
		case ALLEGRO_EVENT_JOYSTICK_AXIS:
			
				if (event.joystick.stick == 0 && abs(event.joystick.pos) > AxisDeadZone) { //float joys[3][2]
					joys[event.joystick.stick][event.joystick.axis] = event.joystick.pos;
				}
				else if(event.joystick.stick == 0)
				{
					joys[event.joystick.stick][event.joystick.axis] = 0;
				}
			
				if (event.joystick.stick == 1 && abs(event.joystick.pos) > AxisDeadZone) { //float joys[3][2]
					joys[event.joystick.stick][event.joystick.axis] = event.joystick.pos;
				}
				else if (event.joystick.stick == 1)
				{
					joys[event.joystick.stick][event.joystick.axis] = 0;
				}

			break;

		case ALLEGRO_EVENT_DISPLAY_CLOSE:
			done = true;
			break;
		}

		if (done)
			break;

		if (redraw && al_is_event_queue_empty(queue))
		{
			al_clear_to_color(al_map_rgb(0, 0, 0));
			al_draw_textf(font, al_map_rgb(255, 255, 255), 0, 0, 0, "X: %.1f Y: %.1f", x, y);
			al_draw_textf(font, al_map_rgb(255, 255, 255), 0, 20, 0, "X: %.1f Y: %.1f", i, j);
			al_draw_filled_rectangle(x, y, x + 10, y + 10, al_map_rgb(255, 0, 0));
			al_draw_filled_rectangle(i, j, i + 10, j + 10, al_map_rgb(255, 255, 0));

			al_flip_display();

			redraw = false;
		}
	}

	al_destroy_font(font);
	al_destroy_display(disp);
	al_destroy_timer(timer);
	al_destroy_event_queue(queue);

	return 0;
}