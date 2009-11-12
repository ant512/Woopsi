#include "pacmap.h"
#include "pacman.h"
#include "superbitmap.h"

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
	Graphics* gfx = superBitmap->getGraphics();

	for (u8 i = 0; i < _height; i++) {
		for (u8 j = 0; j < _width; j++) {

			// Draw gaps
			if (_layout[i][j] != _wallBlock) {
				gfx->drawFilledRect((j * _blockWidth), (i * _blockHeight), _blockWidth, _blockHeight, woopsiRGB(31, 31, 31));
			}

			// Draw walls
			if (_layout[i][j] == _wallBlock) {
				gfx->drawFilledRect((j * _blockWidth), (i * _blockHeight), _blockWidth, _blockHeight, _colour);
			}
			
			// Draw pills
			if (_layout[i][j] == _pillBlock) {
				gfx->drawFilledRect((j * _blockWidth) + _pillOffset, (i * _blockHeight) + _pillOffset, _pillWidth, _pillHeight, _colour);
			}
		}
	}
}

void PacMap::drawPills() {
	SuperBitmap* superBitmap = _game->getSuperBitmap();
	Graphics* gfx = superBitmap->getGraphics();

	for (u8 i = 0; i < _height; i++) {
		for (u8 j = 0; j < _width; j++) {
			
			// Draw pills
			if (_layout[i][j] == _pillBlock) {
				gfx->drawFilledRect((j * _blockWidth) + _pillOffset, (i * _blockHeight) + _pillOffset, _pillWidth, _pillHeight, _colour);
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
