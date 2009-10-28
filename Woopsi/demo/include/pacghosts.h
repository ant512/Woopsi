#ifndef _PAC_GHOSTS_H_
#define _PAC_GHOSTS_H_

#include "woopsifuncs.h"
#include "woopsiarray.h"
#include "pacsprite.h"

class PacMan;
class PacGhost;

class PacGhosts {

private:
	PacMan* _game;
	WoopsiArray<PacGhost*> _ghosts;
	u8 _ghostCount;
	u8 _ghostsInHome;
	
public:
	PacGhosts(PacMan* game);
	~PacGhosts();

	void reset();
	void run();
	void draw();
};

#endif
