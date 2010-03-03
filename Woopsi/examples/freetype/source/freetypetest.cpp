#include <fat.h>
#include "freetypetest.h"

#include "screen.h"
#include "gadget.h"

#include "multilinetextbox.h"
#include "woopsistring.h"

using namespace WoopsiUI;

void FreeTypeTest::startup() {

	// Create screens
	Screen* topScreen = new Screen("Top screen", Gadget::GADGET_BORDERLESS);
	Screen* bottomScreen = new Screen("Bottom screen", Gadget::GADGET_BORDERLESS);
	topScreen->flipToTopScreen();

	woopsiApplication->addGadget(topScreen);
	woopsiApplication->addGadget(bottomScreen);

	// Get available area within screens
	Rect topRect;
	Rect bottomRect;
	topScreen->getClientRect(topRect);
	bottomScreen->getClientRect(bottomRect);

	// Add multiline text box
	MultiLineTextBox* top = new MultiLineTextBox(topRect.x, topRect.y, topRect.width, topRect.height,"", Gadget::GADGET_BORDERLESS,40);
	MultiLineTextBox* bottom = new MultiLineTextBox(bottomRect.x, bottomRect.y, bottomRect.width, bottomRect.height,"", Gadget::GADGET_BORDERLESS,40);
	top->setTextAlignmentHoriz(MultiLineTextBox::TEXT_ALIGNMENT_HORIZ_CENTRE);
	bottom->setTextAlignmentHoriz(MultiLineTextBox::TEXT_ALIGNMENT_HORIZ_CENTRE);
	topScreen->addGadget(top);
	bottomScreen->addGadget(bottom);

	// Add freetype fonts
	_faceManager = new FreeTypeFaceManager();
	_faceCache = new FreeTypeCache();

	// Initialising libfat (necessary for freetype)  
	fatInitDefault();

	const FaceID* topID = _faceManager->addFace("fat:/default.ttf",0);
	FreeTypeFont* topFont = new FreeTypeFont(_faceCache, topID, 14, 14, 0xFC00);

	const FaceID* bottomID = _faceManager->addFace("fat:/default.ttf",0);
	FreeTypeFont* bottomFont = new FreeTypeFont(_faceCache, bottomID, 20, 20, 0x0000);

	top->setFont(topFont);
	bottom->setFont(bottomFont);

	top->setText("講る\nぎこ"); //E8AC9B   E3828B   E3818E   E38193
	bottom->setText("講る\nぎう\nlet's try my new font class with an ASCII string first.let's try my new font class with an ASCII string first.Japanese test\n日let's try my new font class with an ASCII string first.let's try my new font class with an ASCII string first.Japanese test\n今日は世界\n世界\n There are artifacts if you try to drag the multiline tex box (bug 1) besides, it still displays newline (this should be easy to fix...just ask him not to display code point 10 and 13 etc...)");//*/

	// Ensure Woopsi can draw itself
	enableDrawing();

	// Draw GUI
	redraw();
}

void FreeTypeTest::shutdown() {

	// Call base shutdown method
	Woopsi::shutdown();
}
