#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "debug.h"
#include "textwriter.h"
#include "font.h"
#include "sysfont.h"
#include "woopsifuncs.h"
#include "amigascreen.h"
#include "amigawindow.h"
#include "woopsi.h"

AmigaScreen* Debug::_screen = NULL;
AmigaWindow* Debug::_window = NULL;
MultiLineTextBox* Debug::_textBox = NULL;
Font* Debug::_font = NULL;
Woopsi* Debug::_woopsi = NULL;

void Debug::output(char* text) {
	if (DEBUG_ACTIVE) {

		createGUI();

		_textBox->addText(">");
		_textBox->addText(text);
		_textBox->addText("\n");
		_textBox->draw();
	}
}

void Debug::printf(const char *format, ...) {
	char buffer[256];

	va_list args;
	va_start(args, format);
	vsnprintf(buffer, sizeof(buffer), format, args);
	va_end(args);

	output(buffer);
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

void Debug::setWoopsi(Woopsi* woopsi) {
	_woopsi = woopsi;
}

void Debug::createGUI() {

	if (_woopsi != NULL) {

		// Add debug screen
		if (_screen == NULL) {
			_screen = new AmigaScreen("Debug");
			_woopsi->addGadget(_screen);
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
			_font = new Font(tinyfont_Bitmap, 128, 24, 4, 6, 64543);
		}

		// Add textbox
		if (_textBox == NULL) {
			Gadget::Rect rect;
			_window->getClientRect(rect);

			_textBox = new MultiLineTextBox(rect.x, rect.y, rect.width, rect.height, "", Gadget::GADGET_DRAGGABLE, 50, _font);
			_textBox->setAutomaticDrawing(false);
			_window->addGadget(_textBox);
			_textBox->setTextPositionHoriz(MultiLineTextBox::TEXT_POSITION_HORIZ_LEFT);
			_textBox->setTextPositionVert(MultiLineTextBox::TEXT_POSITION_VERT_TOP);
			_textBox->addText("Woopsi Version ");
			_textBox->addText(WOOPSI_VERSION);
			_textBox->addText("\n");
			_textBox->addText(WOOPSI_COPYRIGHT);
			_textBox->addText("\n");
			_textBox->draw();
		}
	}
}
