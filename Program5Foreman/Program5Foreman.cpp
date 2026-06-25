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
#include <time.h>
#include <cstdlib>


int collided(int x, int y);
bool endValue(int x, int y);
int main(void) {

	srand(time(0));

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
	bad bads[10];
	int num_bads = 10;
	bullet bullets;
	bool done = false;
	bool render = false;



	ALLEGRO_DISPLAY* display = NULL;
	ALLEGRO_EVENT_QUEUE* event_queue = NULL;
	ALLEGRO_TIMER* timer;
	ALLEGRO_BITMAP* image = NULL;
	ALLEGRO_SAMPLE* sample1 = al_load_sample("pop.wav");
	ALLEGRO_SAMPLE* sample2 = al_load_sample("background.wav");
	if (!sample1 || !sample2)
		return -5;

	al_play_sample(sample2, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);

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


	bool intro = true;
	while (intro) {
		ALLEGRO_EVENT ev;

		al_draw_text(font, al_map_rgb(255, 255, 255), WIDTH / 4, 100 , 0, "You are trapped in a time loop with aliens.");
		al_draw_text(font, al_map_rgb(255, 255, 255), WIDTH / 4, 150, 0, "each loop time gets shorter");
		al_draw_text(font, al_map_rgb(255, 255, 255), WIDTH / 4, 200, 0, "kill all bads before time runs out or you will die.");
		al_draw_text(font, al_map_rgb(255, 255, 255), WIDTH / 4, 250, 0, "arrow keys to move, space to shoot.");
		al_draw_text(font, al_map_rgb(255, 255, 255), WIDTH / 4, 300, 0, "press any key to begin");
		al_flip_display();

		al_wait_for_event(event_queue, &ev);
		if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			intro = false;
		}


	}


	if (MapLoad("Level1.fmp", 1)) {
		return -5;
	}
	//draw the background tiles
	MapDrawBG(xOff, yOff, 0, 0, WIDTH - 1, HEIGHT - 1);

	//draw foreground tiles
	MapDrawFG(xOff, yOff, 0, 0, WIDTH - 1, HEIGHT - 1, 0);
	player.DrawSprites(xOff, yOff);

	int time;
	int tick = 0;
	int level = 0;
	bool leveling;
	while (!done) {
		for (int i = 0; i < num_bads; i++) {
			int badx = rand() % (mapwidth * mapblockwidth);
			int bady = rand() % (mapheight * mapblockheight);
			if (bady < 32)
				bady += 32;
			if (badx < 32)
				badx += 32;
			if (bady > HEIGHT - 32)
				bady -= 32;
			bads[i].spawnBad(badx, bady);
			bads[i].drawBad(0, xOff, yOff);
		}
		al_flip_display();
		al_clear_to_color(al_map_rgb(0, 0, 0));

		leveling = true;
		level += 1;
		if (level == 1)
			time = 120;
		else if (level == 2)
			time = 100;
		else if (level == 3)
			time = 80;
		else if (level == 4)
			time = 60;
		else if (level == 5) {
			done = true;
			leveling = false;
		}
		int tick = 0;

		while (leveling) {

			tick += 1;
			if (tick % 60 == 0)
				time -= 1;
			if (time <= 0)
				leveling = false;
			if (player.getScore() == 10) {
				leveling = false;
				player.resetScore();
				player.setX(80);
				player.setY(50);
			}

			ALLEGRO_EVENT ev;
			al_wait_for_event(event_queue, &ev);

			al_start_timer(timer);

			if (ev.type == ALLEGRO_EVENT_TIMER)
			{


				MapUpdateAnims();
				bullets.updateBullet(WIDTH, HEIGHT);
				bullets.collideBullet(bads, num_bads, player);
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
					al_play_sample(sample1, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
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
				for (int i = 0; i < num_bads; i++) {
					bads[i].drawBad(bads[i].getVersion(), xOff, yOff);
				}
				bullets.drawBullet(xOff, yOff);
				player.DrawSprites(xOff, yOff);
				std::string timeTook = std::to_string(time);
				al_draw_text(font, al_map_rgb(255, 255, 255), WIDTH - 350, HEIGHT - 100, 0, "YOU HAVE");
				al_draw_text(font, al_map_rgb(255, 255, 255), WIDTH - 200, HEIGHT - 100, 0, timeTook.c_str());
				al_draw_text(font, al_map_rgb(255, 255, 255), WIDTH - 150, HEIGHT - 100, 0, "LEFT");
				al_flip_display();
				al_clear_to_color(al_map_rgb(0, 0, 0));
			}
		}
	}
	al_clear_to_color(al_map_rgb(0, 0, 0));
	if (level >= 5) {
		al_draw_text(font, al_map_rgb(255, 255, 255), WIDTH/2, HEIGHT/2, 0, "YOU MADE IT OUT");
	}
	else if (level < 5) { 
		al_draw_text(font, al_map_rgb(255, 255, 255), WIDTH / 2, HEIGHT / 2, 0, "YOU WERE TRAPPED FOREVER IN THE LOOP");
	}
	al_flip_display();
	al_rest(5.0);
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