#include "pacplayer.h"
#include "pacman.h"
#include "pacghost.h"
#include "pacmap.h"
#include "pacghosts.h"
#include "superbitmap.h"

PacPlayer::PacPlayer(PacMan* game) : PacSprite(game) {
	_x = 45;
	_y = 60;
	_width = 5;
	_height = 5;
	_bufferedDirection = 0;
	_currentDirection = 1;	// 1 = up, 2 = down, 3 = left, 4 = right
	_speed = 1;
	_colour = woopsiRGB(28, 28, 0);
	_lives = 3;

	draw();
}

void PacPlayer::reset() {
	_bufferedDirection = 0;
	_currentDirection = 0;
	_x = 45;
	_y = 60;
}

void PacPlayer::loseLife() {
	reset();
	_game->getGhosts()->reset();
	_lives--;
	
	if (_lives == 0) {
		_game->endGame();
	} else {
		_game->getMap()->draw();
		_game->getGhosts()->draw();
		draw();
	}
}

void PacPlayer::move() {

	// Attempt to switch to new direction
	if (PacSprite::move(_bufferedDirection, _speed, false)) {
		_currentDirection = _bufferedDirection;
		_bufferedDirection = 0;
	} else {
		
		// Keep current direction
		PacSprite::move(_currentDirection, _speed, false);			
	}
	
	_game->getMap()->clearBlock(_x, _y);
}
