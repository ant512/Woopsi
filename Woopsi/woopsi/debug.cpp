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
			//_screen->flipToTopScreen();
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

			_textBox = new MultiLineTextBox(rect.x, rect.y, rect.width - 10, rect.height, "", Gadget::GADGET_DRAGGABLE, 50, _font);
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
			_slider = new SliderVertical(242, 13, 10, 162);
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
						_textBox->setFiresEvents(false);
						_textBox->jump(0, 0 - _slider->getValue());
						_textBox->setFiresEvents(true);
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
						_slider->setFiresEvents(false);
						_slider->setValue(0 - _textBox->getCanvasY());
						_slider->setFiresEvents(true);
						return true;
					}
					break;
				case EVENT_SCROLL:
					if (_slider != NULL) {
						_slider->setFiresEvents(false);
						_slider->setMaximumValue(_textBox->getCanvasHeight());
						_slider->resizeGrip();
						_slider->setValue(0 - _textBox->getCanvasY());
						_slider->setFiresEvents(true);
						return true;
					}
					break;
				case EVENT_VALUE_CHANGE:
					if (_slider != NULL) {
						//_slider->setFiresEvents(false);
						//_slider->setMaximumValue(_textBox->getCanvasHeight());
						//_slider->resizeGrip();
						//_slider->setValue(0 - _textBox->getCanvasY());
						//_slider->setFiresEvents(true);
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
