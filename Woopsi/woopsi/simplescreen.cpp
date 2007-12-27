#include "woopsi.h"

SimpleScreen::SimpleScreen(char* title, FontBase* font) : AmigaScreen(title, font) {
}

SimpleScreen::~SimpleScreen() {
}

// Add a new window to the screen
SimpleWindow* SimpleScreen::newWindow(s16 x, s16 y, u16 width, u16 height, char* title, u32 flags) {
	// Create a new window
	SimpleWindow* newWindow = new SimpleWindow(x, y, width, height, title, flags, _font);
	addGadget(newWindow);
	newWindow->focus();

	return newWindow;
}

SuperBitmap* SimpleScreen::newSuperBitmap(s16 x, s16 y, u16 width, u16 height, u32 bitmapWidth, u32 bitmapHeight, bool isDecoration) {
	// Create new superbitmap
	SuperBitmap* newSuperBitmap = new SuperBitmap(x, _titleHeight + y, width, height, bitmapWidth, bitmapHeight, isDecoration, _font);
	addGadget(newSuperBitmap);

	return newSuperBitmap;
}

// TODO: Embed the sizing code into the requester, and allow windows to
// resize themselves
Alert* SimpleScreen::addAlert(char* title, char* text) {

	// Calculate alert size
	s16 alertX;
	s16 alertY;
	u16 alertWidth;
	u16 alertHeight;

	alertWidth = (strlen(text) * _font->getWidth()) + (WINDOW_BORDER_SIZE * 2) + 6;
	alertHeight = (_font->getHeight() * 2) + WINDOW_BORDER_SIZE + WINDOW_TITLE_HEIGHT + 14;

	alertX = 128 - (alertWidth / 2);
	alertY = 96 - (alertHeight / 2);

	// Create new alert requester
	Alert* newAlert = new Alert(alertX, alertY, alertWidth, alertHeight, title, text, _font);

	// Add to gadget list
	addGadget(newAlert);
	newAlert->focus();

	return newAlert;
}
