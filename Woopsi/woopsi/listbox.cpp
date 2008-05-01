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
	
	resizeCanvas();
}

void ListBox::addOption(const char* text, const u32 value) {
	addOption(text, value, _shadowColour, _backColour, _shadowColour, _highlightColour);
}

void ListBox::removeOption(const s32 index) {

	// Bounds check
	if (index < _options.size()) {

		// Delete the option
		delete _options[index]->text;
		delete _options[index];

		// Erase the option from the list
		_options.erase(index);

		// Shrink the canvas
		resizeCanvas();
	}
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
	setOptionSelected(index, true);
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

void ListBox::selectOption(const s32 index) {
	setOptionSelected(index, true);
}

void ListBox::deselectOption(const s32 index) {
	setOptionSelected(index, false);
}

void ListBox::setOptionSelected(const s32 index, bool selected) {

	// Deselect old options if we're making an option selected and we're not a multiple list
	if (((!_allowMultipleSelections) || (index == -1)) && (selected)) {
		for (s32 i = 0; i < _options.size(); i++) {
			_options[i]->selected = false;
		}
	}

	// Select or deselect the new option
	if ((index > -1) && (index < _options.size())) {
		_options[index]->selected = selected;
	}
	
	draw();
	
	raiseValueChangeEvent();
}

void ListBox::deselectAllOptions() {
	for (s32 i = 0; i < _options.size(); i++) {
		_options[i]->selected = false;
	}

	draw();
	
	raiseValueChangeEvent();
}

void ListBox::selectAllOptions() {
	if (_allowMultipleSelections) {
		for (s32 i = 0; i < _options.size(); i++) {
			_options[i]->selected = true;
		}

		draw();
		
		raiseValueChangeEvent();
	}
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
			s32 newSelectedIndex = (-_canvasY + (y - getY())) / getOptionHeight();	
			
			// Are we setting or unsetting?
			if (_options[newSelectedIndex]->selected) {
				
				// Deselecting
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
			s32 newSelectedIndex = (-_canvasY + (y - getY())) / getOptionHeight();	

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

const u16 ListBox::getOptionHeight() const {
	return _font->getHeight() + (_optionPadding << 1);
}

void ListBox::resizeCanvas() {

	// Get client area
	Rect rect;
	getClientRect(rect);

	// Resize the canvas
	_canvasHeight = (_options.size() * getOptionHeight()) - 1;

	// Ensure canvas is at least as tall as the gadget
	_canvasHeight = _canvasHeight < rect.height ? rect.height : _canvasHeight;
}

void ListBox::sort() {
	quickSort(0, _options.size() - 1);
	draw();
}

void ListBox::quickSort(const s32 start, const s32 end) {
	if (end > start) {

		int left = start;
		int right = end;

		char* pivot = _options[(start + end) >> 1]->text;

		do {
			while ((strcmp(_options[left]->text, pivot) < 0) && (left < end)) left++;
			while ((strcmp(_options[right]->text, pivot) > 0) && (right > start)) right--;

			if (left > right) break;

			swapOptions(left, right);
			left++;
			right--;
		} while (left <= right);

		quickSort(start, right);
		quickSort(left, end);
	}
}

void ListBox::swapOptions(const s32 index1, const s32 index2) {
	ListBoxOption* tmp = _options[index1];
	_options[index1] = _options[index2];
	_options[index2] = tmp;
}
