#include "pacmap.h"
#include "pacman.h"
#include "superbitmap.h"

u8 PacMap::_layout[MAP_HEIGHT][MAP_WIDTH] = {
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
	_remainingPills = 183;
	_colour = woopsiRGB(0, 0, 0);

	draw();
}

void PacMap::draw() {

	SuperBitmap* superBitmap = _game->getSuperBitmap();
	Graphics* gfx = superBitmap->getGraphics();

	for (u8 i = 0; i < MAP_HEIGHT; i++) {
		for (u8 j = 0; j < MAP_WIDTH; j++) {

			// Draw gaps
			if (_layout[i][j] != BLOCK_WALL) {
				gfx->drawFilledRect((j * BLOCK_WIDTH), (i * BLOCK_HEIGHT), BLOCK_WIDTH, BLOCK_HEIGHT, woopsiRGB(31, 31, 31));
			}

			// Draw walls
			if (_layout[i][j] == BLOCK_WALL) {
				gfx->drawFilledRect((j * BLOCK_WIDTH), (i * BLOCK_HEIGHT), BLOCK_WIDTH, BLOCK_HEIGHT, _colour);
			}

			// Draw pills
			if (_layout[i][j] == BLOCK_PILL) {
				gfx->drawFilledRect((j * BLOCK_WIDTH) + PILL_OFFSET, (i * BLOCK_HEIGHT) + PILL_OFFSET, PILL_WIDTH, PILL_HEIGHT, _colour);
			}
		}
	}
}

void PacMap::drawPills() {
	SuperBitmap* superBitmap = _game->getSuperBitmap();
	Graphics* gfx = superBitmap->getGraphics();

	for (u8 i = 0; i < MAP_HEIGHT; i++) {
		for (u8 j = 0; j < MAP_WIDTH; j++) {

			// Draw pills
			if (_layout[i][j] == BLOCK_PILL) {
				gfx->drawFilledRect((j * BLOCK_WIDTH) + PILL_OFFSET, (i * BLOCK_HEIGHT) + PILL_OFFSET, PILL_WIDTH, PILL_HEIGHT, _colour);
			}
		}
	}
}

void PacMap::clearBlock(u8 x, u8 y) {

	// Get block co-ords with fixed-point decimal
	u16 accurateBlockX = (x << 8) / BLOCK_WIDTH;
	u16 accurateBlockY = (y << 8) / BLOCK_HEIGHT;

	// Get inaccurate block co-ords
	u16 blockX = (accurateBlockX >> 8);
	u16 blockY = (accurateBlockY >> 8);

	if ((blockX << 8 == accurateBlockX) && (blockY << 8 == accurateBlockY)) {
		if (_layout[blockY][blockX] == BLOCK_PILL) {
			_layout[blockY][blockX] = BLOCK_PILL_EATEN;
			_remainingPills--;

			if (_remainingPills == 0) {
				_game->reset();
			}
		}
	}
}

void PacMap::reset() {
	_remainingPills = 183;

	for (u8 i = 0; i < MAP_HEIGHT; i++) {
		for (u8 j = 0; j < MAP_WIDTH; j++) {
			if (_layout[i][j] == BLOCK_PILL_EATEN) {
				_layout[i][j] = BLOCK_PILL;
			}
		}
	}
}

bool PacMap::checkBlock(s16 x, s16 y, u8 isGhost) {

	// Get tile-translated co-ords
	s16 blockX1 = x / BLOCK_WIDTH;
	s16 blockY1 = y / BLOCK_HEIGHT;

	s16 blockX2 = ((x + BLOCK_WIDTH - 1) / BLOCK_WIDTH);
	s16 blockY2 = ((y + BLOCK_HEIGHT - 1) / BLOCK_HEIGHT);

	// Check for wrap-around
	if ((blockY1 == 10) && (blockY2 == 10)) {
		if (blockX1 < 0) return true;
		if (blockX2 >= MAP_WIDTH) return true;
	}

	// Check collisions
	if (_layout[blockY1][blockX1] > BLOCK_POWER) return false;
	if (_layout[blockY1][blockX2] > BLOCK_POWER) return false;
	if (_layout[blockY2][blockX1] > BLOCK_POWER) return false;
	if (_layout[blockY2][blockX2] > BLOCK_POWER) return false;

	return true;
}
