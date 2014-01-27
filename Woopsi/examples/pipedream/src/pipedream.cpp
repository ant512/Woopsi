#include <alert.h>
#include <amigascreen.h>
#include <amigawindow.h>
#include <button.h>
#include <tab.h>
#include <textbox.h>

#include "constants.h"
#include "pipedream.h"

static const u8 FLOW_INCREASE = 5;
static const u8 FLOW_TIMEOUT_SLOW = 20;
static const u8 FLOW_TIMEOUT_FAST = 1;

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

	tab = _tabs->newTab("License");
	tab->setRefcon(3);

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
												"Dragging a tile to a non-draggable destination will have no effect.\n\n"
												"Buttons\n\n"
												"Start: Starts a new game.\n"
												"Complete: Speeds up the spark to quickly complete a level.\n\n"
												"Levels\n\n"
												"All levels are generated randomly, but they all have a solution.", 50);
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

	_licenseTextBox = new ScrollingTextBox(0, textBoxY, SCREEN_WIDTH, SCREEN_HEIGHT - textBoxY, "Copyright (c) 2014, Antony Dzeryn\n"
										   "All rights reserved.\n\n"
										   "Redistribution and use in source and binary forms, with or without modification, are "
										   "permitted provided that the following conditions are met:\n\n"
										   "* Redistributions of source code must retain the above copyright notice, this list "
										   "of conditions and the following disclaimer.\n"
										   "* Redistributions in binary form must reproduce the above copyright notice, this list "
										   "of conditions and the following disclaimer in the documentation and/or other materials "
										   "provided with the distribution.\n"
										   "* Neither the names 'Sparky', 'Simian Zombie' nor the names of its contributors may be "
										   "used to endorse or promote products derived from this software without specific prior "
										   "written permission.\n\n"
										   "THIS SOFTWARE IS PROVIDED BY Antony Dzeryn ``AS IS'' AND ANY EXPRESS OR IMPLIED "
										   "WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY "
										   "AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL Antony Dzeryn "
										   "BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL "
										   "DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; "
										   "LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY "
										   "THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING "
										   "NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF "
										   "ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.", 100);
	_licenseTextBox->setTextAlignmentHoriz(MultiLineTextBox::TEXT_ALIGNMENT_HORIZ_LEFT);
	_licenseTextBox->setTextAlignmentVert(MultiLineTextBox::TEXT_ALIGNMENT_VERT_TOP);
	_licenseTextBox->jump(0, 0);
	_licenseTextBox->hideCursor();
	_docsScreen->addGadget(_licenseTextBox);
	_licenseTextBox->shelve();
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

	_flowTimer = new WoopsiTimer(FLOW_TIMEOUT_SLOW, true);
	_flowTimer->setGadgetEventHandler(this);
	window->addGadget(_flowTimer);
	_flowTimer->setRefcon(2);

	_redrawTimer = new WoopsiTimer(1, true);
	_redrawTimer->setGadgetEventHandler(this);
	window->addGadget(_redrawTimer);
	_redrawTimer->setRefcon(3);

	Button* startButton = new Button(_grid->getRelativeX() + _grid->getWidth() + 10, rect.y + rect.height - 22, 70, 20, "Start");
	startButton->setGadgetEventHandler(this);
	startButton->setRefcon(4);
	window->addGadget(startButton);

	Button* completeButton = new Button(_grid->getRelativeX() + _grid->getWidth() + 10, startButton->getRelativeY() - startButton->getHeight() - 2, 70, 20, "Complete");
	completeButton->setGadgetEventHandler(this);
	completeButton->setRefcon(5);
	window->addGadget(completeButton);

	Label* levelLabel = new Label(startButton->getRelativeX(), rect.y + 2, 40, startButton->getHeight(), "Level");
	window->addGadget(levelLabel);

	WoopsiString level;
	level.format("%d", _level);

	_levelTextBox = new TextBox(levelLabel->getRelativeX() + levelLabel->getWidth() + 2, levelLabel->getRelativeY(), 28, levelLabel->getHeight(), level);
	_levelTextBox->hideCursor();
	window->addGadget(_levelTextBox);
}

void PipeDream::setLevel(u8 level) {
	_level = level;

	WoopsiString str;
	str.format("%d", _level);

	_levelTextBox->setText(str);
}

void PipeDream::shutdown() {

	// Call base shutdown method
	Woopsi::shutdown();
}

void PipeDream::handleActionEvent(Gadget& source) {
	switch (source.getRefcon()) {
		case 2:
			if (!_grid->increaseFlow(FLOW_INCREASE)) {

				if (_grid->isComplete()) {

					// Level complete
					setLevel(_level + 1);
					_grid->reset(_level);
					_grid->enable();
					_redrawTimer->reset();
					_redrawTimer->start();
					_flowTimer->setTimeout(FLOW_TIMEOUT_SLOW);
					_flowTimer->reset();
					_flowTimer->start();
				} else {

					// Game over
					_redrawTimer->stop();
					_flowTimer->stop();
					_grid->disable();
					Alert* alert = new Alert((SCREEN_WIDTH - 100) / 2, (SCREEN_HEIGHT - 80) / 2, 100, 80, "Ooops!", "Game Over");

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
			}
			break;
		case 3:
			_grid->redrawActiveButton();
			break;
		case 4:

			// Restart
			setLevel(0);
			_grid->reset(_level);
			_grid->enable();
			_redrawTimer->reset();
			_redrawTimer->start();
			_flowTimer->setTimeout(FLOW_TIMEOUT_SLOW);
			_flowTimer->reset();
			_flowTimer->start();
			break;

		case 5:

			// Done button increases flow speed
			_flowTimer->setTimeout(FLOW_TIMEOUT_FAST);
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
					_licenseTextBox->shelve();
					break;
				case 2:
					_instructionsTextBox->shelve();
					_aboutTextBox->unshelve();
					_licenseTextBox->shelve();
					break;
				case 3:
					_instructionsTextBox->shelve();
					_aboutTextBox->shelve();
					_licenseTextBox->unshelve();
					break;
			}
			break;
	}
}
