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
	_lastSelectedIndex = -1;

	// Disallow horizontal scrolling
	setAllowsHorizontalScroll(false);
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
	
	// Calculate clipping values adjusted to gadget-space
	s16 clipX = clipRect.x - getX();
	s16 clipY = clipRect.y - getY();
	s16 clipWidth = clipRect.width;
	s16 clipHeight = clipRect.height;

	// Precalc values for option draw loop
	s16 optionHeight = getOptionHeight();
	s32 topOption = (clipY - _canvasY) / optionHeight;
	s32 bottomOption = topOption + (clipHeight * optionHeight);
	s16 y = _canvasY + (topOption * optionHeight) + (!isBorderless());
	s32 i = topOption;

	// Ensure bottom option does not exceed number of options
	if (bottomOption >= _options.getItemCount()) bottomOption = _options.getItemCount() - 1;

	// Draw background
	port->drawFilledRect(clipX, clipY, clipWidth, clipHeight, _backColour);

	const ListDataItem* item = NULL;

	// Loop through all options drawing each one
	while (i <= bottomOption) {

		item = _options.getItem(i);
		
		// Is the option selected?
		if (item->isSelected()) {
			
			// Draw background
			if (item->getSelectedBackColour() != _backColour) {
				port->drawFilledRect(clipX, y, _width, optionHeight, item->getSelectedBackColour());
			}
		
			// Draw text
			if (isEnabled()) {
				port->drawText(_optionPadding, y + _optionPadding, _font, item->getText(), item->getSelectedTextColour());
			} else {
				port->drawText(_optionPadding, y + _optionPadding, _font, item->getText(), _darkColour);
			}
		} else {
			
			// Draw background
			if (item->getNormalBackColour() != _backColour) {
				port->drawFilledRect(clipX, y, _width, optionHeight, item->getNormalBackColour());
			}
			
			// Draw text
			if (isEnabled()) {
				port->drawText(_optionPadding, y + _optionPadding, _font, item->getText(), item->getNormalTextColour());
			} else {
				port->drawText(_optionPadding, y + _optionPadding, _font, item->getText(), _darkColour);
			}
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

const ListDataItem* ListBox::getSelectedOption() const {
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

						// Calculate which option was clicked
						s32 selectedIndex = (-_canvasY + (y - getY())) / getOptionHeight();

						// Has the same option been clicked twice?  Ignore double-clicks that
						// occur on different items
						if (selectedIndex == _lastSelectedIndex) {
			
							// Process click as a double-click
							return doubleClick(x, y);
						}
					}
				}
			}
		}
	}

	if (checkCollision(x, y)) {
		if (isEnabled()) {

			// Calculate which option was clicked
			_lastSelectedIndex = (-_canvasY + (y - getY())) / getOptionHeight();	
			
			const ListDataItem* item = _options.getItem(_lastSelectedIndex);

			// Are we setting or unsetting?
			if (item->isSelected()) {
				
				// Deselecting
				_options.deselectItem(_lastSelectedIndex);
			} else {
			
				// Selecting
				_options.selectItem(_lastSelectedIndex);
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
		}

		return true;
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
			raiseActionEvent(x, y, 0, 0, KEY_CODE_NONE);

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

// Get the preferred dimensions of the gadget
void ListBox::getPreferredDimensions(Rect& rect) const {
	rect.x = _x;
	rect.y = _y;

	rect.width = ((!_flags.borderless) << 1) + getCanvasWidth();
	rect.height = ((!_flags.borderless) << 1) + getOptionHeight() * 3;
}
