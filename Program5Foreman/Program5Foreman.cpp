// Program5Foreman.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <iostream>
#include <string>
#include <allegro5/allegro.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "mappy_A5.h"
#include "SpriteSheet.h"
#include "bad.h"
#include "bullet.h"


int collided(int x, int y);
bool endValue(int x, int y);
int main(void) {

	if (!al_init()) {
		return -1;
	}
	al_install_audio();
	al_init_acodec_addon();
	al_install_keyboard();
	al_init_image_addon();
	al_init_primitives_addon();
	al_init_ttf_addon();
	al_init_font_addon();

	al_reserve_samples(10);

	const int WIDTH = 900;
	const int HEIGHT = 480;
	bool keys[] = { false, false, false, false, false };
	enum KEYS { UP, DOWN, LEFT, RIGHT, SPACE };
	Sprite player;
	bad bads;
	bullet bullets;
	bool done = false;
	bool render = false;



	ALLEGRO_DISPLAY* display = NULL;
	ALLEGRO_EVENT_QUEUE* event_queue = NULL;
	ALLEGRO_TIMER* timer;
	ALLEGRO_BITMAP* image = NULL;
	ALLEGRO_SAMPLE* sample = al_load_sample("pop.wav");
	if (!sample)
		return -5;

	display = al_create_display(WIDTH, HEIGHT);
	if (!display) {
		return -1;
	}
	ALLEGRO_FONT* font = al_load_font("Cat.ttf", 30, 0);
	if (!font) {
		return -5;
	}
	
	player.InitSprites(WIDTH, HEIGHT);

	int xOff = 0;
	int yOff = 0;

	event_queue = al_create_event_queue();
	timer = al_create_timer(1.0 / 60);

	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	int time = 60;
	int tick = 0;
	if (MapLoad("Level1.fmp", 1)) {
		return -5;
	}
	//draw the background tiles
	MapDrawBG(xOff, yOff, 0, 0, WIDTH - 1, HEIGHT - 1);

	//draw foreground tiles
	MapDrawFG(xOff, yOff, 0, 0, WIDTH - 1, HEIGHT - 1, 0);
	player.DrawSprites(xOff, yOff);

	bads.spawnBad(50, 200);
	bads.drawBad(0);
	al_flip_display();
	al_clear_to_color(al_map_rgb(0, 0, 0));

	while (!done) {
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		al_start_timer(timer);

		if (ev.type == ALLEGRO_EVENT_TIMER)
		{
			MapUpdateAnims();
			bullets.updateBullet(WIDTH, HEIGHT);
			render = true;
			if (keys[LEFT])
				player.UpdateSprites(WIDTH, HEIGHT, 0);
			else if (keys[RIGHT])
				player.UpdateSprites(WIDTH, HEIGHT, 1);
			else if (keys[UP])
				player.UpdateSprites(WIDTH, HEIGHT, 3);
			else if (keys[DOWN])
				player.UpdateSprites(WIDTH, HEIGHT, 4);
			else if (keys[SPACE]) {
				bullets.fireBullet(player, player.getAngle(), xOff, yOff);
				al_play_sample(sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
			}
			else
				player.UpdateSprites(WIDTH, HEIGHT, 2);
		}
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			done = true;
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			case ALLEGRO_KEY_UP:
				keys[UP] = true;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = true;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = true;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = true;
				break;
			case ALLEGRO_KEY_SPACE:
				keys[SPACE] = true;
				break;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			case ALLEGRO_KEY_UP:
				keys[UP] = false;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = false;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = false;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = false;
				break;
			case ALLEGRO_KEY_SPACE:
				keys[SPACE] = false;
				break;
			}
		}
		if (render && al_is_event_queue_empty(event_queue))
		{
			render = false;

			//update the map scroll position
			xOff = player.getX() + player.getWidth() - WIDTH / 2;
			yOff = player.getY() + player.getHeight() - HEIGHT / 2;

			//avoid moving beyond the map edge
			if (xOff < 0) xOff = 0;

			if (xOff > (mapwidth * mapblockwidth - WIDTH))
				xOff = mapwidth * mapblockwidth - WIDTH;
			if (yOff < 0)
				yOff = 0;
			if (yOff > (mapheight * mapblockheight - HEIGHT))
				yOff = mapheight * mapblockheight - HEIGHT;


			//draw the background tiles
			MapDrawBG(xOff, yOff, 0, 0, WIDTH, HEIGHT);

			//draw foreground tiles
			MapDrawFG(xOff, yOff, 0, 0, WIDTH, HEIGHT, 0);
			bads.drawBad(0);
			bullets.drawBullet(xOff, yOff);
			player.DrawSprites(xOff, yOff);
			al_flip_display();
			al_clear_to_color(al_map_rgb(0, 0, 0));
		}
	}
}
int collided(int x, int y)
{
	BLKSTR* blockdata;
	blockdata = MapGetBlock(x / mapblockwidth, y / mapblockheight);
	if (blockdata == NULL) {
		return 0;
	}
	return blockdata->tl;
}
bool endValue(int x, int y) {
	BLKSTR* data;
	data = MapGetBlock(x / mapblockwidth, y / mapblockheight);

	if (data->user1 == 8)
	{
		return true;
	}
	else
		return false;
}