#include "simplewindow.h"	
#include "screen.h"

SimpleWindow::SimpleWindow(s16 x, s16 y, u16 width, u16 height, char* title, u32 flags, FontBase* font) : AmigaWindow(x, y, width, height, title, flags, font) {
}

SimpleWindow::~SimpleWindow() {
}

Button* SimpleWindow::newButton(s16 x, s16 y, u16 width, u16 height, char* text) {
	// Create a new button
	Button* newButton = new Button(getBorderSize() + x, getTitleHeight() + y, width, height, text, _font);
	addGadget(newButton);

	return newButton;
}

BitmapButton* SimpleWindow::newBitmapButton(s16 x, s16 y, u16 width, u16 height, u16 bitmapX, u16 bitmapY, u16 bitmapWidth, u16 bitmapHeight, const u16* bitmapNormal, const u16* bitmapClicked) {
	// Create a new button
	Rect rect;
	getClientRect(rect);

	BitmapButton* newButton = new BitmapButton(rect.x + x, rect.y + y, width, height, bitmapX, bitmapY, bitmapWidth, bitmapHeight, bitmapNormal, bitmapClicked);
	addGadget(newButton);

	return newButton;
}

Textbox* SimpleWindow::newTextbox(s16 x, s16 y, u16 width, u16 height, char* text) {
	// Create a new textbox
	Textbox* newTextBox = new Textbox(getBorderSize() + x, getTitleHeight() + y, width, height, text, _font);
	addGadget(newTextBox);

	return newTextBox;
}

MultiLineTextBox* SimpleWindow::newMultiLineTextBox(s16 x, s16 y, u16 width, u16 height, u8 maxRows, char* text) {
	// Create a new textbox
	MultiLineTextBox* newTextBox = new MultiLineTextBox(getBorderSize() + x, getTitleHeight() + y, width, height, text, maxRows, _font);
	addGadget(newTextBox);

	return newTextBox;
}

TextViewer* SimpleWindow::newTextViewer(s16 x, s16 y, u16 width, u16 height, char* text) {
	// Create new text viewer
	TextViewer* newTextViewer = new TextViewer(getBorderSize() + x, getTitleHeight() + y, width, height, 0, text, _font);
	addGadget(newTextViewer);

	return newTextViewer;
}

SuperBitmap* SimpleWindow::newSuperBitmap(s16 x, s16 y, u16 width, u16 height, u32 bitmapWidth, u32 bitmapHeight, bool isDecoration) {
	// Create new superbitmap
	SuperBitmap* newSuperBitmap = new SuperBitmap(getBorderSize() + x, getTitleHeight() + y, width, height, bitmapWidth, bitmapHeight, isDecoration, _font);
	addGadget(newSuperBitmap);

	return newSuperBitmap;
}

RadioButtonGroup* SimpleWindow::newRadioButtonGroup(s16 x, s16 y) {
	// Create new radiobutton group
	RadioButtonGroup* newGroup = new RadioButtonGroup(getBorderSize() + x, getTitleHeight() + y, _font);
	addGadget(newGroup);

	return newGroup;
}

CheckBox* SimpleWindow::newCheckBox(s16 x, s16 y, u16 width, u16 height) {
	// Create a new checkbox
	CheckBox* newCheckBox = new CheckBox(getBorderSize() + x, getTitleHeight() + y, width, height, _font);
	addGadget(newCheckBox);

	return newCheckBox;
}
