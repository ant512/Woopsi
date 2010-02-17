#include "cyclebutton.h"
#include "graphicsport.h"
#include "woopsi.h"

using namespace WoopsiUI;

CycleButton::CycleButton(s16 x, s16 y, u16 width, u16 height, GadgetStyle* style) : Button(x, y, width, height, "", style) {
	Rect rect;
	getClientRect(rect);

	// Force text to align left
	u16 glyphWidth = getGlyphFont()->getCharWidth(GLYPH_CYCLE);
	u16 glyphSpace = _borderSize.left - 1;

	// Text x co-ord is width of cycle glyph plus a space plus width
	// of the spacer line (2px) plus a space
	_textX = glyphWidth + 2 + (glyphSpace << 1);

	_options.addListDataEventHandler(this);
	_options.setAllowMultipleSelections(false);
}

void CycleButton::addOption(const WoopsiString& text, const u32 value) {
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

void CycleButton::drawContents(GraphicsPort* port) {

	Rect rect;
	getClientRect(rect);

	u16 glyphWidth = getGlyphFont()->getCharWidth(GLYPH_CYCLE);
	u16 glyphSpace = _borderSize.left - 1;

	s16 glyphY = (rect.height - getGlyphFont()->getHeight()) / 2;

	u16 textColour;
	u16 separatorLeftColour;
	u16 separatorRightColour;

	if (!isEnabled()) {
		textColour = getDarkColour();
		separatorLeftColour = getShadowColour();
		separatorRightColour = getShineColour();
	} else if (!isClicked()) {
		textColour = getShadowColour();
		separatorLeftColour = getShadowColour();
		separatorRightColour = getShineColour();
	} else {
		textColour = getShineColour();
		separatorLeftColour = getShineColour();
		separatorRightColour = getShadowColour();
	}

	// Draw cycle glyph
	port->drawText(0, glyphY, getGlyphFont(), GLYPH_CYCLE, 0, 1, textColour);

	// Draw separator
	port->drawVertLine(glyphSpace + glyphWidth, 0, rect.height, separatorLeftColour);
	port->drawVertLine(glyphSpace + 1 + glyphWidth, 0, rect.height, separatorRightColour);

	// Only draw text if option is selected
	if (_options.getSelectedItem() != NULL) {
		port->drawText(_textX, _textY, getFont(), _options.getSelectedItem()->getText(), 0, _options.getSelectedItem()->getText().getLength(), textColour);
	}
}

void CycleButton::drawBorder(GraphicsPort* port) {

	// Determine the background colour
	u16 colour;
	if (isClicked()) {
		colour = getDarkColour();
	} else {
		colour = getBackColour();
	}

	port->drawFilledRect(0, 0, _width, _height, colour);
	
	drawOutline(port);
}

void CycleButton::drawOutline(GraphicsPort* port) {

	// Stop drawing if the gadget indicates it should not have an outline
	if (isBorderless()) return;
	
	// Work out which colours to use
	u16 col1;
	u16 col2;
	
	if (isClicked()) {
		// Bevelled into the screen
		col1 = getShadowColour();
		col2 = getShineColour();
	} else {
		// Bevelled out of the screen
		col1 = getShineColour();
		col2 = getShadowColour();
	}
	
	port->drawBevelledRect(0, 0, _width, _height, col1, col2);
}

void CycleButton::onRelease(s16 x, s16 y) {

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

	redraw();
}

void CycleButton::onReleaseOutside(s16 x, s16 y) {
	redraw();
}

// Get the preferred dimensions of the gadget
void CycleButton::getPreferredDimensions(Rect& rect) const {
	rect.x = _x;
	rect.y = _y;
	rect.width = 0;
	rect.height = 0;

	s16 maxWidth = 0;
	s16 optionWidth = 0;

	// Locate longest string in options
	for (s32 i = 0; i < _options.getItemCount(); ++i) {
		optionWidth = getFont()->getStringWidth(_options.getItem(i)->getText());

		if (optionWidth > maxWidth) {
			maxWidth = optionWidth;
		}
	}

	if (!_flags.borderless) {
		rect.width = _borderSize.left + _borderSize.right;
		rect.height = _borderSize.top + _borderSize.bottom;
	}

	rect.width += _textX + maxWidth;
	rect.height += _textY + getFont()->getHeight();
}
