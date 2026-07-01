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
		al_draw_rotated_bitmap(image, 9, 3, x - xoff, y - yoff, radian_angle, 0);
}
//fix this
void bullet::fireBullet(Sprite &Player, float angle, int xoff, int yoff) {
	if (!live) {
		x = (Player.getX() + 25);
		y = (Player.getY() + 30);
		radian_angle = angle;
		live = true;
	}
}
void bullet::updateBullet(int WIDTH, int HEIGHT) {
	x += (speed * cos(radian_angle));
	y += (speed * sin(radian_angle));
	if (((y + 10 <= 0) || (y + 10 > HEIGHT)) || (collided(x, y)))
		live = false;
}
//make this
void bullet::collideBullet(bad Bads[], int num_bads, Sprite& Player) {
	if (live) {
		for (int i = 0; i < num_bads; i++) {
			if (Bads[i].getVersion() == 0) {
				int fx = Bads[i].getX();
				int fy = Bads[i].getY();
				int bx = Bads[i].getBoundX();
				int by = Bads[i].getBoundY();
				if (x > fx && x < fx + bx && y > fy && y < fy + bx) {
					live = false;
					Player.addScore();
					Bads[i].setVersion(1);
				}
			}
		}
	}
}

bool bullet::getLive() { return live; }
int bullet::getX() { return x; }
int bullet::getY() { return y; }