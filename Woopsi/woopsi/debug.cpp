#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "debug.h"
#include "textwriter.h"
#include "sysfont.h"
#include "woopsifuncs.h"
#include "amigascreen.h"
#include "amigawindow.h"
#include "woopsi.h"
#include "scrollbarvertical.h"

Woopsi* Debug::_woopsi = NULL;
Debug* Debug::_debug = NULL;

Debug::Debug() {
	_screen = NULL;
	_window = NULL;
	_textBox = NULL;
	_font = NULL;
	_scrollbar = NULL;

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

		createDebug();

		_debug->_textBox->setVisible(false);
		_debug->_textBox->addText(">");
		_debug->_textBox->addText(text);
		_debug->_textBox->setVisible(true);
		_debug->_textBox->addText("\n");
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
	if (_woopsi == NULL)
		_woopsi = woopsiApplication;

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
			_font = new MonoFont(tinyfont_Bitmap, 128, 24, 4, 6, 32768);
		}

		// Add textbox
		if (_textBox == NULL) {
			Gadget::Rect rect;
			_window->getClientRect(rect);

			_textBox = new MultiLineTextBox(rect.x, rect.y, rect.width - 9, rect.height, "", Gadget::GADGET_DRAGGABLE, 50, _font);
			_textBox->setVisible(false);
			_window->addGadget(_textBox);
			_textBox->setTextPositionHoriz(MultiLineTextBox::TEXT_POSITION_HORIZ_LEFT);
			_textBox->setTextPositionVert(MultiLineTextBox::TEXT_POSITION_VERT_TOP);
			_textBox->setEventHandler(this);
			_textBox->addText("Woopsi Version ");
			_textBox->addText(WOOPSI_VERSION);
			_textBox->addText("\n");
			_textBox->addText(WOOPSI_COPYRIGHT);
			_textBox->setVisible(true);
			_textBox->addText("\n");
		}
		
		// Add slider
		if (_scrollbar == NULL) {
			Gadget::Rect rect;
			_textBox->getClientRect(rect);

			_scrollbar = new ScrollbarVertical(243, 13, 9, 162);
			_window->addGadget(_scrollbar);
			_scrollbar->setMinimumValue(0);
			_scrollbar->setMaximumValue(0);
			_scrollbar->setPageSize(rect.height);
			_scrollbar->setEventHandler(this);
			_scrollbar->draw();
		}
	}
}

bool Debug::handleEvent(const EventArgs& e) {
	if (e.gadget != NULL) {
		if (e.gadget == _scrollbar) {
			
			// Slider events
			switch (e.type) {
				case EVENT_VALUE_CHANGE:
					if (_textBox != NULL) {
						_textBox->setRaisesEvents(false);
						_textBox->jump(0, 0 - _scrollbar->getValue());
						_textBox->setRaisesEvents(true);
						return true;
					}
					break;
				default:
					break;
			}
		} else if (e.gadget == _textBox) {

			// Textbox events
			switch (e.type) {
				case EVENT_DRAG:
					if (_scrollbar != NULL) {
						_scrollbar->setRaisesEvents(false);
						_scrollbar->setValue(0 - _textBox->getCanvasY());
						_scrollbar->setRaisesEvents(true);
						return true;
					}
					break;
				case EVENT_SCROLL:
					if (_scrollbar != NULL) {
						_scrollbar->setRaisesEvents(false);
						_scrollbar->setMaximumValue(_textBox->getCanvasHeight());
						_scrollbar->resizeGrip();
						_scrollbar->setValue(0 - _textBox->getCanvasY());
						_scrollbar->setRaisesEvents(true);
						return true;
					}
					break;
				default:
					break;
			}
		}
	}
	
	return false;
}
