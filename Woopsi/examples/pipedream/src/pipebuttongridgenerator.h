#ifndef _PIPE_BUTTON_GRID_GENERATOR_H_
#define _PIPE_BUTTON_GRID_GENERATOR_H_

#include <woopsi.h>
#include <woopsipoint.h>
#include "pipebuttongrid.h"

using namespace WoopsiUI;

class PipeButtonGridGenerator {
public:
	typedef enum {
		TILE_TYPE_BLANK = 0,
		TILE_TYPE_BLOCKING = 1,
		TILE_TYPE_VERTICAL = 2,
		TILE_TYPE_HORIZONTAL = 3,
		TILE_TYPE_TOP_RIGHT = 4,
		TILE_TYPE_BOTTOM_RIGHT = 5,
		TILE_TYPE_BOTTOM_LEFT = 6,
		TILE_TYPE_TOP_LEFT = 7,
	} TileType;

	PipeButtonGridGenerator(u8 rows, u8 columns);
	u8* generateRandomTiles(u8 blockingTileCount);

	inline const WoopsiPoint& getStart() { return _start; };
	inline const WoopsiPoint& getEnd() { return _end; };
	
private:
	WoopsiPoint _start;
	WoopsiPoint _end;
	u8 _rows;
	u8 _columns;

	void fillColumn(u8 column, TileType tile, u8* grid);
	void fillRow(u8 row, TileType tile, u8* grid);
	void placeTileAtRandomCoordinates(TileType tile, u8* grid);
	TileType tileToConnectCoordinates(u8 previousX,
									  u8 previousY,
									  u8 currentX,
									  u8 currentY,
									  u8 nextX,
									  u8 nextY);
	bool solveGrid(u8 previousX,
				   u8 previousY,
				   u8 currentX,
				   u8 currentY,
				   u8 endX,
				   u8 endY,
				   u8* grid,
				   bool* checked);
};

#endif