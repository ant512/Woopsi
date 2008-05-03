#include <stdlib.h>
#include "pacghost.h"
#include "pacman.h"
#include "pacplayer.h"
#include "superbitmap.h"

PacGhost::PacGhost(PacMan* game) : PacSprite(game) {
	_width = 5;
	_height = 5;
	_type = 0;
	_colour = woopsiRGB(0, 31, 31);

	reset();
}

void PacGhost::reset() {
	_x = 45;
	_y = 50;
	_currentDirection = 0;
	_bufferedDirection = 0;
	_speed = 1;
	_leftHome = false;
	_active = false;
}

void PacGhost::leaveHome() {

	// Erase old
	_game->getSuperBitmap()->drawFilledRect(_x, _y, _width, _height, woopsiRGB(31, 31, 31));

	_y -= _speed;

	// Draw new
	draw();

	if (_y == 40) {
		_leftHome = true;
	}
}

void PacGhost::move() {
	
	if (_active) {
		
		// Collision detection
		if (checkCollision(_game->getPlayer())) {
			_game->getPlayer()->loseLife();
		} else {
	
			// Movement
			if (!_leftHome) {
				// Exit safe area
				leaveHome();
			} else {
				// Move the sprite
				PacSprite::move(_currentDirection, _speed, false);
				
				// AI routines
				switch (_type) {
					case 0:
						doDumbAI();
						break;
					case 1:
						doSmartAI();
						break;
				}
			}
		}
	}
}

void PacGhost::doDumbAI() {
	
	// Work out which directions we can travel in and store them in an array; do not allow the
	// ghost to reverse direction
	DynamicArray<u8> directions;

	// Check up		
	if (checkMove(1, _speed, false) && (_currentDirection != 2)) {
		directions.push_back(1);
	}
	
	// Check down
	if (checkMove(2, _speed, false) && (_currentDirection != 1)) {
		directions.push_back(2);
	}
	
	// Check left
	if (checkMove(3, _speed, false) && (_currentDirection != 4)) {
		directions.push_back(3);
	}
	
	// Check right
	if (checkMove(4, _speed, false) && (_currentDirection != 3)) {
		directions.push_back(4);
	}
	
	// Choose a random direction
	_currentDirection = directions[rand() % directions.size()];
}

// Smart AI will take the shortest route to the player, but cannot change direction on a straight
void PacGhost::doSmartAI() {
	
	// Work out which directions we can travel in using a bitmask; do not allow the ghost to
	// reverse direction
	u8 directions = 0;

	// Check up		
	if (checkMove(1, _speed, false) && (_currentDirection != 2)) {
		directions += 1;
	}
	
	// Check down
	if (checkMove(2, _speed, false) && (_currentDirection != 1)) {
		directions += 2;
	}
	
	// Check left
	if (checkMove(3, _speed, false) && (_currentDirection != 4)) {
		directions += 4;
	}
	
	// Check right
	if (checkMove(4, _speed, false) && (_currentDirection != 3)) {
		directions += 8;
	}
	
	// Choose a new direction to travel in
	if (directions > 0) {
		
		// Work out distance components to player
		s16 hDist = _x - _game->getPlayer()->getX();
		s16 vDist = _y - _game->getPlayer()->getY();
		
		// Force the ghost to choose a perpendicular direction if it is on a line with the
		// player but heading in the wrong direction
		if (hDist == 0) {
			if ((vDist > 0) && (_currentDirection == 2)) {
				// Player immediately above ghost and ghost heading downwards
				hDist = _game->getWidth() * 2;
			} else if ((vDist < 0) && (_currentDirection == 1)) {
				// Player immediately below ghost and ghost heading upwards
				hDist = _game->getWidth() * 2;
			}
		} else if (vDist == 0) {
			if ((hDist > 0) && (_currentDirection == 4)) {
				// Player immediately to the left of the ghost and ghost heading right
				vDist = _game->getHeight() * 2;
			} else if ((hDist < 0) && (_currentDirection == 3)) {
				// Player immediately to the right of the ghost and ghost heading left
				vDist = _game->getHeight() * 2;
			}
		}
		
		// Which way do we go?
		if (abs(hDist) > abs(vDist)) {
			// Try to go horizontally (the longest distance) first
			if (hDist > 0) {

				// Player is to the left
				if (directions & 4) {
					_currentDirection = 3;
				} else {
					// Cannot go left, so try the best vertical distance
					if ((vDist > 0) && (directions & 1)) {
						//Sprite above, so try to go up
						_currentDirection = 1;
					} else if ((vDist < 0) && (directions & 2)) {
						//Sprite below, so try to go down
						_currentDirection = 2;
					} else if (directions & 1) {
						// Try going up ignoring distance
						_currentDirection = 1;
					} else if (directions & 2) {
						// Try going down ignoring distance
						_currentDirection = 2;
					} else if (directions & 8) {
						// No choice but to go right
						_currentDirection = 4;
					}
				}
			} else {

				// Player is to the right
				if (directions & 8) {
					_currentDirection = 4;
				} else {
					// Cannot go right, so try the best vertical distance
					if ((vDist > 0) && (directions & 1)) {
						// Sprite above, so try to go up
						_currentDirection = 1;
					} else if ((vDist < 0) && (directions & 2)) {
						// Sprite below, so try to go down
						_currentDirection = 2;
					} else if (directions & 1) {
						// Try going up ignoring distance
						_currentDirection = 1;
					} else if (directions & 2) {
						// Try going down ignoring distance
						_currentDirection = 2;
					} else if (directions & 4) {
						// No choice but to go left
						_currentDirection = 3;
					}
				}
			}
		} else {
			// Try to go vertically
			if (vDist > 0) {

				// Player is above
				if (directions & 1) {
					_currentDirection = 1;
				} else {
					// Cannot go up, so try the best horizontal distance
					if ((hDist > 0) && (directions & 4)) {
						//Sprite to the left, so try to go left
						_currentDirection = 3;
					} else if ((hDist < 0) && (directions & 8)) {
						//Sprite to the right, so try to go right
						_currentDirection = 4;
					} else if (directions & 4) {
						// Try going left ignoring distance
						_currentDirection = 3;
					} else if (directions & 8) {
						// Try going right ignoring distance
						_currentDirection = 4;
					} else if (directions & 2) {
						// No choice but to go down
						_currentDirection = 2;
					}
				}
			} else {

				// Player is below
				if (directions & 2) {
					_currentDirection = 2;
				} else {
					// Cannot go down, so try the best horizontal distance
					if ((hDist > 0) && (directions & 4)) {
						//Sprite to the left, so try to go left
						_currentDirection = 3;
					} else if ((hDist < 0) && (directions & 8)) {
						//Sprite to the right, so try to go right
						_currentDirection = 4;
					} else if (directions & 4) {
						// Try going left ignoring distance
						_currentDirection = 3;
					} else if (directions & 8) {
						// Try going right ignoring distamce
						_currentDirection = 4;
					} else if (directions & 1) {
						// No choice but to go up
						_currentDirection = 1;
					}
				}
			}
		}
	}
}
