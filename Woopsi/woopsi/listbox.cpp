#include "listbox.h"
#include "graphicsport.h"
#include "fontbase.h"
#include "woopsi.h"

ListBox::ListBox(s16 x, s16 y, u16 width, u16 height, FontBase* font) : ScrollingPanel(x, y, width, height, 0, font) {
	_outline = OUTLINE_IN;
	_flags.draggable = true;
	_flags.doubleClickable = true;
	_optionPadding = 2;
	_allowMultipleSelections = true;
}

ListBox::~ListBox() {
	
	// Delete all option data
	for (s32 i = 0; i < _options.size(); i++) {
		delete _options[i]->text;
		delete _options[i];
	}
	
	_options.clear();
}

void ListBox::addOption(const char* text, const u32 value, const u16 normalTextColour, const u16 normalBackColour, const u16 selectedTextColour, const u16 selectedBackColour) {
	
	// Create new memory for string
	char* newText = new char[strlen(text) + 1];
	
	// Copy text
	strcpy(newText, text);
	
	// Create new option
	ListBoxOption* newOption = new ListBoxOption;
	newOption->text = newText;
	newOption->value = value;
	newOption->normalTextColour = normalTextColour;
	newOption->normalBackColour = normalBackColour;
	newOption->selectedTextColour = selectedTextColour;
	newOption->selectedBackColour = selectedBackColour;
	newOption->selected = false;
	
	// Add to option array
	_options.push_back(newOption);
	
	// Get client area
	Rect clientRect;
	getClientRect(clientRect);

	// Need to adjust canvas size?
	s32 newCanvasHeight = (_options.size() * (_font->getHeight() + (_optionPadding << 1))) - 1;
	if (newCanvasHeight > _canvasHeight) _canvasHeight = newCanvasHeight;
}

void ListBox::addOption(const char* text, const u32 value) {
	addOption(text, value, _shadowColour, _backColour, _shadowColour, _highlightColour);
}

void ListBox::draw(Rect clipRect) {

	GraphicsPort* port = newInternalGraphicsPort(clipRect);
	
	port->drawFilledRect(0, 0, _width, _height, _backColour);
	
	// TODO: Optimise this so that it limits its drawing to the clip rect's dimensios
	// (at present it draws the entire gadget, although 99% of this drawing is clipped out)
	
	// Precalc values for option draw loop
	s16 optionHeight = _font->getHeight() + (_optionPadding << 1);
	s32 topOption = -_canvasY / optionHeight;
	s16 y = _canvasY + (topOption * optionHeight);
	s32 i = topOption;

	// Loop through all options drawing each one
	while ((i < _options.size()) & (y < _height)) {
		
		// Is the option selected?
		if (_options[i]->selected) {
			
			// Draw background
			if (_options[i]->selectedBackColour != _backColour) {
				port->drawFilledRect(0, y, _width, optionHeight, _options[i]->selectedBackColour);
			}
		
			// Draw text
			port->drawText(_optionPadding, y + _optionPadding, _font, _options[i]->text, _options[i]->selectedTextColour);
		} else {
			
			// Draw background
			if (_options[i]->normalBackColour != _backColour) {
				port->drawFilledRect(0, y, _width, optionHeight, _options[i]->normalBackColour);
			}
			
			// Draw text
			port->drawText(_optionPadding, y + _optionPadding, _font, _options[i]->text, _options[i]->normalTextColour);
		}
		
		i++;
		y += optionHeight;
	}
	
	// Draw outline
	port->drawBevelledRect(0, 0, _width, _height);

	delete port;
}

void ListBox::setSelectedIndex(const s32 index) {
	
	// Unset all indexes
	for (s32 i = 0; i < _options.size(); i++) {
		_options[i]->selected = false;
	}
	
	// Select new index
	_options[index]->selected = true;
	
	draw();
	
	raiseValueChangeEvent();
}

const s32 ListBox::getSelectedIndex() const {

	// Get the first selected index
	for (s32 i = 0; i < _options.size(); i++) {
		if (_options[i]->selected) return i;
	}
	
	return -1;
}

const ListBox::ListBoxOption* ListBox::getSelectedOption() const {
	
	// Get the first selected option
	s32 index = getSelectedIndex();
	
	if (index > -1) {
		return _options[index];
	}
	
	return NULL;
}

bool ListBox::click(s16 x, s16 y) {

	// Check for a double-click
	if (_flags.doubleClickable) {

		// Within the allowed time?
		if (woopsiApplication != NULL) {
			if (woopsiApplication->getVBLCount() - _lastClickTime < 10) {

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
			s32 newSelectedIndex = (-_canvasY + (y - getY())) / (_font->getHeight() + (_optionPadding << 1));	
			
			// Are we setting or unsetting?
			if (_options[newSelectedIndex]->selected) {
				
				// Unselecting
				_options[newSelectedIndex]->selected = false;
				draw();
			} else {
			
				// Selecting
				if (_allowMultipleSelections) {
					_options[newSelectedIndex]->selected = true;
					draw();
				} else {
					setSelectedIndex(newSelectedIndex);
				}
			}


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

			// Tell parent that the clicked gadget has changed
			if (_parent != NULL) {
				_parent->setClickedGadget(this);
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
			s32 newSelectedIndex = (-_canvasY + (y - getY())) / (_font->getHeight() + (_optionPadding << 1));	

			// Double-click - select the item exclusively
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

			// Tell parent that the clicked gadget has changed
			if (_parent != NULL) {
				_parent->setClickedGadget(this);
			}

			// Enable dragging
			setDragging(x, y);

			raiseDoubleClickEvent(x, y);

			return true;
		}
	}

	return false;
}
