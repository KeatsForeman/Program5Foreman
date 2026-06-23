#include "SpriteSheet.h"

int collided(int x, int y);
bool endValue(int x, int y);

Sprite::Sprite()
{
	image = NULL;
}
Sprite::~Sprite()
{
	al_destroy_bitmap(image);
}
void Sprite::InitSprites(int width, int height)
{
	x = 80;
	y = 50;


	maxFrame = 8;
	curFrame = 0;
	frameCount = 0;
	frameDelay = 6;
	frameWidth = 50;
	frameHeight = 64;
	animationColumns = 8;
	animationDirection = 1;
	isJumping = false;
	angle = 0;

	image = al_load_bitmap("guy.bmp");
	al_convert_mask_to_alpha(image, al_map_rgb(255, 0, 255));
}
void Sprite::UpdateSprites(int width, int height, int dir)
{
	int oldx = x;
	int oldy = y;
	if (dir == 4) { //down
		animationDirection = 1;
		angle = ALLEGRO_PI / 2;
		y += 4;
		if (++frameCount > frameDelay)
		{
			frameCount = 0;
			if (++curFrame > maxFrame - 4)
				curFrame = 1;
		}
	}
	else if (dir == 3) { //up
		animationDirection = 0;
		angle = 3 * ALLEGRO_PI / 2;
		y -= 4;
		if (++frameCount > frameDelay)
		{
			frameCount = 0;
			if (++curFrame > maxFrame - 4)
				curFrame = 1;
		}
	}
	else if (dir == 1) { //right key
		animationDirection = 1;
		angle = 0;
		x += 4;
		if (++frameCount > frameDelay)
		{
			frameCount = 0;
			if (++curFrame > maxFrame - 4)
				curFrame = 1;
		}
	}
	else if (dir == 0) { //left key
		animationDirection = 0;
		angle = ALLEGRO_PI;
		x -= 4;
		if (++frameCount > frameDelay)
		{
			frameCount = 0;
			if (++curFrame > maxFrame - 4)
				curFrame = 1;
		}
	}
	//check for collided with foreground tiles

		//checks the feet
	if ((collided(x, y + frameHeight - 1)) || (collided(x + frameWidth, y + frameHeight - 1))) { //collision detection to the left
		x = oldx;
		y = oldy;
	}
}

bool Sprite::CollisionEndBlock()
{
	if (endValue(x + frameWidth + 5, y + frameHeight - 1))
		return true;
	else
		return false;
}

void Sprite::DrawSprites(int xoffset, int yoffset)
{
	int fx = (curFrame % animationColumns) * frameWidth;
	int fy = (curFrame / animationColumns) * frameHeight;

	if (animationDirection == 1) {
		al_draw_bitmap_region(image, fx, fy, frameWidth, frameHeight, x - xoffset, y - yoffset, 0);
	}
	else if (animationDirection == 0) {
		al_draw_bitmap_region(image, fx, fy, frameWidth, frameHeight, x - xoffset, y - yoffset, ALLEGRO_FLIP_HORIZONTAL);
	}
	else if (animationDirection == 2) {
		al_draw_bitmap_region(image, 0, 0, frameWidth, frameHeight, x - xoffset, y - yoffset, 0);

	}
}
