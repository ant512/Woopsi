#include "cyclebutton.h"
#include "graphicsport.h"
#include "woopsi.h"

using namespace WoopsiUI;

CycleButton::CycleButton(s16 x, s16 y, u16 width, u16 height, GadgetStyle* style) : Button(x, y, width, height, "", style) {
	_outline = OUTLINE_CLICK_DEPENDENT;

	// Force text to align left (x) and centre (y)  
	_textX = (_padding * 3) + getFont()->getCharWidth(GLYPH_CYCLE) + 2;
	_textY = (_height - getFont()->getHeight()) >> 1;

	_options.addListDataEventHandler(this);
	_options.setAllowMultipleSelections(false);
}

void CycleButton::addOption(const char* text, const u32 value) {
	_options.addItem(new ListDataItem(text, value));

	// Select the option if this is the first option added
	if (_options.getItemCount() == 1) {
		selectOption(0);
	}
}

void CycleButton::removeOption(const s32 index) {
	_options.removeItem(index);
}

void CycleButton::setSelectedIndex(const s32 index) {
	_options.setItemSelected(index, true);
}

void CycleButton::selectOption(const s32 index) {
	_options.setItemSelected(index, true);
}

const s32 CycleButton::getSelectedIndex() const {
	return _options.getSelectedIndex();
}

const ListDataItem* CycleButton::getSelectedOption() const {
	return _options.getSelectedItem();
}

void CycleButton::sort() {
	_options.sort();
}

void CycleButton::removeAllOptions() {
	_options.removeAllItems();
}

void CycleButton::handleListDataChangedEvent(const ListDataEventArgs& e) {
	redraw();
}

void CycleButton::handleListDataSelectionChangedEvent(const ListDataEventArgs& e) {
	redraw();
	_gadgetEventHandlers->raiseValueChangeEvent();
}

void CycleButton::draw(Rect clipRect) {

	GraphicsPort* port = newInternalGraphicsPort(clipRect);

	if (!isEnabled()) {

		// Draw disabled state
		port->drawFilledRect(0, 0, _width, _height, getBackColour());

		// Draw cycle glyph
		port->drawText(_padding, _textY, getFont(), GLYPH_CYCLE, getDarkColour());

		// Draw separator
		port->drawVertLine((_padding << 1) + getFont()->getCharWidth(GLYPH_CYCLE), _padding, _height - (_padding << 1), getShadowColour());
		port->drawVertLine((_padding << 1) + getFont()->getCharWidth(GLYPH_CYCLE) + 1, _padding, _height - (_padding << 1), getShineColour());

		// Only draw text if option is selected
		if (_options.getSelectedItem() != NULL) {
			port->drawText(_textX, _textY, getFont(), _options.getSelectedItem()->getText(), getDarkColour());
		}
	} else if (!isClicked()) {

		// Draw normal state
		port->drawFilledRect(0, 0, _width, _height, getBackColour());

		// Draw cycle glyph
		port->drawText(_padding, _textY, getFont(), GLYPH_CYCLE);

		// Draw separator
		port->drawVertLine((_padding << 1) + getFont()->getCharWidth(GLYPH_CYCLE), _padding, _height - (_padding << 1), getShadowColour());
		port->drawVertLine((_padding << 1) + getFont()->getCharWidth(GLYPH_CYCLE) + 1, _padding, _height - (_padding << 1), getShineColour());

		// Only draw text if option is selected
		if (_options.getSelectedItem() != NULL) {
			port->drawText(_textX, _textY, getFont(), _options.getSelectedItem()->getText());
		}
	} else {

		// Draw clicked state
		port->drawFilledRect(0, 0, _width, _height, getDarkColour());

		// Draw cycle glyph
		port->drawText(_padding, _textY, getFont(), GLYPH_CYCLE, getShineColour());

		// Draw separator
		port->drawVertLine((_padding << 1) + getFont()->getCharWidth(GLYPH_CYCLE), _padding, _height - (_padding << 1), getShineColour());
		port->drawVertLine((_padding << 1) + getFont()->getCharWidth(GLYPH_CYCLE) + 1, _padding, _height - (_padding << 1), getShadowColour());

		// Only draw text if option is selected
		if (_options.getSelectedItem() != NULL) {
			port->drawText(_textX, _textY, getFont(), _options.getSelectedItem()->getText(), getShineColour());
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
			if (_options.getItemCount() > 1) {

				s32 selectedIndex = _options.getSelectedIndex();

				if (selectedIndex < _options.getItemCount() - 1) {

					// Move to next option
					selectOption(selectedIndex + 1);
				} else {

					// Wrap around as there are no more options
					selectOption(0);
				}
			}

			// Release occurred within gadget; raise release
			_gadgetEventHandlers->raiseReleaseEvent(x, y);
		} else {
			// Release occurred outside gadget; raise release
			_gadgetEventHandlers->raiseReleaseOutsideEvent(x, y);
		}

		redraw();

		return true;
	}

	return false;
}

// Get the preferred dimensions of the gadget
void CycleButton::getPreferredDimensions(Rect& rect) const {
	rect.x = _x;
	rect.y = _y;

	s16 maxWidth = 0;
	s16 optionWidth = 0;

	// Locate longest string in options
	for (s32 i = 0; i < _options.getItemCount(); ++i) {
		optionWidth = getFont()->getStringWidth(_options.getItem(i)->getText());

		if (optionWidth > maxWidth) {
			maxWidth = optionWidth;
		}
	}

	rect.width = ((!_flags.borderless + _padding) << 1) + _textX + maxWidth;
	rect.height = ((!_flags.borderless + _padding) << 1) + _textY + getFont()->getHeight();
}
