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
#include "slidervertical.h"

//AmigaScreen* Debug::_screen = NULL;
//AmigaWindow* Debug::_window = NULL;
//MultiLineTextBox* Debug::_textBox = NULL;
//Font* Debug::_font = NULL;
//SliderVertical* Debug::_slider = NULL;

Woopsi* Debug::_woopsi = NULL;
Debug* Debug::_debug = NULL;

Debug::Debug() {
	_screen = NULL;
	_window = NULL;
	_textBox = NULL;
	_font = NULL;
	_slider = NULL;

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

		_debug->_textBox->addText(">");
		_debug->_textBox->addText(text);
		_debug->_textBox->addText("\n");
		_debug->_textBox->draw();
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
			_textBox->setEventHandler(this);
			_textBox->addText("Woopsi Version ");
			_textBox->addText(WOOPSI_VERSION);
			_textBox->addText("\n");
			_textBox->addText(WOOPSI_COPYRIGHT);
			_textBox->addText("\n");
			_textBox->draw();
		}
		
		// Add slider
		if (_slider == NULL) {
			_slider = new SliderVertical(30, 30, 10, 100);
			_window->addGadget(_slider);
			_slider->setMinimumValue(0);
			_slider->setMaximumValue(0);
			_slider->setPageSize(_textBox->getHeight());
			_slider->setEventHandler(this);
			_slider->draw();
		}
	}
}

bool Debug::handleEvent(const EventArgs& e) {
	if (e.gadget != NULL) {
		if (e.gadget == _slider) {
			
			// Slider events
			switch (e.type) {
				case EVENT_VALUE_CHANGE:
					if (_textBox != NULL) {
						//_textBox->jump(0, _slider->getValue());
						//Debug::printf("jump %d", _slider->getValue());
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
					if (_slider != NULL) {
						_slider->setValue(_textBox->getScrollY());
						_slider->recalculate();
						return true;
					}
					break;
				case EVENT_SCROLL:
					if (_slider != NULL) {
						_slider->setMinimumValue(_textBox->getMinScrollY() - _textBox->getHeight());
						_slider->recalculate();
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
