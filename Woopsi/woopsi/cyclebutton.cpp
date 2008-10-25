#include "cyclebutton.h"
#include "graphicsport.h"
#include "woopsi.h"

using namespace WoopsiUI;

CycleButton::CycleButton(s16 x, s16 y, u16 width, u16 height, FontBase* font) : Button(x, y, width, height, "", font) {
	_outline = OUTLINE_CLICK_DEPENDENT;
	_selectedIndex = 0;

	// Force text to align left (x) and centre (y)  
	_textX = (_padding * 3) + _font->getCharWidth(GLYPH_CYCLE) + 2;
	_textY = (_height - _font->getHeight()) >> 1;
}

CycleButton::~CycleButton() {

	// Delete all option data
	for (s32 i = 0; i < _options.size(); i++) {
		delete _options[i]->text;
		delete _options[i];
	}

	_options.clear();
}

void CycleButton::addOption(const char* text, const u32 value) {

	// Create new memory for string
	char* newText = new char[strlen(text) + 1];

	// Copy text
	strcpy(newText, text);

	// Create new option
	CycleButtonOption* newOption = new CycleButtonOption;
	newOption->text = newText;
	newOption->value = value;

	// Add to option array
	_options.push_back(newOption);
}

void CycleButton::draw(Rect clipRect) {

	GraphicsPort* port = newInternalGraphicsPort(clipRect);

	if (!isEnabled()) {

		// Draw disabled state
		port->drawFilledRect(0, 0, _width, _height, _backColour);

		// Draw cycle glyph
		port->drawText(_padding, _textY, _font, GLYPH_CYCLE, _darkColour);

		// Draw separator
		port->drawVertLine((_padding << 1) + _font->getCharWidth(GLYPH_CYCLE), _padding, _height - (_padding << 1), _shadowColour);
		port->drawVertLine((_padding << 1) + _font->getCharWidth(GLYPH_CYCLE) + 1, _padding, _height - (_padding << 1), _shineColour);

		// Only draw text if options exist
		if (_options.size() > 0) {
			port->drawText(_textX, _textY, _font, _options[_selectedIndex]->text, _darkColour);
		}
	} else if (!isClicked()) {

		// Draw normal state
		port->drawFilledRect(0, 0, _width, _height, _backColour);

		// Draw cycle glyph
		port->drawText(_padding, _textY, _font, GLYPH_CYCLE);

		// Draw separator
		port->drawVertLine((_padding << 1) + _font->getCharWidth(GLYPH_CYCLE), _padding, _height - (_padding << 1), _shadowColour);
		port->drawVertLine((_padding << 1) + _font->getCharWidth(GLYPH_CYCLE) + 1, _padding, _height - (_padding << 1), _shineColour);

		// Only draw text if options exist
		if (_options.size() > 0) {
			port->drawText(_textX, _textY, _font, _options[_selectedIndex]->text);
		}
	} else {

		// Draw clicked state
		port->drawFilledRect(0, 0, _width, _height, _darkColour);

		// Draw cycle glyph
		port->drawText(_padding, _textY, _font, GLYPH_CYCLE, _shineColour);

		// Draw separator
		port->drawVertLine((_padding << 1) + _font->getCharWidth(GLYPH_CYCLE), _padding, _height - (_padding << 1), _shineColour);
		port->drawVertLine((_padding << 1) + _font->getCharWidth(GLYPH_CYCLE) + 1, _padding, _height - (_padding << 1), _shadowColour);

		// Only draw text if options exist
		if (_options.size() > 0) {
			port->drawText(_textX, _textY, _font, _options[_selectedIndex]->text, _shineColour);
		}
	}

	// Draw outline
	port->drawBevelledRect(0, 0, _width, _height);

	delete port;
}

bool CycleButton::release(s16 x, s16 y) {

	if (_flags.clicked) {
		_flags.clicked = false;
		_flags.dragging = false;

		if (woopsiApplication->getClickedGadget() == this) {
			woopsiApplication->setClickedGadget(NULL);
		}

		// Determine which release event to fire
		if (checkCollision(x, y)) {
			
			// Choose next option
			if (_options.size() > 1) {
				_selectedIndex++;

				// Wrap around
				if (_selectedIndex >= _options.size()) {
					_selectedIndex = 0;
				}

				// Raise "value change" event
				raiseValueChangeEvent();
			}

			// Release occurred within gadget; raise release
			raiseReleaseEvent(x, y);
		} else {
			// Release occurred outside gadget; raise release
			raiseReleaseOutsideEvent(x, y);
		}

		draw();

		return true;
	}

	return false;
}
