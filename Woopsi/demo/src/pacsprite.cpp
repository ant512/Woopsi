#include "pacsprite.h"
#include "pacman.h"
#include "pacmap.h"
#include "superbitmap.h"

PacSprite::PacSprite(PacMan* game) {
	_game = game;
	_x = 0;
	_y = 0;
	_width = 5;
	_height = 5;
	_colour = woopsiRGB(0, 0, 0);
}

void PacSprite::draw() {
	_game->getSuperBitmap()->getGraphics()->drawFilledRect(_x, _y, _width, _height, _colour);
}

bool PacSprite::checkCollision(PacSprite* sprite) {
	if ((_x + _width >= sprite->getX()) && (_x <= sprite->getX() + sprite->getWidth())) {
		if ((_y + _width >= sprite->getY()) && (_y <= sprite->getY() + sprite->getHeight())) {
			return true;
		}
	}
	
	return false;
}

bool PacSprite::checkMove(u8 newDirection, u8 speed, bool isGhost) {
	if (newDirection > 0) {
	
		// Get next co-ords
		s16 nextX = _x;
		s16 nextY = _y;
		
		switch (newDirection) {
			case 1:
				nextY -= speed;
				break;
			case 2:
				nextY += speed;
				break;
			case 3:
				nextX -= speed;
				break;
			case 4:
				nextX += speed;
				break;
		}

		
		// Wrap-around
		if (nextX < 0) {
			nextX = _game->getWidth() - _width;
		} else if (nextX + _width > _game->getWidth()) {
			nextX = 0;
		}
		
		// Check collision
		if (_game->getMap()->checkBlock(nextX, nextY, isGhost)) {
			
			// Move OK!
			return true;
		}
	}
	
	return false;
}

bool PacSprite::move(u8 newDirection, u8 speed, bool isGhost) {
	if (newDirection > 0) {
	
		// Get next co-ords
		s16 nextX = _x;
		s16 nextY = _y;
		
		switch (newDirection) {
			case 1:
				nextY -= speed;
				break;
			case 2:
				nextY += speed;
				break;
			case 3:
				nextX -= speed;
				break;
			case 4:
				nextX += speed;
				break;
		}

		
		// Wrap-around
		if (nextX < 0) {
			nextX = _game->getWidth() - _width;
		} else if (nextX + _width > _game->getWidth()) {
			nextX = 0;
		}

		// Check collision
		if (_game->getMap()->checkBlock(nextX, nextY, isGhost)) {

			// Erase old
			_game->getSuperBitmap()->getGraphics()->drawFilledRect(_x, _y, _width, _height, woopsiRGB(31, 31, 31));

			_x = nextX;
			_y = nextY;

			// Draw new
			draw();
			
			// Move OK!
			return true;
		}
	}

	draw();
	
	return false;
}
