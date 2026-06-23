#pragma once
#ifndef BAD_H
#define BAD_H

class bad {
public:
	bad();
	~bad();
	void drawBad(int v);
	void spawnBad(int x, int y);
	void updateBad();
	void setLive(bool new_live) { live = new_live; };
	bool getLive();
	int getX() { return x; }
	int getY() { return y; }
	int getBoundX() { return boundx; }
	int getBoundY() { return boundy; }
	void setVersion(int v) { version = v; }
	int getVersion() { return version; }

private:
	int x, y;
	int boundx, boundy;
	bool live;
	int speed;
	int version;
	ALLEGRO_BITMAP* image;
};

#endif