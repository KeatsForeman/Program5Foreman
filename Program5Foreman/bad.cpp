#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
#include "bad.h"
#include <iostream>

bad::bad() {
	speed = 2;
	live = false;
	boundx = 32;
	boundy = 32;
	version = 0;
	image = al_load_bitmap("bad.png");
	if (!image) {
		std::cout << "FAILED TO LOAD bad.png\n";
	}
}
bad::~bad() {
	al_destroy_bitmap(image);
}
void bad::drawBad(int v) {
	if (live)
		al_draw_bitmap(image, x, y, 0);
}
void bad::spawnBad(int xx, int yy) {
	x = xx;
	y = yy;
	version = 0;
	live = true;
}
//add player here for score
void bad::updateBad() {
	
}