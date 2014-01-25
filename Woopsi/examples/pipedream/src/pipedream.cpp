#include <alert.h>
#include <amigascreen.h>
#include <amigawindow.h>
#include <button.h>
#include <tab.h>

#include "constants.h"
#include "pipedream.h"

void PipeDream::startup() {
	createGameScreen();
	createDocsScreen();
}

void PipeDream::createDocsScreen() {
	_docsScreen = new AmigaScreen("Documentation", true, true);
	woopsiApplication->addGadget(_docsScreen);
	_docsScreen->flipToTopScreen();

	_tabs = new TabGroup(0, SCREEN_TITLE_HEIGHT, SCREEN_WIDTH, 20);
	_tabs->setRefcon(4);
	_tabs->setGadgetEventHandler(this);

	Tab* tab = _tabs->newTab("Instructions");
	tab->setRefcon(1);

	tab = _tabs->newTab("About");
	tab->setRefcon(2);

	_docsScreen->addGadget(_tabs);

	s16 textBoxY = _tabs->getRelativeY() + _tabs->getHeight();

	_instructionsTextBox = new ScrollingTextBox(0, textBoxY, SCREEN_WIDTH, SCREEN_HEIGHT - textBoxY, "The Game\n\n"
												"This game is similar to the hacking subgame in the original Bioshock.\n\n"
												"The objective is to create a path for the spark from the entrance to the exit of the grid. "
												"The spark travels along the path automatically. "
												"If the spark reaches a dead end and the game is over.\n\n"
												"How It Works\n\n"
												"Tap on a tile with the stylus to reveal it. Tiles contain either path sections, which can be "
												"vertical, horizontal, or angle joints; or grey blocks, which are dead ends. To move a tile, "
												"drag it to a new location and release it. The two tiles will swap places. The destination tile must "
												"also be a draggable tile.\n\n"
												"Non-draggable tiles are:\n\n"
												"- Grey dead ends\n"
												"- Entrance and exit\n"
												"- Path tiles that have already been travelled\n\n"
												"Dragging a tile to a non-draggable destination will have no effect.", 50);
	_instructionsTextBox->setTextAlignmentHoriz(MultiLineTextBox::TEXT_ALIGNMENT_HORIZ_LEFT);
	_instructionsTextBox->setTextAlignmentVert(MultiLineTextBox::TEXT_ALIGNMENT_VERT_TOP);
	_instructionsTextBox->jump(0, 0);
	_instructionsTextBox->hideCursor();
	_docsScreen->addGadget(_instructionsTextBox);

	_aboutTextBox = new ScrollingTextBox(0, textBoxY, SCREEN_WIDTH, SCREEN_HEIGHT - textBoxY, "Sparky Version 1.0\n\n"
										 "Copyright 2014 Antony Dzeryn\n\n"
										 "Written using Woopsi, the DS GUI framework\n\n"
										 "http://woopsi.org\n"
										 "http://simianzombie.com\n\n"
										 "Sourcecode for the game is distributed with Woopsi and BSD licensed. Fork me on GitHub!\n\n"
										 "https://github.com/ant512/Woopsi\n\n"
										 "ant@simianzombie.com", 20);
	_aboutTextBox->setTextAlignmentHoriz(MultiLineTextBox::TEXT_ALIGNMENT_HORIZ_LEFT);
	_aboutTextBox->setTextAlignmentVert(MultiLineTextBox::TEXT_ALIGNMENT_VERT_TOP);
	_aboutTextBox->jump(0, 0);
	_aboutTextBox->hideCursor();
	_docsScreen->addGadget(_aboutTextBox);
	_aboutTextBox->shelve();
}

void PipeDream::createGameScreen() {

	// Create screens
	_gameScreen = new AmigaScreen("Sparky", true, true);
	woopsiApplication->addGadget(_gameScreen);

	// Add window
	AmigaWindow* window = new AmigaWindow(0, 13, 256, 179, "Sparky", false, true);
	_gameScreen->addGadget(window);

	// Get available area within window
	Rect rect;
	window->getClientRect(rect);

	_grid = new PipeButtonGrid(rect.x + 1, rect.y + 1, 8, 8);
	_grid->setRefcon(1);
	_grid->disable();
	window->addGadget(_grid);

	Button* button = new Button(_grid->getRelativeX() + _grid->getWidth() + 10, 20, 50, 20, "Start");
	button->setGadgetEventHandler(this);
	button->setRefcon(4);
	window->addGadget(button);

	_flowTimer = new WoopsiTimer(20, true);
	_flowTimer->setGadgetEventHandler(this);
	window->addGadget(_flowTimer);
	_flowTimer->setRefcon(2);

	_redrawTimer = new WoopsiTimer(1, true);
	_redrawTimer->setGadgetEventHandler(this);
	window->addGadget(_redrawTimer);
	_redrawTimer->setRefcon(3);
}

void PipeDream::shutdown() {

	// Call base shutdown method
	Woopsi::shutdown();
}

void PipeDream::handleActionEvent(Gadget& source) {
	switch (source.getRefcon()) {
		case 2:
			if (!_grid->increaseFlow(5)) {

				// Game over!
				_redrawTimer->stop();
				_flowTimer->stop();
				_grid->disable();
				Alert* alert = new Alert((SCREEN_WIDTH - 100) / 2, (SCREEN_HEIGHT - 80) / 2, 100, 80, "Game Over", "Ooops!");

				// We always want to show the modal alert on the touch screen
				AmigaScreen* screen = NULL;

				if (_gameScreen->getPhysicalScreenNumber() == 0) {
					screen = _gameScreen;
				} else {
					screen = _docsScreen;
				}

				screen->addGadget(alert);
				alert->goModal();
			}
			break;
		case 3:
			_grid->redrawActiveButton();
			break;
		case 4:

			// Restart
			_grid->reset();
			_grid->enable();
			//_redrawTimer->reset();
			_redrawTimer->start();
			//_flowTimer->reset();
			_flowTimer->start();
			break;
	}
}

void PipeDream::handleValueChangeEvent(Gadget& source) {
	switch (source.getRefcon()) {
		case 4:
			switch (_tabs->getSelectedGadget()->getRefcon()) {
				case 1:
					_instructionsTextBox->unshelve();
					_aboutTextBox->shelve();
					break;
				case 2:
					_instructionsTextBox->shelve();
					_aboutTextBox->unshelve();
					break;
			}
			break;
	}
}
