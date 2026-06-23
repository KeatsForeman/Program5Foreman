#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
#include "bullet.h"
#include <iostream>

int collided(int x, int y);
bullet::bullet() {
	speed = 10;
	live = false;
	radian_angle = 0;
	image = al_load_bitmap("bullet.png");
}
bullet::~bullet() {
	al_destroy_bitmap(image);
}
void bullet::drawBullet(int xoff, int yoff) {
	if (live)
		al_draw_bitmap(image, x - xoff, y - yoff, 0);
}
//fix this
void bullet::fireBullet(Sprite &Player, float angle, int xoff, int yoff) {
	if (!live) {
		x = (Player.getX() + 10);
		y = (Player.getY() + 20);
		radian_angle = angle;
		live = true;
	}
}
void bullet::updateBullet(int WIDTH, int HEIGHT) {
	x += (5 * cos(radian_angle));
	y += (5 * sin(radian_angle));
	if (((y + 10 <= 0) || (y + 10 > HEIGHT)) || (collided(x, y)))
		live = false;
}
//make this
void bullet::collideBullet(bad Bads[], int num_bads, Sprite& Player) {
	if (live) {
		for (int i = 0; i < num_bads; i++) {
		int fx = Bads[i].getX();
		int fy = Bads[i].getY();
		int bx = Bads[i].getBoundX();
		int by = Bads[i].getBoundY();
		if (x > (fx - bx) &&
			x < (fx + bx) &&
			y >(fy - by) &&
			y < (fy + by)) {
			live = false;
			Bads[i].setLive(false);
			Player.addScore();
			}
		}
	}
}

bool bullet::getLive() { return live; }
int bullet::getX() { return x; }
int bullet::getY() { return y; }