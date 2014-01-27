#include "pipebuttongridgenerator.h"

PipeButtonGridGenerator::PipeButtonGridGenerator(u8 rows, u8 columns) {
	_rows = rows;
	_columns = columns;
}

void PipeButtonGridGenerator::fillColumn(u8 column, TileType tile, u8* grid) {
	for (u8 i = 0; i < _rows; ++i) {
		grid[column + (i * _columns)] = tile;
	}
}

void PipeButtonGridGenerator::fillRow(u8 row, TileType tile, u8* grid) {
	for (u8 i = 0; i < _columns; ++i) {
		grid[i + (row * _columns)] = tile;
	}
}

void PipeButtonGridGenerator::placeTileAtRandomCoordinates(TileType tile, u8* grid) {

	// We're using a crappy brute-force algorithm.  It'd be more efficient to
	// store an array of all unused co-ordinates and choose one of those at
	// random, then remove that from the array.
	bool placed = false;

	do {
		u8 x = 1 + (rand() % (_columns - 2));
		u8 y = 1 + (rand() % (_rows - 2));
		u8 index = x + (y * _columns);

		if (grid[index] == 0) {
			grid[index] = tile;
			placed = true;
		}
	} while (!placed);
}

u8* PipeButtonGridGenerator::generateRandomTiles(u8 blockingTileCount) {
	u8* layout = new u8[_rows * _columns];

	// Erase the grid to begins
	for (u8 i = 0; i < _rows; ++i) {
		fillRow(i, TILE_TYPE_BLANK, layout);
	}

	// Fill top and bottom with blocking tiles
	fillColumn(0, TILE_TYPE_BLOCKING, layout);
	fillColumn(_columns - 1, TILE_TYPE_BLOCKING, layout);

	// Fill left and right with blocking tiles
	fillRow(0, TILE_TYPE_BLOCKING, layout);
	fillRow(_rows - 1, TILE_TYPE_BLOCKING, layout);

	// Place start and end tiles
	bool startAtTop = rand() % 2;

	if (startAtTop) {
		_start.setY(0);
		_end.setY(_rows - 1);

		if (_columns - 2 > 0) {
			_start.setX(1 + (rand() % (_columns - 2)));
		}

		// We want to ensure that the start and end are not exactly opposite
		do {
			_end.setX(1 + (rand() % (_rows - 2)));
		} while (_end.getX() == _start.getX());
		
		// Ensure that the start isn't blocked off by placing a random tile
		// below it
		layout[_start.getX() + ((_start.getY() + 1) * _columns)] = TILE_TYPE_VERTICAL + (rand() % 6);
	} else {
		_start.setX(0);
		_end.setX(_columns - 1);
		
		if (_rows - 2 > 0) {
			_start.setY(1 + (rand() % (_rows - 2)));
		}

		do {
			_end.setY(1 + (rand() % (_rows - 2)));
		} while (_end.getY() == _start.getY());
		
		// Ensure that the start isn't blocked off by placing a random tile next
		// to it
		layout[_start.getX() + 1 + (_start.getY() * _columns)] = TILE_TYPE_VERTICAL + (rand() % 6);
	}

	layout[_start.getX() + (_start.getY() * _columns)] = startAtTop ? TILE_TYPE_VERTICAL : TILE_TYPE_HORIZONTAL;
	layout[_end.getX() + (_end.getY() * _columns)] = startAtTop ? TILE_TYPE_VERTICAL : TILE_TYPE_HORIZONTAL;

	// We don't want more than 1/3 of the grid to be blocking tiles, as
	// there's a good chance we can't create valid levels with more blocks
	// (the ratio is chosen arbitrarily based on gut feel rather than
	// testing).
	if (blockingTileCount > ((_rows - 2) * (_columns - 2)) / 3) {
		blockingTileCount = ((_rows - 2) * (_columns - 2)) / 3;
	}

	for (u8 i = 0; i < blockingTileCount; ++i) {
		placeTileAtRandomCoordinates(TILE_TYPE_BLOCKING, layout);
	}

	// Populate the empty space with random tiles.
	for (u8 y = 1; y < _rows - 1; ++y) {
		for (u8 x = 1; x < _columns - 1; ++x) {
			if (layout[x + (_columns * y)] == TILE_TYPE_BLANK) {
				layout[x + (_columns * y)] = TILE_TYPE_VERTICAL + (rand() % 6);
			}
		}
	}

	// Shuffle all tiles except the blocking tiles and entry/exit.  We use
	// another crappy algorithm - iterate through each internal block, find one
	// we can swap with, and swap it.
	for (u8 y = 1; y < _rows - 2; ++y) {
		for (u8 x = 1; x < _columns - 2; ++x) {
			TileType src = (TileType)layout[x + (y * _columns)];

			if (src != TILE_TYPE_BLOCKING) {
				u8 destX = 0;
				u8 destY = 0;
				TileType dest = TILE_TYPE_BLOCKING;

				while (dest == TILE_TYPE_BLOCKING) {
					destX = 1 + (rand() % (_columns - 1));
					destY = 1 + (rand() % (_rows - 1));
					dest = (TileType)layout[destX + (destY * _columns)];
				}

				layout[x + (y * _columns)] = layout[destX + (destY * _columns)];
				layout[destX + (destY * _columns)] = src;
			}
		}
	}

	return layout;
}
