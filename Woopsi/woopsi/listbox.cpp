#include "listbox.h"
#include "graphicsport.h"
#include "fontbase.h"
#include "woopsi.h"
#include "woopsifuncs.h"

using namespace WoopsiUI;

ListBox::ListBox(s16 x, s16 y, u16 width, u16 height, FontBase* font) : ScrollingPanel(x, y, width, height, 0, font) {
	_outline = OUTLINE_IN;
	_flags.draggable = true;
	_flags.doubleClickable = true;
	_optionPadding = 2;
	_options.addListDataEventHandler(this);
}

ListBox::~ListBox() {
	_options.removeListDataEventHandler(this);
}

void ListBox::addOption(const char* text, const u32 value, const u16 normalTextColour, const u16 normalBackColour, const u16 selectedTextColour, const u16 selectedBackColour) {
	_options.addItem(text, value, normalTextColour, normalBackColour, selectedTextColour, selectedBackColour);
}

void ListBox::addOption(const char* text, const u32 value) {
	addOption(text, value, _shadowColour, _backColour, _shadowColour, _highlightColour);
}

void ListBox::removeOption(const s32 index) {
	_options.removeItem(index);
}

void ListBox::draw(Rect clipRect) {

	GraphicsPort* port = newInternalGraphicsPort(clipRect);
	
	port->drawFilledRect(0, 0, _width, _height, _backColour);
	
	// TODO: Optimise this so that it limits its drawing to the clip rect's dimensios
	// (at present it draws the entire gadget, although 99% of this drawing is clipped out)
	
	// Precalc values for option draw loop
	s16 optionHeight = getOptionHeight();
	s32 topOption = -_canvasY / optionHeight;
	s16 y = _canvasY + (topOption * optionHeight);
	s32 i = topOption;

	// Loop through all options drawing each one
	while ((i < _options.getItemCount()) & (y < _height)) {
		
		// Is the option selected?
		if (_options.getItem(i)->selected) {
			
			// Draw background
			if (_options.getItem(i)->selectedBackColour != _backColour) {
				port->drawFilledRect(0, y, _width, optionHeight, _options.getItem(i)->selectedBackColour);
			}
		
			// Draw text
			port->drawText(_optionPadding, y + _optionPadding, _font, _options.getItem(i)->text, _options.getItem(i)->selectedTextColour);
		} else {
			
			// Draw background
			if (_options.getItem(i)->normalBackColour != _backColour) {
				port->drawFilledRect(0, y, _width, optionHeight, _options.getItem(i)->normalBackColour);
			}
			
			// Draw text
			port->drawText(_optionPadding, y + _optionPadding, _font, _options.getItem(i)->text, _options.getItem(i)->normalTextColour);
		}
		
		i++;
		y += optionHeight;
	}
	
	// Draw outline
	port->drawBevelledRect(0, 0, _width, _height);

	delete port;
}

void ListBox::setSelectedIndex(const s32 index) {
	setOptionSelected(index, true);
}

const s32 ListBox::getSelectedIndex() const {
	return _options.getSelectedIndex();
}

const ListData::ListDataItem* ListBox::getSelectedOption() const {
	return _options.getSelectedItem();
}

void ListBox::selectOption(const s32 index) {
	setOptionSelected(index, true);
}

void ListBox::deselectOption(const s32 index) {
	setOptionSelected(index, false);
}

void ListBox::setOptionSelected(const s32 index, bool selected) {
	_options.setItemSelected(index, selected);
}

void ListBox::deselectAllOptions() {
	_options.deselectAllItems();
}

void ListBox::selectAllOptions() {
	_options.selectAllItems();
}

bool ListBox::click(s16 x, s16 y) {

	// Check for a double-click
	if (_flags.doubleClickable) {

		// Within the allowed time?
		if (woopsiApplication != NULL) {
			if (Stylus.DblClick) {

				// Within the allowed region?
				if ((_lastClickX > x - _doubleClickBounds) && (_lastClickX < x + _doubleClickBounds)) {
					if ((_lastClickY > y - _doubleClickBounds) && (_lastClickY < y + _doubleClickBounds)) {

						// Process click as a double-click
						return doubleClick(x, y);
					}
				}
			}
		}
	}

	if (isEnabled()) {
		if (checkCollision(x, y)) {

			// Calculate which option was clicked
			s32 newSelectedIndex = (-_canvasY + (y - getY())) / getOptionHeight();	
			
			// Are we setting or unsetting?
			if (_options.getItem(newSelectedIndex)->selected) {
				
				// Deselecting
				_options.getItem(newSelectedIndex)->selected = false;
			} else {
			
				// Selecting
				if (_options.allowsMultipleSelections()) {
					_options.getItem(newSelectedIndex)->selected = true;
				} else {
					_options.setSelectedIndex(newSelectedIndex);
				}
			}

			redraw();

			// Standard click code follows, sans sub-gadget stuff
			_flags.clicked = true;

			// Record data for double-click
			if (woopsiApplication != NULL) {
				_lastClickTime = woopsiApplication->getVBLCount();
			} else {
				_lastClickTime = 0;
			}

			_lastClickX = x;
			_lastClickY = y;

			// Take focus away from child gadgets
			setFocusedGadget(NULL);

			// Tell Woopsi that the clicked gadget has changed
			if (woopsiApplication != NULL) {
				woopsiApplication->setClickedGadget(this);
			}

			// Enable dragging
			setDragging(x, y);

			raiseClickEvent(x, y);

			return true;
		}
	}

	return false;
}

bool ListBox::doubleClick(s16 x, s16 y) {

	if (isEnabled()) {
		if (checkCollision(x, y)) {

			// Calculate which option was clicked
			s32 newSelectedIndex = (-_canvasY + (y - getY())) / getOptionHeight();	

			// Double-click - select the item exclusively
			deselectAllOptions();
			setSelectedIndex(newSelectedIndex);

			// Standard double-click code follows, sans sub-gadget stuff
			_flags.clicked = true;

			// Record data for double-click
			if (woopsiApplication != NULL) {
				_lastClickTime = woopsiApplication->getVBLCount();
			} else {
				_lastClickTime = 0;
			}

			_lastClickX = x;
			_lastClickY = y;

			// Take focus away from child gadgets
			setFocusedGadget(NULL);

			// Tell Woopsi that the clicked gadget has changed
			if (woopsiApplication != NULL) {
				woopsiApplication->setClickedGadget(this);
			}

			// Enable dragging
			setDragging(x, y);

			raiseDoubleClickEvent(x, y);

			return true;
		}
	}

	return false;
}

const u16 ListBox::getOptionHeight() const {
	return _font->getHeight() + (_optionPadding << 1);
}

void ListBox::resizeCanvas() {

	// Get client area
	Rect rect;
	getClientRect(rect);

	// Resize the canvas
	_canvasHeight = (_options.getItemCount() * getOptionHeight());

	// Ensure canvas is at least as tall as the gadget
	_canvasHeight = _canvasHeight < rect.height ? rect.height : _canvasHeight;
}

void ListBox::sort() {
	_options.sort();
	redraw();
}

void ListBox::removeAllOptions() {
	_options.removeAllItems();
}

void ListBox::handleListDataChangedEvent(const ListDataEventArgs& e) {
	resizeCanvas();
}

void ListBox::handleListDataSelectionChangedEvent(const ListDataEventArgs& e) {
	redraw();
	raiseValueChangeEvent();
}
