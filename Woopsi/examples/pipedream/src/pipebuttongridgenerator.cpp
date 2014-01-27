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

	// We keep generating new layouts until we create one that can be solved.
	bool solvable = false;

	do {

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
			_start.setX(1 + (rand() % (_columns - 2)));

			// We want to ensure that the start and end are not exactly opposite
			do {
				_end.setX(1 + (rand() % (_rows - 2)));
			} while (_end.getX() == _start.getX());
		} else {
			_start.setX(0);
			_end.setX(_columns - 1);
			_start.setY(1 + (rand() % (_columns - 2)));

			do {
				_end.setY(1 + (rand() % (_rows - 2)));
			} while (_end.getY() == _start.getY());
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

		// Insert the tiles required to solve the grid
		bool* checked = new bool[_columns * _rows];

		if (startAtTop) {
			solvable = solveGrid(_start.getX(), _start.getY(), _start.getX(), _start.getY() + 1, _end.getX(), _end.getY(), layout, checked);
		} else {
			solvable = solveGrid(_start.getX(), _start.getY(), _start.getX() + 1, _start.getY(), _end.getX(), _end.getY(), layout, checked);
		}

		delete checked;
	} while (!solvable);

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

bool PipeButtonGridGenerator::solveGrid(u8 previousX,
										u8 previousY,
										u8 currentX,
										u8 currentY,
										u8 endX,
										u8 endY,
										u8* grid,
										bool* checked) {

	// TODO: This needs to be rewritten so that, instead of favouring left, then
	// right, then up, then down, it tries to move towards the exit first before
	// falling back on the other directions.  Easiest way to achieve this is to
	// populate a 4 element array with the desired order of directions, then
	// iterate over the array trying each direction in preferred order.  That
	// will make the algorithm always hunt the exit instead of (as is the case
	// here) hug the walls or do other stupid stuff like pointless backtracking.
	//
	// Actually, having played it for a little while, this is sufficient...

	u8 index = currentX + (_columns * currentY);

	if (checked[index]) return false;

	checked[index] = true;

	if (currentX == endX && currentY == endY) {

		// Found the exit!  Path complete
		return true;
	} else if (grid[index] != TILE_TYPE_BLANK) {

		// Ran into a tile that's already used
		return false;
	} else if (currentX > 0 && (solveGrid(currentX, currentY, currentX - 1, currentY, endX, endY, grid, checked))) {

		// Left
		grid[index] = tileToConnectCoordinates(previousX, previousY, currentX, currentY, currentX - 1, currentY);
		return true;
	} else if (currentX < _columns - 1 && solveGrid(currentX, currentY, currentX + 1, currentY, endX, endY, grid, checked)) {

		// Right
		grid[index] = tileToConnectCoordinates(previousX, previousY, currentX, currentY, currentX + 1, currentY);
		return true;
	} else if (currentY > 0 && solveGrid(currentX, currentY, currentX, currentY - 1, endX, endY, grid, checked)) {

		// Up
		grid[index] = tileToConnectCoordinates(previousX, previousY, currentX, currentY, currentX, currentY - 1);
		return true;
	} else if (currentY < _rows - 1 && solveGrid(currentX, currentY, currentX, currentY + 1, endX, endY, grid, checked)) {

		// Down
		grid[index] = tileToConnectCoordinates(previousX, previousY, currentX, currentY, currentX, currentY + 1);
		return true;
	}

	// No way through for these co-ordinates
	return false;
}

PipeButtonGridGenerator::TileType PipeButtonGridGenerator::tileToConnectCoordinates(u8 previousX, u8 previousY, u8 currentX, u8 currentY, u8 nextX, u8 nextY) {
	if (previousX == nextX) return TILE_TYPE_VERTICAL;
	if (previousY == nextY) return TILE_TYPE_HORIZONTAL;

	if (previousY > nextY && previousX < nextX) {
		if (currentX == previousX) {
			return TILE_TYPE_BOTTOM_RIGHT;
		} else {
			return TILE_TYPE_TOP_LEFT;
		}
	}

	if (nextY > previousY && nextX < previousX) {
		if (currentX == nextX) {
			return TILE_TYPE_BOTTOM_RIGHT;
		} else {
			return TILE_TYPE_TOP_LEFT;
		}
	}

	if (previousY < nextY && previousX < nextX) {
		if (currentX == previousX) {
			return TILE_TYPE_TOP_RIGHT;
		} else {
			return TILE_TYPE_BOTTOM_LEFT;
		}
	}

	if (nextY < previousY && nextX < previousX) {
		if (currentX == nextX) {
			return TILE_TYPE_TOP_RIGHT;
		} else {
			return TILE_TYPE_BOTTOM_LEFT;
		}
	}

	if (previousY > nextY && previousX > nextX) {
		if (currentX == previousX) {
			return TILE_TYPE_BOTTOM_LEFT;
		} else {
			return TILE_TYPE_TOP_RIGHT;
		}
	}

	if (nextY > previousY && nextX > previousX) {
		if (currentX == nextX) {
			return TILE_TYPE_BOTTOM_LEFT;
		} else {
			return TILE_TYPE_TOP_RIGHT;
		}
	}

	if (previousY < nextY && previousX > nextX) {
		if (currentX == previousX) {
			return TILE_TYPE_TOP_LEFT;
		} else {
			return TILE_TYPE_BOTTOM_RIGHT;
		}
	}

	if (nextY < previousY && nextX > previousX) {
		if (currentX == nextX) {
			return TILE_TYPE_TOP_LEFT;
		} else {
			return TILE_TYPE_BOTTOM_RIGHT;
		}
	}

	// No connection could be made!
	return TILE_TYPE_BLANK;
}

