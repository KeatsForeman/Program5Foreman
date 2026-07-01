#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
#include "bad.h"
#include <iostream>

int collided(int x, int y);
bad::bad() {
	speed = 2;
	live = false;
	boundx = 32;
	boundy = 32;
	version = 0;
	image[0] = al_load_bitmap("bad.png");
	al_convert_mask_to_alpha(image[0], al_map_rgb(255, 0, 255));
	image[1] = al_load_bitmap("badDead.png");
	al_convert_mask_to_alpha(image[1], al_map_rgb(255, 0, 255));
	if (!image[0]) {
		std::cout << "FAILED TO LOAD bad.png\n";
	}
}
bad::~bad() {
	al_destroy_bitmap(image[0]);
}
void bad::drawBad(int v, int xoff, int yoff) {
	if (live)
		al_draw_bitmap(image[v], x - xoff, y - yoff, 0);
}
void bad::spawnBad(int xx, int yy) {
	x = xx;
	y = yy;
	version = 0;
	live = true;
}
//add player here for score
void bad::updateBad(Sprite& Player) {
	int tempx = x;
	int tempy = y;
	if (version == 0) {
		if (x < Player.getX())
			x += 1;
		else if (x > Player.getX())
			x -= 1;
		if (y < Player.getY() + 10)
			y += 1;
		else if (y > Player.getY() + 10)
			y -= 1;
	}
	if (collided(x, y) || (collided(x + 32, y + 32)) || (collided(x, y + 32)) || (collided(x + 32, y))) {
		x = tempx;
		y = tempy;
	}
}