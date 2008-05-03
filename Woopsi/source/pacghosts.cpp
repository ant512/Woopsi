#include <stdlib.h>
#include "pacghosts.h"
#include "pacman.h"
#include "superbitmap.h"
#include "pacghost.h"

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
