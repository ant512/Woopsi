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

void ListBox::addOption(ListBoxDataItem* option) {
	_options.addItem(option);
}

void ListBox::addOption(const char* text, const u32 value, const u16 normalTextColour, const u16 normalBackColour, const u16 selectedTextColour, const u16 selectedBackColour) {
	addOption(new ListBoxDataItem(text, value, normalTextColour, normalBackColour, selectedTextColour, selectedBackColour));
}

void ListBox::addOption(const char* text, const u32 value) {
	addOption(text, value, _colours.shadow, _colours.back, _colours.shadow, _colours.highlight);
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
	
	// Ensure that we subtract 1 from topOption to ensure that the option
	// above is drawn if it is partially visible
	s32 topOption = ((clipY - _canvasY) / optionHeight) - 1;
	
	// Ensure that we add 2 to the bottom option to ensure that the option
	// below is draw if it is partially visible - subbing 1 from topOption
	// means we need to add an additional 1 to compensate
	s32 bottomOption = topOption + (clipHeight / optionHeight) + 2;
	s32 y = _canvasY + (topOption * optionHeight) + (!isBorderless());

	// Ensure top options is not negative
	if (topOption < 0) topOption = 0;

	// Ensure bottom option does not exceed number of options
	if (bottomOption >= _options.getItemCount()) bottomOption = _options.getItemCount() - 1;

	// Draw background
	port->drawFilledRect(clipX, clipY, clipWidth, clipHeight, _colours.back);

	const ListBoxDataItem* item = NULL;

	// Loop through all options drawing each one
	s32 i = topOption;
	while (i <= bottomOption) {

		item = (const ListBoxDataItem*)_options.getItem(i);
		
		// Is the option selected?
		if (item->isSelected()) {
			
			// Draw background
			if (item->getSelectedBackColour() != _colours.back) {
				port->drawFilledRect(clipX, y, _width, optionHeight, item->getSelectedBackColour());
			}
		
			// Draw text
			if (isEnabled()) {
				port->drawText(_optionPadding, y + _optionPadding, _font, item->getText(), item->getSelectedTextColour());
			} else {
				port->drawText(_optionPadding, y + _optionPadding, _font, item->getText(), _colours.dark);
			}
		} else {
			
			// Draw background
			if (item->getNormalBackColour() != _colours.back) {
				port->drawFilledRect(clipX, y, _width, optionHeight, item->getNormalBackColour());
			}
			
			// Draw text
			if (isEnabled()) {
				port->drawText(_optionPadding, y + _optionPadding, _font, item->getText(), item->getNormalTextColour());
			} else {
				port->drawText(_optionPadding, y + _optionPadding, _font, item->getText(), _colours.dark);
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

const ListBoxDataItem* ListBox::getSelectedOption() const {
	return (const ListBoxDataItem*)_options.getSelectedItem();
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
	if (isDoubleClick(x, y)) {

		// Calculate which option was clicked
		s32 selectedIndex = (-_canvasY + (y - getY())) / getOptionHeight();

		// Has the same option been clicked twice?  Ignore double-clicks that
		// occur on different items
		if (selectedIndex == _lastSelectedIndex) {

			// Process click as a double-click
			return doubleClick(x, y);
		}
	}

	if (checkCollision(x, y)) {
		if (isEnabled()) {

			// Calculate which option was clicked
			_lastSelectedIndex = (-_canvasY + (y - getY())) / getOptionHeight();	
			
			const ListBoxDataItem* item = (const ListBoxDataItem*)_options.getItem(_lastSelectedIndex);

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

	s32 oldCanvasHeight = _canvasHeight;

	// Resize the canvas
	_canvasHeight = (_options.getItemCount() * getOptionHeight());

	// Ensure canvas is at least as tall as the gadget
	_canvasHeight = _canvasHeight < rect.height ? rect.height : _canvasHeight;

	// If resize has left scroll position beyond end of canvas, adjust to compensate
	if (_canvasY + (_canvasHeight - _height) < 0) scroll(0, -(oldCanvasHeight - (_canvasHeight - _height)));
}

void ListBox::sort() {
	_options.sort();
}

void ListBox::removeAllOptions() {
	_options.removeAllItems();
}

void ListBox::handleListDataChangedEvent(const ListDataEventArgs& e) {
	
	// Forget the last selected item as it may have changed
	_lastSelectedIndex = -1;

	resizeCanvas();
	redraw();
}

void ListBox::handleListDataSelectionChangedEvent(const ListDataEventArgs& e) {
	redraw();
	raiseValueChangeEvent();
}

// Get the preferred dimensions of the gadget
void ListBox::getPreferredDimensions(Rect& rect) const {
	rect.x = _x;
	rect.y = _y;

	s16 maxWidth = 0;
	s16 optionWidth = 0;

	// Locate longest string in options
	for (s32 i = 0; i < _options.getItemCount(); ++i) {
		optionWidth = _font->getStringWidth(_options.getItem(i)->getText());

		if (optionWidth > maxWidth) {
			maxWidth = optionWidth;
		}
	}

	rect.width = ((!_flags.borderless) << 1) + (_optionPadding << 1) + maxWidth;
	rect.height = ((!_flags.borderless) << 1) + (getOptionHeight() * 3);
}
