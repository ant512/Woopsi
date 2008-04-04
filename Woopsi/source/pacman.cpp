#include "pacman.h"
#include "superbitmap.h"
#include "button.h"
#include "amigawindow.h"
#include "amigascreen.h"
#include "woopsifuncs.h"
#include "debug.h"

// PacSprite

PacSprite::PacSprite(PacMan* game) {
	_game = game;
	_x = 0;
	_y = 0;
	_width = 5;
	_height = 5;
	_colour = woopsiRGB(0, 0, 0);
}

s16 PacSprite::getX() {
	return _x;
}

s16 PacSprite::getY() {
	return _y;
}

u8 PacSprite::getWidth() {
	return _width;
}

u8 PacSprite::getHeight() {
	return _height;
}

void PacSprite::setColour(u16 colour) {
	_colour = colour;
}

void PacSprite::draw() {
	_game->getSuperBitmap()->drawFilledRect(_x, _y, _width, _height, _colour);
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
			_game->getSuperBitmap()->drawFilledRect(_x, _y, _width, _height, woopsiRGB(31, 31, 31));

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




// PacMan

PacMan::PacMan(AmigaScreen* screen) : EventHandler() {
	_screen = screen;
	_width = 95;
	_height = 110;
	_gameOver = false;

	initGUI();

	_map = new PacMap(this);
	_ghosts = new PacGhosts(this);
	_player = new PacPlayer(this);
}

PacMan::~PacMan() {
	delete _map;
	delete _player;
	delete _ghosts;
}

u16 PacMan::getWidth() {
	return _width;
}

u16 PacMan::getHeight() {
	return _height;
}

PacMap* PacMan::getMap() {
	return _map;
}

bool PacMan::isGameOver() {
	return _gameOver;
}

PacGhosts* PacMan::getGhosts() {
	return _ghosts;
}

PacPlayer* PacMan::getPlayer() {
	return _player;
}

SuperBitmap* PacMan::getSuperBitmap() {
	return _superBitmap;
}

void PacMan::reset() {
	_map->reset();
	_ghosts->reset();
	_player->reset();
	_map->draw();
}

void PacMan::run() {
	if (!_gameOver) {
		_map->drawPills();
		_player->run();
		_ghosts->run();

		_superBitmap->draw();
	}
}

void PacMan::endGame() {
	_gameOver = true;

	_superBitmap->drawFilledRect(0, 0, _width, _height, _superBitmap->getBackColour());
	_superBitmap->drawText(10, 40, _superBitmap->getFont(), "Game Over");
}

bool PacMan::handleEvent(const EventArgs& e) {
	switch (e.type) {
		case EVENT_VBL:
			handleVBL(e);
			return true;
		case EVENT_KEY_PRESS:
			handleKeyPress(e);
			return true;
		case EVENT_RELEASE:
			handleRelease(e);
			return true;
		default:
			return false;
	}
}

void PacMan::handleVBL(const EventArgs& e) {
	if (!_gameOver) {
		run();
	}
}

void PacMan::handleKeyPress(const EventArgs& e) {
	if (_window->isActive()) {
		if (e.gadget->getRefcon() == 1) {
			switch (e.keyCode) {
				case KEY_CODE_UP:
					Debug::printf("PacMan: Up pressed.");
					_player->setBufferedDirection(1);
					break;
				case KEY_CODE_DOWN:
					Debug::printf("PacMan: Down pressed.");
					_player->setBufferedDirection(2);
					break;
				case KEY_CODE_LEFT:
					Debug::printf("PacMan: Left pressed.");
					_player->setBufferedDirection(3);
					break;
				case KEY_CODE_RIGHT:
					Debug::printf("PacMan: Right pressed.");
					_player->setBufferedDirection(4);
					break;
				default:
					break;
			}
		}
	}
}

void PacMan::initGUI() {
	// Create window
	_window = new AmigaWindow(0, 13, 103, 150, "PacMan", Gadget::GADGET_CLOSEABLE | Gadget::GADGET_DRAGGABLE);
	_screen->addGadget(_window);

	_window->setEventHandler(this);
	_window->setRefcon(1);

	Gadget::Rect rect;
	_window->getClientRect(rect);

	_superBitmap = new SuperBitmap(rect.x, rect.y, _width, _height, _width, _height, false);
	_window->addGadget(_superBitmap);
	_superBitmap->setAllowStylusScroll(false);
	_superBitmap->setRefcon(2);

	_resetButton = new Button(rect.x + 18, rect.y + 114, 60, 14, "Reset");
	_window->addGadget(_resetButton);
	_resetButton->setEventHandler(this);
	_resetButton->setRefcon(3);
	
	// Register window for VBL events
	Woopsi::registerForVBL(_window);
}

void PacMan::handleRelease(const EventArgs& e) {
	if (e.gadget->getRefcon() == 3) {
		_player->resetLives();
		_gameOver = false;
		reset();
	}
}



// PacMap

u8 PacMap::_layout[22][19] = {
							{5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5},
							{5,1,1,1,1,1,1,1,1,5,1,1,1,1,1,1,1,1,5},
							{5,1,5,5,1,5,5,5,1,5,1,5,5,5,1,5,5,1,5},
							{5,1,5,5,1,5,5,5,1,5,1,5,5,5,1,5,5,1,5},
							{5,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,5},
							{5,1,5,5,1,5,1,5,5,5,5,5,1,5,1,5,5,1,5},
							{5,1,1,1,1,5,1,1,1,5,1,1,1,5,1,1,1,1,5},
							{5,5,5,5,1,5,5,5,1,5,1,5,5,5,1,5,5,5,5},
							{6,6,6,5,1,5,1,1,1,1,1,1,1,5,1,5,6,6,6},
							{5,5,5,5,1,5,1,5,5,7,5,5,1,5,1,5,5,5,5},
							{0,0,0,0,1,1,1,5,0,0,0,5,1,1,1,0,0,0,0},
							{5,5,5,5,1,5,1,5,5,5,5,5,1,5,1,5,5,5,5},
							{6,6,6,5,1,5,1,1,1,1,1,1,1,5,1,5,6,6,6},
							{5,5,5,5,1,5,1,5,5,5,5,5,1,5,1,5,5,5,5},
							{5,1,1,1,1,1,1,1,1,5,1,1,1,1,1,1,1,1,5},
							{5,1,5,5,1,5,5,5,1,5,1,5,5,5,1,5,5,1,5},
							{5,1,1,5,1,1,1,1,1,1,1,1,1,1,1,5,1,1,5},
							{5,5,1,5,1,5,1,5,5,5,5,5,1,5,1,5,1,5,5},
							{5,1,1,1,1,5,1,1,1,5,1,1,1,5,1,1,1,1,5},
							{5,1,5,5,5,5,5,5,1,5,1,5,5,5,5,5,5,1,5},
							{5,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,5},
							{5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5}
							};

PacMap::PacMap(PacMan* game) {
	_game = game;
	_blockWidth = 5;
	_blockHeight = 5;
	_pillOffset = 1;
	_pillWidth = 2;
	_pillHeight = 2;
	_emptyBlock = 0;
	_pillBlock = 1;
	_eatenPillBlock = 2;
	_powerBlock = 3;
	_eatenPowerBlock = 4;
	_wallBlock = 5;
	_spaceBlock = 6;
	_ghostBlock = 7;
	_remainingPills = 183;
	_height = 22;
	_width = 19;
	_colour = woopsiRGB(0, 0, 0);

	draw();
}		
						
void PacMap::draw() {

	SuperBitmap* superBitmap = _game->getSuperBitmap();

	for (u8 i = 0; i < _height; i++) {
		for (u8 j = 0; j < _width; j++) {

			// Draw gaps
			if (_layout[i][j] != _wallBlock) {
				superBitmap->drawFilledRect((j * _blockWidth), (i * _blockHeight), _blockWidth, _blockHeight, woopsiRGB(31, 31, 31));
			}

			// Draw walls
			if (_layout[i][j] == _wallBlock) {
				superBitmap->drawFilledRect((j * _blockWidth), (i * _blockHeight), _blockWidth, _blockHeight, _colour);
			}
			
			// Draw pills
			if (_layout[i][j] == _pillBlock) {
				superBitmap->drawFilledRect((j * _blockWidth) + _pillOffset, (i * _blockHeight) + _pillOffset, _pillWidth, _pillHeight, _colour);
			}
		}
	}
}

void PacMap::drawPills() {
	SuperBitmap* superBitmap = _game->getSuperBitmap();

	for (u8 i = 0; i < _height; i++) {
		for (u8 j = 0; j < _width; j++) {
			
			// Draw pills
			if (_layout[i][j] == _pillBlock) {
				superBitmap->drawFilledRect((j * _blockWidth) + _pillOffset, (i * _blockHeight) + _pillOffset, _pillWidth, _pillHeight, _colour);
			}
		}
	}
}

void PacMap::clearBlock(u8 x, u8 y) {

	// Get block co-ords with fixed-point decimal
	u16 accurateBlockX = (x << 8) / _blockWidth;
	u16 accurateBlockY = (y << 8) / _blockHeight;

	// Get inaccurate block co-ords
	u16 blockX = (accurateBlockX >> 8);
	u16 blockY = (accurateBlockY >> 8);
	
	if ((blockX << 8 == accurateBlockX) && (blockY << 8 == accurateBlockY)) {
		if (_layout[blockY][blockX] == _pillBlock) {
			_layout[blockY][blockX] = _eatenPillBlock;
			_remainingPills--;
			
			if (_remainingPills == 0) {
				_game->reset();
			}
		}
	}
}

void PacMap::reset() {
	_remainingPills = 183;
	
	for (u8 i = 0; i < _height; i++) {
		for (u8 j = 0; j < _width; j++) {
			if (_layout[i][j] == _eatenPillBlock) {
				_layout[i][j] = _pillBlock;
			}
		}
	}
}

bool PacMap::checkBlock(s16 x, s16 y, u8 isGhost) {

	// Get tile-translated co-ords
	s16 blockX1 = x / _blockWidth;
	s16 blockY1 = y / _blockHeight;
	
	s16 blockX2 = ((x + _blockWidth - 1) / _blockWidth);
	s16 blockY2 = ((y + _blockHeight - 1) / _blockHeight);
	
	// Check for wrap-around
	if ((blockY1 == 10) && (blockY2 == 10)) {
	if (blockX1 < 0) return true;
	if (blockX2 >= _width) return true;
	}
	
	// Check collisions
	if (_layout[blockY1][blockX1] > _powerBlock) return false;
	if (_layout[blockY1][blockX2] > _powerBlock) return false;
	if (_layout[blockY2][blockX1] > _powerBlock) return false;
	if (_layout[blockY2][blockX2] > _powerBlock) return false;

	return true;
}	




// PacPlayer

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

void PacPlayer::resetLives() {
	_lives = 3;
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

void PacPlayer::run() {
	move();
}

void PacPlayer::setBufferedDirection(u8 bufferedDirection) {
	_bufferedDirection = bufferedDirection;
}



// PacGhosts

PacGhosts::PacGhosts(PacMan* game) {
	_game = game;
	_ghostCount = 4;
	_ghostsInHome = _ghostCount;

	// Create ghosts	
	for (u8 i = 0; i < _ghostCount; i++) {
		_ghosts.push_back(new PacGhost(_game));
	}
	
	// Make first ghost intelligent
	_ghosts[0]->setType(1);
	_ghosts[0]->setColour(woopsiRGB(31, 0, 0));
	
	_ghostsInHome = _ghostCount;

	// Draw all ghosts
	for (u8 i = 0; i < _ghosts.size(); i++) {
		_ghosts[i]->draw();
	}
}

PacGhosts::~PacGhosts() {
	// Delete ghosts
	for (u8 i = 0; i < _ghosts.size(); i++) {
		delete _ghosts[i];
	}

	_ghosts.clear();
}

void PacGhosts::reset() {
	_ghostsInHome = _ghostCount;
	for (u8 i = 0; i < _ghosts.size(); i++) {
		_ghosts[i]->reset();
	}
}

void PacGhosts::draw() {
	for (u8 i = 0; i < _ghosts.size(); i++) {
		_ghosts[i]->draw();
	}
}

void PacGhosts::run() {
	
	// Release ghosts from home
	if (_ghostsInHome > 0) {
		if (rand() % 50 == 1) {
			// Choose a ghost to release
			for (u8 i = 0; i < _ghosts.size(); i++) {
				if (!_ghosts[i]->isActive()) {
					_ghosts[i]->setActive(true);
					break;
				}
			}
		}
	}
	
	// Run all ghosts
	for (u8 i = 0; i < _ghosts.size(); i++) {
		if (_game->isGameOver()) return;

		_ghosts[i]->run();
	}
}



// PacGhost

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

bool PacGhost::isActive() {
	return _active;
}

void PacGhost::setType(u8 type) {
	_type = type;
}

void PacGhost::setActive(bool active) {
	_active = active;
}

void PacGhost::run() {
	move();
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
