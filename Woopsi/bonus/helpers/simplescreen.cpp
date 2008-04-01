#include "simplescreen.h"
#include "simplewindow.h"
#include "superbitmap.h"
#include "alert.h"

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

Alert* SimpleScreen::addAlert(s16 x, s16 y, u16 width, u16 height, char* title, char* text) {

	// Create new alert requester
	Alert* newAlert = new Alert(x, y, width, height, title, text, _font);

	// Add to gadget list
	addGadget(newAlert);
	newAlert->focus();

	return newAlert;
}
