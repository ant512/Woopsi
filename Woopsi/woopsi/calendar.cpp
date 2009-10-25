#include <stdio.h>
#include <stdlib.h>
#include "calendar.h"
#include "date.h"
#include "button.h"
#include "graphicsport.h"
#include "debug.h"
#include "label.h"

using namespace WoopsiUI;

Calendar::Calendar(s16 x, s16 y, u16 width, u16 height, u8 day, u8 month, u16 year, u32 flags, FontBase* font) : Gadget(x, y, width, height, flags, font) {
	_date = NULL;
	_visibleDate = NULL;

	setOutlineType(OUTLINE_OUT);
	_selectedDayButton = NULL;

	buildGUI();

	setDate(day, month, year);
}

Calendar::~Calendar() {
	delete _date;
}

void Calendar::draw(Rect clipRect) {

	GraphicsPort* port = newInternalGraphicsPort(clipRect);

	port->drawFilledRect(0, 0, _width, _height, _backColour);

	// Draw outline
	port->drawBevelledRect(0, 0, _width, _height);

	delete port;
}

void Calendar::handleReleaseEvent(const GadgetEventArgs& e) {

	if (e.getSource() != NULL) {

		if (e.getSource() == _leftArrow) {

			// Handle left arrow release
			_visibleDate->addMonths(-1);
			populateGUI();

		} else if (e.getSource() == _rightArrow) {

			// Handle right arrow release
			_visibleDate->addMonths(1);
			populateGUI();

		} else if (e.getSource()->getRefcon() > 0) {

			// Handle day button release

			// Calculate the new date
			u8 day = atoi(((Button*)e.getSource())->getText());
			Date* newDate = new Date(day, _visibleDate->getMonth(), _visibleDate->getYear());

			// Prevent changes if new date is same as old
			if (*_date != *newDate) {

				_date->setDate(day, _visibleDate->getMonth(), _visibleDate->getYear());

				// Select the new gadget and deselect the old
				e.getSource()->setOutlineType(OUTLINE_IN);
				if (_selectedDayButton != NULL) {
					_selectedDayButton->setOutlineType(OUTLINE_CLICK_DEPENDENT);
					_selectedDayButton->redraw();
				}

				_selectedDayButton = (Button*)e.getSource();

				// Raise an action event
				raiseActionEvent(0, 0, 0, 0, KEY_CODE_NONE);
			}

			delete newDate;
		}
	}
}

void Calendar::setDate(u8 day, u8 month, u16 year) {
	
	// Initialise date class
	if (_date == NULL) {
		_date = new Date(day, month, year);
		_visibleDate = new Date(day, month, year);
	} else {
		_date->setDate(day, month, year);
		_visibleDate->setDate(day, month, year);
	}

	populateGUI();
}

void Calendar::populateGUI() {

	// Buffer for int-to-string conversions
	char buffer[6];

	// Set month label
	_monthLabel->disableDrawing();
	_monthLabel->setText(_visibleDate->getMonthName());

	// Append year to month name label
	sprintf(buffer, " %d", _visibleDate->getYear());
	_monthLabel->enableDrawing();
	_monthLabel->appendText(buffer);

	u8 buttonIndex = 0;
	Button* button;

	// Reset the selected day button
	_selectedDayButton = NULL;

	// Locate the first button in the gadget array
	for (u8 i = _decorationCount; i < _gadgets.size(); i++) {
		if (_gadgets[i]->getRefcon() > 0) {
			buttonIndex = i;
			break;
		}
	}

	// Get date object for start of this month
	Date* thisMonth = new Date(1, _visibleDate->getMonth(), _visibleDate->getYear());

	// Prepare to update grid of days
	u8 updatedDays = 0;
	u8 maxDays = CALENDAR_ROWS * CALENDAR_COLS;
	u8 startDay = 1;

	if (thisMonth->getWeekDay() != 1) {

		// Get start date of previous month
		Date* previousMonth = new Date(1, _visibleDate->getMonth(), _visibleDate->getYear());
		previousMonth->addMonths(-1);

		// Calculate calendar start date
		startDay = (previousMonth->getMonthDays() - thisMonth->getWeekDay()) + 2;

		// Set buttons
		while (startDay <= previousMonth->getMonthDays()) {
		
			sprintf(buffer, "%d", startDay);

			button = (Button*)_gadgets[buttonIndex];
			button->disable();
			button->setOutlineType(OUTLINE_CLICK_DEPENDENT);
			button->setText(buffer);

			startDay++;
			updatedDays++;
			buttonIndex++;
		}

		// Reset start day
		startDay = 1;

		delete previousMonth;
	}

	// Set all buttons for this month
	while (startDay <= thisMonth->getMonthDays()) {
		sprintf(buffer, "%d", startDay);

		button = (Button*)_gadgets[buttonIndex];
		button->enable();
		button->setOutlineType(OUTLINE_CLICK_DEPENDENT);

		// Select the day if necessary
		if (_visibleDate->getYear() == _date->getYear()) {
			if (_visibleDate->getMonth() == _date->getMonth()) {
				if (_date->getDay() == startDay) {
					button->setOutlineType(OUTLINE_IN);
					_selectedDayButton = button;
				}
			}
		}

		button->setText(buffer);

		startDay++;
		updatedDays++;
		buttonIndex++;
	}

	// Set all buttons for following month
	startDay = 1;
	while (updatedDays < maxDays) {
		sprintf(buffer, "%d", startDay);

		button = (Button*)_gadgets[buttonIndex];
		button->disable();
		button->setOutlineType(OUTLINE_CLICK_DEPENDENT);
		button->setText(buffer);

		startDay++;
		updatedDays++;
		buttonIndex++;
	}

	delete thisMonth;
}

void Calendar::calculateDayButtonWidths(s32 spaceWidth, u8* dayWidths) {

	// Calculate the size of a single button, disregarding remainder
	u8 buttonWidth = spaceWidth / CALENDAR_COLS;

	// Calculate sizes of each column of day buttons - automatcally
	// assigns any remainder in integer division of rect width 1px
	// at a time to columns in first come, first served fashion
	s32 remainderWidth = spaceWidth % CALENDAR_COLS;
	for (u8 i = 0; i < CALENDAR_COLS; ++i) {
		dayWidths[i] = buttonWidth;

		if (remainderWidth > 0) {
			dayWidths[i]++;
			remainderWidth--;
		}
	}
}

void Calendar::buildGUI() {

	Rect rect;
	getClientRect(rect);

	// Define basic button properties
	u8 buttonHeight = rect.height / (CALENDAR_ROWS + 2);
	u16 gridY = (rect.height - (buttonHeight * CALENDAR_ROWS)) + 1;
	Button* button;

	// Get the widths of the day buttons
	u8 buttonWidths[CALENDAR_COLS];
	calculateDayButtonWidths(rect.width, buttonWidths);

	// Add arrows and month label
	_leftArrow = new Button(rect.x, rect.y, buttonWidths[0], buttonHeight, GLYPH_ARROW_LEFT);
	_leftArrow->addGadgetEventHandler(this);
	addGadget(_leftArrow);

	_rightArrow = new Button((rect.width - buttonWidths[CALENDAR_COLS - 1]) + 1, rect.y, buttonWidths[CALENDAR_COLS - 1], buttonHeight, GLYPH_ARROW_RIGHT);
	_rightArrow->addGadgetEventHandler(this);
	addGadget(_rightArrow);

	// Month name
	_monthLabel = new Label(rect.x + buttonWidths[0], rect.y, rect.width - (buttonWidths[0] + buttonWidths[CALENDAR_COLS - 1]), buttonHeight, "");
	_monthLabel->setBorderless(true);
	addGadget(_monthLabel);

	// Add day labels
	Label* label;
	const char* dayInitials = "MTWTFSS";
	s16 labelX = rect.x;

	for (u8 i = 0; i < CALENDAR_COLS; ++i) {
		label = new Label(labelX, rect.y + buttonHeight, buttonWidths[i], buttonHeight, dayInitials[i]);
		label->setBorderless(true);
		addGadget(label);
		
		labelX += buttonWidths[i];
	}

	// Prepare to build grid of days
	u8 allocatedDays = 0;
	u8 maxDays = CALENDAR_ROWS * CALENDAR_COLS;

	// Create all boxes for this month
	u16 buttonX = rect.x;
	while (allocatedDays < maxDays) {
		button = new Button(buttonX, gridY + ((allocatedDays / CALENDAR_COLS) * buttonHeight), buttonWidths[allocatedDays % CALENDAR_COLS], buttonHeight, "");
		button->addGadgetEventHandler(this);
		button->setRefcon(allocatedDays + 1);

		// Calculate x pos of next button
		buttonX += buttonWidths[allocatedDays % CALENDAR_COLS];

		addGadget(button);
		allocatedDays++;

		// Reset x pos of next button if moving to next row
		if (allocatedDays % CALENDAR_COLS == 0) {
			buttonX = rect.x;
		}
	}
}

const u8 Calendar::getDay() const { return _date->getDay(); }

const u8 Calendar::getMonth() const { return _date->getMonth(); }

const u16 Calendar::getYear() const { return _date->getYear(); }

bool Calendar::resize(u16 width, u16 height) {

	// Enforce gadget to stay within parent confines if necessary
	if (_parent != NULL) {
		if (!_parent->isPermeable()) {

			Rect parentRect;
			_parent->getClientRect(parentRect);

			// Check width
			if (_x + width > parentRect.width) {
				width = parentRect.width - _x;
			}

			// Check height
			if (_y + height > parentRect.height) {
				height = parentRect.height - _y;
			}
		}
	}

	if ((_width != width) || (_height != height)) {
	
		// Remember if the gadget is permeable
		bool wasPermeable = _flags.permeable;

		// Remember if gadget was drawing
		bool wasDrawEnabled = _flags.drawingEnabled;

		_flags.permeable = true;

		erase();

		disableDrawing();

		_width = width;
		_height = height;

		// Resize children

		// Get a rect describing the gadget
		Rect rect;
		getClientRect(rect);

		// Define basic button properties
		u8 buttonHeight = rect.height / (CALENDAR_ROWS + 2);
		u16 gridY = (rect.height - (buttonHeight * CALENDAR_ROWS)) + 1;

		// Get the widths of the day buttons
		u8 buttonWidths[CALENDAR_COLS];
		calculateDayButtonWidths(rect.width, buttonWidths);

		// Resize arrows
		_leftArrow->changeDimensions(rect.x, rect.y, buttonWidths[0], buttonHeight);
		_rightArrow->changeDimensions((rect.width - buttonWidths[CALENDAR_COLS - 1]) + 1, rect.y, buttonWidths[CALENDAR_COLS - 1], buttonHeight);

		// Resize month name
		_monthLabel->changeDimensions(rect.x + buttonWidths[0], rect.y, rect.width - (buttonWidths[0] + buttonWidths[CALENDAR_COLS - 1]), buttonHeight);

		// Resize day labels

		// Locate first day label - work on the assumption that this will always
		// be the first gadget after the month label
		s32 gadgetIndex = getGadgetIndex(_monthLabel) + 1;
		s16 labelX = rect.x;
	
		for (u8 i = 0; i < CALENDAR_COLS; ++i) {
			_gadgets[gadgetIndex]->changeDimensions(labelX, rect.y + buttonHeight, buttonWidths[i], buttonHeight);
			gadgetIndex++;

			labelX += buttonWidths[i];
		}

		// Resize day buttons
		u8 allocatedDays = 0;
		u8 maxDays = CALENDAR_ROWS * CALENDAR_COLS;
		s16 buttonX = rect.x;

		// Create all boxes for this month
		while (allocatedDays < maxDays) {
			_gadgets[gadgetIndex]->changeDimensions(buttonX, gridY + ((allocatedDays / CALENDAR_COLS) * buttonHeight), buttonWidths[allocatedDays % CALENDAR_COLS], buttonHeight);

			// Calculate x pos of next button
			buttonX += buttonWidths[allocatedDays % CALENDAR_COLS];

			allocatedDays++;

			// Reset x pos of next button if moving to next row
			if (allocatedDays % CALENDAR_COLS == 0) {
				buttonX = rect.x;
			}

			gadgetIndex++;
		}

		// Reset the permeable value
		_flags.permeable = wasPermeable;

		// Reset drawing value
		_flags.drawingEnabled = wasDrawEnabled;

		redraw();

		raiseResizeEvent(width, height);

		return true;
	}

	return false;
}

// Get the preferred dimensions of the gadget
void Calendar::getPreferredDimensions(Rect& rect) const {
	rect.x = _x;
	rect.y = _y;
	rect.width = (!_flags.borderless << 1) + 160;
	rect.height = (!_flags.borderless << 1) + 106;
}
