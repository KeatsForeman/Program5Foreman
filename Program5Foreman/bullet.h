#pragma once
#ifndef BULLET_H
#define BULLET_H
#include "bad.h"
#include "SpriteSheet.h"

class bullet {
	friend int collided(int x, int y);
public:
	bullet();
	~bullet();
	void drawBullet(int xoff, int yoff);
	void fireBullet(Sprite &player, float angle, int xoff, int yoff);
	void updateBullet(int WIDTH, int HEIGHT);
	void collideBullet(bad Bads[], int num_bads, Sprite& Player);
	bool getLive();
	int getX();
	int getY();

private:
	int x, y;
	int speed;
	bool live;
	float radian_angle;
	ALLEGRO_BITMAP* image;
};


#endif