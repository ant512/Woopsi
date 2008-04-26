#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "debug.h"
#include "sysfont.h"
#include "woopsifuncs.h"
#include "amigascreen.h"
#include "amigawindow.h"
#include "woopsi.h"
#include "scrollingtextbox.h"
#include "monofont.h"

Debug* Debug::_debug = NULL;

Debug::Debug() {
	_screen = NULL;
	_window = NULL;
	_textBox = NULL;
	_font = NULL;

	createGUI();
}

Debug::~Debug() {
	_screen->close();

	delete _font;
}

void Debug::createDebug() {
	if (_debug == NULL) {
		_debug = new Debug();
	}
}

void Debug::output(char* text) {
	if (DEBUG_ACTIVE) {
		if (woopsiApplication != NULL) {
			createDebug();

			_debug->_textBox->disableDrawing();
			_debug->_textBox->addText(">");
			_debug->_textBox->addText(text);
			_debug->_textBox->enableDrawing();
			_debug->_textBox->addText("\n");
		}
	}
}

void Debug::printf(const char *format, ...) {
	char buffer[256];

	va_list args;
	va_start(args, format);
	vsnprintf(buffer, sizeof(buffer), format, args);
	va_end(args);

	_debug->output(buffer);
}


void Debug::busyWait() {
	if (DEBUG_ACTIVE) {
		while (!Pad.Held.B) {
			woopsiWaitVBL();
		}
		while (Pad.Held.B) {
			woopsiWaitVBL();
		}
	}
}

void Debug::createGUI() {

	// Add debug screen
	if (_screen == NULL) {
		_screen = new AmigaScreen("Debug");
		woopsiApplication->addGadget(_screen);
		_screen->draw();
	}

	// Add debug window
	if (_window == NULL) {
		_window = new AmigaWindow(0, 13, 256, 179, "Debug Output", Gadget::GADGET_DRAGGABLE);
		_screen->addGadget(_window);
		_window->draw();
	}

	// Create font
	if (_font == NULL) {
		_font = new MonoFont(tinyfont_Bitmap, 128, 24, 4, 6, 32768);
	}

	// Add textbox
	if (_textBox == NULL) {
		Gadget::Rect rect;
		_window->getClientRect(rect);

		_textBox = new ScrollingTextBox(rect.x, rect.y, rect.width, rect.height, "", Gadget::GADGET_DRAGGABLE, 50, _font);
		_textBox->disableDrawing();
		_window->addGadget(_textBox);
		_textBox->setTextPositionHoriz(MultiLineTextBox::TEXT_POSITION_HORIZ_LEFT);
		_textBox->setTextPositionVert(MultiLineTextBox::TEXT_POSITION_VERT_TOP);
		_textBox->addText("Woopsi Version ");
		_textBox->addText(WOOPSI_VERSION);
		_textBox->addText("\n");
		_textBox->addText(WOOPSI_COPYRIGHT);
		_textBox->enableDrawing();
		_textBox->addText("\n");
	}
}
