#include "scrollbarpanel.h"

#include "button.h"

using namespace WoopsiUI;

ScrollbarPanel::ScrollbarPanel(s16 x, s16 y, u16 width, u16 height, u32 flags, GadgetStyle* style) : Gadget(x, y, width, height, flags, style) {
	_scrollbarWidth = 10;
	_scrollbarHeight = 10;
	_hasVerticalScrollbar = true;
	_hasHorizontalScrollbar = true;

	_borderSize.top = 0;
	_borderSize.right = 0;
	_borderSize.left = 0;
	_borderSize.bottom = 0;
	_flags.borderless = true;

	_panel = NULL;
	_scrollbarVertical = NULL;
	_scrollbarHorizontal = NULL;

	buildUI();
}

void ScrollbarPanel::buildUI() {

	Rect rect;
	getClientRect(rect);

	u16 panelWidth = rect.width;
	u16 panelHeight = rect.height;
	if (_hasVerticalScrollbar) panelWidth -= _scrollbarWidth;
	if (_hasHorizontalScrollbar) panelHeight -= _scrollbarHeight;

	_panel = new ScrollingPanel(rect.x, rect.y, panelWidth, panelHeight, 0, &_style);
	addGadget(_panel);

	Rect panelRect;
	_panel->getClientRect(panelRect);

	// Adjust scrollbar dimensions based on scrollbar visibility
	u16 verticalScrollHeight = rect.height;
	u16 horizontalScrollWidth = rect.width;

	if (_hasHorizontalScrollbar) verticalScrollHeight -= _scrollbarHeight;
	if (_hasVerticalScrollbar) horizontalScrollWidth -= _scrollbarWidth;

	if (_hasHorizontalScrollbar) {
		_scrollbarHorizontal = new ScrollbarHorizontal(rect.x, rect.height - _scrollbarHeight, horizontalScrollWidth, _scrollbarHeight, &_style);
		_scrollbarHorizontal->setMinimumValue(0);
		_scrollbarHorizontal->setMaximumValue(getCanvasWidth());
		_scrollbarHorizontal->setPageSize(panelRect.width);
		_scrollbarHorizontal->addGadgetEventHandler(this);
		addGadget(_scrollbarHorizontal);
	}

	if (_hasVerticalScrollbar) {
		_scrollbarVertical = new ScrollbarVertical(rect.width - _scrollbarWidth, rect.y, _scrollbarWidth, verticalScrollHeight, &_style);
		_scrollbarVertical->setMinimumValue(0);
		_scrollbarVertical->setMaximumValue(getCanvasHeight());
		_scrollbarVertical->setPageSize(panelRect.height);
		_scrollbarVertical->addGadgetEventHandler(this);
		addGadget(_scrollbarVertical);
	}
}

void ScrollbarPanel::scroll(s32 dx, s32 dy) {
	_panel->scroll(dx, dy);
}

void ScrollbarPanel::jump(s32 x, s32 y) {
	_panel->jump(x, y);
}

void ScrollbarPanel::setAllowsVerticalScroll(bool allow) {
	_panel->setAllowsVerticalScroll(allow);

	if (_hasVerticalScrollbar) {
		if (allow) {
			_scrollbarVertical->enable();
		} else {
			_scrollbarVertical->disable();
		}
	}
}

void ScrollbarPanel::setAllowsHorizontalScroll(bool allow) {
	_panel->setAllowsHorizontalScroll(allow);

	if (_hasHorizontalScrollbar) {
		if (allow) {
			_scrollbarHorizontal->enable();
		} else {
			_scrollbarHorizontal->disable();
		}
	}
}

void ScrollbarPanel::setCanvasWidth(const s32 width) {
	_panel->setCanvasWidth(width);
	_scrollbarHorizontal->setMaximumValue(width);
}

void ScrollbarPanel::setCanvasHeight(const s32 height) {
	_panel->setCanvasHeight(height);
	_scrollbarVertical->setMaximumValue(height);
}

bool ScrollbarPanel::allowsVerticalScroll() const {
	return _panel->allowsVerticalScroll();
}

bool ScrollbarPanel::allowsHorizontalScroll() const {
	return _panel->allowsHorizontalScroll();
}

const s32 ScrollbarPanel::getCanvasX() const {
	return _panel->getCanvasX();
}

const s32 ScrollbarPanel::getCanvasY() const {
	return _panel->getCanvasY();
}

const s32 ScrollbarPanel::getCanvasWidth() const {
	return _panel->getCanvasWidth();
}

const s32 ScrollbarPanel::getCanvasHeight() const {
	return _panel->getCanvasHeight();
}

void ScrollbarPanel::handleScrollEvent(const GadgetEventArgs& e) {

	if (e.getSource() != NULL) {
		if (e.getSource() == _panel) {

			if (_scrollbarVertical != NULL) {
				_scrollbarVertical->setRaisesEvents(false);
				_scrollbarVertical->setValue(0 - _panel->getCanvasY());
				_scrollbarVertical->setRaisesEvents(true);
			} else if (_scrollbarHorizontal != NULL) {
				_scrollbarHorizontal->setRaisesEvents(false);
				_scrollbarHorizontal->setValue(0 - _panel->getCanvasX());
				_scrollbarHorizontal->setRaisesEvents(true);
			}
		}
	}
}

void ScrollbarPanel::handleValueChangeEvent(const GadgetEventArgs& e) {

	if (e.getSource() != NULL) {
		if (e.getSource() == _scrollbarVertical) {

			if (_panel != NULL) {
				_panel->setRaisesEvents(false);
				_panel->jump(_panel->getCanvasX(), 0 - _scrollbarVertical->getValue());
				_panel->setRaisesEvents(true);
			}
		} else if (e.getSource() == _scrollbarHorizontal) {

			if (_panel != NULL) {
				_panel->setRaisesEvents(false);
				_panel->jump(0 - _scrollbarHorizontal->getValue(), _panel->getCanvasY());
				_panel->setRaisesEvents(true);
			}
		}
	}
}

void ScrollbarPanel::drawContents(GraphicsPort* port) {
	port->drawFilledRect(0, 0, _width, _height, getBackColour());
}
