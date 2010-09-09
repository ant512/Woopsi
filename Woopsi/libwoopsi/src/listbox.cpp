#include "listbox.h"
#include "graphicsport.h"
#include "fontbase.h"
#include "woopsi.h"
#include "woopsifuncs.h"

using namespace WoopsiUI;

ListBox::ListBox(s16 x, s16 y, u16 width, u16 height, GadgetStyle* style) : ScrollingPanel(x, y, width, height, 0, style) {
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

void ListBox::addOption(const WoopsiString& text, const u32 value, const u16 normalTextColour, const u16 normalBackColour, const u16 selectedTextColour, const u16 selectedBackColour) {
	addOption(new ListBoxDataItem(text, value, normalTextColour, normalBackColour, selectedTextColour, selectedBackColour));
}

void ListBox::addOption(const WoopsiString& text, const u32 value) {
	addOption(text, value, getShadowColour(), getBackColour(), getShadowColour(), getHighlightColour());
}

void ListBox::removeOption(const s32 index) {
	_options.removeItem(index);
}

void ListBox::drawContents(GraphicsPort* port) {

	// Draw background
	port->drawFilledRect(0, 0, getWidth(), getHeight(), getBackColour());

	Rect rect;
	port->getClipRect(rect);

	// Calculate clipping values
	s16 clipX = rect.x;
	s16 clipY = rect.y;
	s16 clipHeight = rect.height;

	// Precalc values for option draw loop
	s16 optionHeight = getOptionHeight();
	
	// Ensure that we subtract 1 from topOption to ensure that the option
	// above is drawn if it is partially visible
	s32 topOption = ((clipY - _canvasY) / optionHeight) - 1;
	
	// Ensure that we add 2 to the bottom option to ensure that the option
	// below is draw if it is partially visible - subbing 1 from topOption
	// means we need to add an additional 1 to compensate
	s32 bottomOption = topOption + (clipHeight / optionHeight) + 2;

	// Ensure top options is not negative
	if (topOption < 0) topOption = 0;

	// Ensure bottom option does not exceed number of options
	if (bottomOption >= _options.getItemCount()) bottomOption = _options.getItemCount() - 1;

	// Calculate values for loop
	s32 y = _canvasY + (topOption * optionHeight);
	s32 i = topOption;

	const ListBoxDataItem* item = NULL;

	// Loop through all options drawing each ones
	while (i <= bottomOption) {

		item = (const ListBoxDataItem*)_options.getItem(i);
		
		// Is the option selected?
		if (item->isSelected()) {
			
			// Draw background
			if (item->getSelectedBackColour() != getBackColour()) {
				port->drawFilledRect(0, y, getWidth(), optionHeight, item->getSelectedBackColour());
			}
		
			// Draw text
			if (isEnabled()) {
				port->drawText(_optionPadding, y + _optionPadding, getFont(), item->getText(), 0, item->getText().getLength(), item->getSelectedTextColour());
			} else {
				port->drawText(_optionPadding, y + _optionPadding, getFont(), item->getText(), 0, item->getText().getLength(), getDarkColour());
			}
		} else {
			
			// Draw background
			if (item->getNormalBackColour() != getBackColour()) {
				port->drawFilledRect(clipX, y, getWidth(), optionHeight, item->getNormalBackColour());
			}
			
			// Draw text
			if (isEnabled()) {
				port->drawText(_optionPadding, y + _optionPadding, getFont(), item->getText(), 0, item->getText().getLength(), item->getNormalTextColour());
			} else {
				port->drawText(_optionPadding, y + _optionPadding, getFont(), item->getText(), 0, item->getText().getLength(), getDarkColour());
			}
		}
		
		i++;
		y += optionHeight;
	}
}

void ListBox::drawBorder(GraphicsPort* port) {

	// Stop drawing if the gadget indicates it should not have an outline
	if (isBorderless()) return;

	port->drawBevelledRect(0, 0, getWidth(), getHeight(), getShadowColour(), getShineColour());
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

bool ListBox::isDoubleClick(s16 x, s16 y) {

	if (Gadget::isDoubleClick(x, y)) {

		// Calculate which option was clicked
		s32 selectedIndex = (-_canvasY + (y - getY())) / getOptionHeight();

		// Has the same option been clicked twice?  Ignore double-clicks that
		// occur on different items
		if (selectedIndex == _lastSelectedIndex) {

			// Process click as a double-click
			return true;
		}
	}

	return false;
}

void ListBox::onClick(s16 x, s16 y) {

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

	startDragging(x, y);

	markRectsDamaged();
}

void ListBox::onDoubleClick(s16 x, s16 y) {

	// Calculate which option was clicked
	s32 newSelectedIndex = (-_canvasY + (y - getY())) / getOptionHeight();	

	// Double-click - select the item exclusively
	deselectAllOptions();
	setSelectedIndex(newSelectedIndex);

	_gadgetEventHandlers->raiseActionEvent();
}

const u16 ListBox::getOptionHeight() const {
	return getFont()->getHeight() + (_optionPadding << 1);
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
	if (_canvasY + (_canvasHeight - getHeight()) < 0) scroll(0, -(oldCanvasHeight - (_canvasHeight - getHeight())));
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
	markRectsDamaged();
}

void ListBox::handleListDataSelectionChangedEvent(const ListDataEventArgs& e) {
	markRectsDamaged();
	_gadgetEventHandlers->raiseValueChangeEvent();
}

// Get the preferred dimensions of the gadget
void ListBox::getPreferredDimensions(Rect& rect) const {
	rect.x = _rect.getX();
	rect.y = _rect.getY();

	rect.width = 0;
	rect.height= 0;

	if (!_flags.borderless) {
		rect.width = _borderSize.left + _borderSize.right;
		rect.height = _borderSize.top + _borderSize.bottom;
	}

	s16 maxWidth = 0;
	s16 optionWidth = 0;

	// Locate longest string in options
	for (s32 i = 0; i < _options.getItemCount(); ++i) {
		optionWidth = getFont()->getStringWidth(_options.getItem(i)->getText());

		if (optionWidth > maxWidth) {
			maxWidth = optionWidth;
		}
	}

	rect.width += (_optionPadding << 1) + maxWidth;
	rect.height += getOptionHeight() * 3;
}
