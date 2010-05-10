// Includes
#include "demo.h"
#include "woopsiheaders.h"
#include "calculator.h"
#include "pong.h"
#include "pacman.h"
#include "bitmapwrapper.h"
#include "bitmap.h"
#include "graphics.h"
#include "rect.h"
#include "gadgetstyle.h"
#include "fonts/newtopaz.h"
#include "zombie.h"
#include "bittest1.h"
#include "bittest2.h"
#include "bittest3.h"
#include "bittest4.h"
#include "bittest5.h"
#include "bittest6.h"
#include "woopsistring.h"

#include "colourpicker.h"

void Demo::startup() {

	Rect rect;

	// Create SuperBitmap test screen
	AmigaScreen* superBitmapScreen = new AmigaScreen(DEMO_VERSION, Gadget::GADGET_DRAGGABLE, AmigaScreen::AMIGA_SCREEN_SHOW_DEPTH | AmigaScreen::AMIGA_SCREEN_SHOW_FLIP);
	woopsiApplication->addGadget(superBitmapScreen);
	superBitmapScreen->setPermeable(true);

	// Create skinned window
	AmigaWindow* superBitmapWindow = new AmigaWindow(10, 10, 100, 100, "window", Gadget::GADGET_DRAGGABLE, AmigaWindow::AMIGA_WINDOW_SHOW_CLOSE | AmigaWindow::AMIGA_WINDOW_SHOW_DEPTH);
	superBitmapScreen->addGadget(superBitmapWindow);

	/** SuperBitmap preparation **/
	// Create bitmap for superbitmap
	Bitmap* superBitmapBitmap = new Bitmap(164, 191);

	// Get a graphics object from the bitmap so that we can modify it
	Graphics* gfx = superBitmapBitmap->newGraphics();

	// Create a zombie bitmap
	Zombie* zombie = new Zombie();

	// Copy the wrapped bitmap to this bitmap
	gfx->drawBitmap(0, 0, 164, 191, zombie, 0, 0);

	// Dim the bitmap
	gfx->dim(0, 0, 164, 191);

	// Draw on the bitmap
	gfx->drawFilledCircle(10, 10, 20, woopsiRGB(31, 31, 31));
	gfx->drawFilledRect(10, 40, 20, 20, woopsiRGB(31, 0, 0));
	gfx->drawFilledEllipse(60, 60, 10, 30, woopsiRGB(10, 31, 0));

	// Clean up
	delete gfx;
	delete zombie;

	// Attach bitmap
	superBitmapWindow->getClientRect(rect);
	SuperBitmap* superBitmap = new SuperBitmap(rect.x, rect.y, rect.width, rect.height, 164, 191, false);
	superBitmap->getGraphics()->drawBitmap(0, 0, 164, 191, superBitmapBitmap, 0, 0);
	superBitmapWindow->addGadget(superBitmap);

	// Create screens
	AmigaScreen* newScreen = new AmigaScreen(DEMO_VERSION, Gadget::GADGET_DRAGGABLE, AmigaScreen::AMIGA_SCREEN_SHOW_DEPTH | AmigaScreen::AMIGA_SCREEN_SHOW_FLIP);
	woopsiApplication->addGadget(newScreen);
	newScreen->setPermeable(true);

	newScreen->addGadget(new WoopsiKeyboard(2, 13, Gadget::GADGET_DRAGGABLE));

	AmigaScreen* newScreen2 = new AmigaScreen("Another screen", Gadget::GADGET_DRAGGABLE, AmigaScreen::AMIGA_SCREEN_SHOW_DEPTH | AmigaScreen::AMIGA_SCREEN_SHOW_FLIP);
	woopsiApplication->addGadget(newScreen2);

	// Add progress bar
	ProgressBar* progressBar = new ProgressBar(20, 20, 100, 20);
	progressBar->setMaximumValue(100);
	progressBar->setValue(50);
	newScreen2->addGadget(progressBar);

	newScreen2->addGadget(new ColourPicker(0, 0, 150, 100, "Colour Picker", 0, Gadget::GADGET_DRAGGABLE));

	Gradient* gradient = new Gradient(0, SCREEN_TITLE_HEIGHT, 256, 192 - SCREEN_TITLE_HEIGHT, woopsiRGB(0, 0, 31), woopsiRGB(31, 0, 0));
	newScreen2->insertGadget(gradient);

	gradient->addContextMenuItem("Context Menu", 0);
	gradient->addContextMenuItem("Test 1", 0);
	gradient->addContextMenuItem("Test 2", 0);
	gradient->addContextMenuItem("Test 3", 0);


	// Add child windows
	AmigaWindow* controlWindow = new AmigaWindow(0, 13, 256, 33, "Controls", Gadget::GADGET_DRAGGABLE, AmigaWindow::AMIGA_WINDOW_SHOW_CLOSE | AmigaWindow::AMIGA_WINDOW_SHOW_DEPTH);
	newScreen->addGadget(controlWindow);

	AmigaWindow* textWindow = new AmigaWindow(0, 46, 256, 146, "Text", Gadget::GADGET_DRAGGABLE, AmigaWindow::AMIGA_WINDOW_SHOW_CLOSE | AmigaWindow::AMIGA_WINDOW_SHOW_DEPTH);
	newScreen->addGadget(textWindow);

	AmigaWindow* textTestWindow = new AmigaWindow(0, 0, 100, 100, "MTest", Gadget::GADGET_DRAGGABLE, AmigaWindow::AMIGA_WINDOW_SHOW_CLOSE | AmigaWindow::AMIGA_WINDOW_SHOW_DEPTH);
	newScreen->addGadget(textTestWindow);

	textTestWindow->getClientRect(rect);
	textTestWindow->addGadget(new MultiLineTextBox(rect.x, rect.y, 80, 80, "This is some\ntest text", Gadget::GADGET_DRAGGABLE, 5));

	// Controls
	controlWindow->getClientRect(rect);

	Button* homeButton = new Button(rect.x, rect.y, 41, 16, "Home");
	homeButton->disable();

	controlWindow->addGadget(homeButton);
	controlWindow->addGadget(new Button(rect.x + 41, rect.y, 49, 16, "Index"));
	controlWindow->addGadget(new Button(rect.x + 90, rect.y, 17, 16, "<"));
	controlWindow->addGadget(new Button(rect.x + 107, rect.y, 17, 16, ">"));
	controlWindow->addGadget(new Button(rect.x + 124, rect.y, 40, 16, "Help"));

	// Text viewer
	textWindow->getClientRect(rect);
	ScrollingTextBox* scrollingBox = new ScrollingTextBox(rect.x + 1, rect.y + 1, 246, 127, "Woopsi\n"
		"------\n\n"
		"This is a demo of a windowing system I'm working on "
		"for the Nintendo DS, written in C++.  The aim of the "
		"project is to create a simple windowing system that "
		"other programmers can use for their applications, which "
		"should decrease the tedious amount of GUI code that "
		"people have to write.\n\n"
		"The system itself is loosely based on the Commodore "
		"Amiga's \"Intuition\" windowing system.  Everything "
		"in the system is treated as a \"gadget\" (in the "
		"same way that Windows treats everything as a sub-class "
		"of a window), everything takes place within a \"screen\" "
		"environment, and the look-and-feel of the system "
		"intentionally resembles the old Amiga system.", Gadget::GADGET_DRAGGABLE, 50);

	//ScrollingTextBox* scrollingBox = new ScrollingTextBox(rect.x + 1, rect.y + 1, 246, 127, "1\n2\n3\n4\n5", Gadget::GADGET_DRAGGABLE, 0);

	scrollingBox->setTextAlignmentHoriz(MultiLineTextBox::TEXT_ALIGNMENT_HORIZ_LEFT);
	//scrollingBox->setTextAlignmentVert(MultiLineTextBox::TEXT_ALIGNMENT_VERT_TOP);
	scrollingBox->showCursor();
	textWindow->addGadget(scrollingBox);

	scrollingBox->addContextMenuItem("Context Menu 2", 0);
	scrollingBox->addContextMenuItem("Test 3", 0);
	scrollingBox->addContextMenuItem("Test 4", 0);
	scrollingBox->addContextMenuItem("Test 5", 0);



	// Add another screen
	AmigaScreen* demoScreen = new AmigaScreen("Demos", Gadget::GADGET_DRAGGABLE, AmigaScreen::AMIGA_SCREEN_SHOW_DEPTH | AmigaScreen::AMIGA_SCREEN_SHOW_FLIP);
	woopsiApplication->addGadget(demoScreen);
	demoScreen->setPermeable(true);
	demoScreen->flipToTopScreen();

	// Add screen background
	//SuperBitmap* demoBackground = demoScreen->newSuperBitmap(0, 0, 256, 179, (u16*)background_Bitmap, 256, 192, true);
	demoScreen->insertGadget(new Gradient(0, SCREEN_TITLE_HEIGHT, 256, 192 - SCREEN_TITLE_HEIGHT, woopsiRGB(31, 0, 0), woopsiRGB(0, 0, 31)));

	// Create calculator app
	_calculator = new Calculator(demoScreen);

	// Create Pong app
	_pong = new Pong(demoScreen);

	// Create Pacman app
	_pacMan = new PacMan(demoScreen);

	// Bitmap button test
	AmigaWindow* buttonWindow = new AmigaWindow(10, 10, 100, 100, "ButtonTest", Gadget::GADGET_DRAGGABLE, AmigaWindow::AMIGA_WINDOW_SHOW_CLOSE);
	demoScreen->addGadget(buttonWindow);

	buttonWindow->getClientRect(rect);
	buttonWindow->addGadget(new BitmapButton(rect.x, rect.y, 92, 28, 0, 0, new BitTest1(), new BitTest4()));
	buttonWindow->addGadget(new BitmapButton(rect.x, rect.y + 28, 92, 28, 0, 0, new BitTest2(), new BitTest5()));
	buttonWindow->addGadget(new BitmapButton(rect.x, rect.y + 56, 92, 27, 0, 0, new BitTest3(), new BitTest6()));

	// Gadget test
	AmigaScreen* gadgetTestScreen = new AmigaScreen("Gadget test", Gadget::GADGET_DRAGGABLE, AmigaScreen::AMIGA_SCREEN_SHOW_DEPTH | AmigaScreen::AMIGA_SCREEN_SHOW_FLIP);
	woopsiApplication->addGadget(gadgetTestScreen);

	AmigaWindow* gadgetTestWindow = new AmigaWindow(0, 13, 256, 179, "Gadget Test", 0, AmigaWindow::AMIGA_WINDOW_SHOW_CLOSE | AmigaWindow::AMIGA_WINDOW_SHOW_DEPTH);
	gadgetTestScreen->addGadget(gadgetTestWindow);

	// Create listbox
	ScrollingListBox* listbox = new ScrollingListBox(6, 40, 100, 100);
	gadgetTestWindow->addGadget(listbox);
	listbox->addOption("ListBox Test", 0, listbox->getShineColour(), listbox->getBackColour(), listbox->getShineColour(), listbox->getHighlightColour());
	listbox->addOption("List Item 1", 1, listbox->getShadowColour(), listbox->getBackColour(), listbox->getShadowColour(), listbox->getHighlightColour());
	listbox->addOption("List Item 4", 4, listbox->getShadowColour(), listbox->getBackColour(), listbox->getShadowColour(), listbox->getHighlightColour());
	listbox->addOption("List Item 12", 12, listbox->getShadowColour(), listbox->getBackColour(), listbox->getShadowColour(), listbox->getHighlightColour());
	listbox->addOption("List Item 5", 5, listbox->getShadowColour(), listbox->getBackColour(), listbox->getShadowColour(), listbox->getHighlightColour());
	listbox->addOption("List Item 2", 2, listbox->getShadowColour(), listbox->getBackColour(), listbox->getShadowColour(), listbox->getHighlightColour());
	listbox->addOption("List Item 7", 7, listbox->getShadowColour(), listbox->getBackColour(), listbox->getShadowColour(), listbox->getHighlightColour());
	listbox->addOption("List Item 8", 8, listbox->getShadowColour(), listbox->getBackColour(), listbox->getShadowColour(), listbox->getHighlightColour());
	listbox->addOption("List Item 6", 6, listbox->getShadowColour(), listbox->getBackColour(), listbox->getShadowColour(), listbox->getHighlightColour());
	listbox->addOption("List Item 3", 3, listbox->getShadowColour(), listbox->getBackColour(), listbox->getShadowColour(), listbox->getHighlightColour());
	listbox->addOption("List Item 9", 9, listbox->getShadowColour(), listbox->getBackColour(), listbox->getShadowColour(), listbox->getHighlightColour());
	listbox->addOption("List Item 10", 10, listbox->getShadowColour(), listbox->getBackColour(), listbox->getShadowColour(), listbox->getHighlightColour());
	listbox->addOption("List Item 11", 11, listbox->getShadowColour(), listbox->getBackColour(), listbox->getShadowColour(), listbox->getHighlightColour());
	listbox->removeOption(9);
	listbox->sort();

	// Create cycle gadget
	CycleButton* cycle = new CycleButton(6, 15, 100, 20);
	cycle->addOption("test 1", 0);
	cycle->addOption("test 2", 0);
	gadgetTestWindow->addGadget(cycle);

	RadioButtonGroup* radioButtons = new RadioButtonGroup(110, 15);
	gadgetTestWindow->addGadget(radioButtons);

	radioButtons->newRadioButton(0, 0, 12, 12);
	radioButtons->newRadioButton(0, 12, 12, 12);
	radioButtons->newRadioButton(0, 24, 12, 12);
	radioButtons->newRadioButton(0, 36, 12, 12);

	Label* rlabel1 = new Label(125, 14, 60, 14, "Radio 1");
	rlabel1->setBorderless(true);
	rlabel1->setTextAlignmentHoriz(Label::TEXT_ALIGNMENT_HORIZ_LEFT);
	gadgetTestWindow->addGadget(rlabel1);

	Label* rlabel2 = new Label(125, 26, 60, 14, "Radio 2");
	rlabel2->setBorderless(true);
	rlabel2->setTextAlignmentHoriz(Label::TEXT_ALIGNMENT_HORIZ_LEFT);
	gadgetTestWindow->addGadget(rlabel2);

	Label* rlabel3 = new Label(125, 38, 60, 14, "Radio 3");
	rlabel3->setBorderless(true);
	rlabel3->setTextAlignmentHoriz(Label::TEXT_ALIGNMENT_HORIZ_LEFT);
	gadgetTestWindow->addGadget(rlabel3);

	Label* rlabel4 = new Label(125, 50, 60, 14, "Radio 4");
	rlabel4->setBorderless(true);
	rlabel4->setTextAlignmentHoriz(Label::TEXT_ALIGNMENT_HORIZ_LEFT);
	gadgetTestWindow->addGadget(rlabel4);

	CheckBox* check1 = new CheckBox(110, 72, 12, 12);
	check1->setState(CheckBox::CHECK_BOX_STATE_ON);
	check1->disable();

	gadgetTestWindow->addGadget(check1);
	gadgetTestWindow->addGadget(new CheckBox(110, 86, 12, 12));
	gadgetTestWindow->addGadget(new CheckBox(110, 100, 12, 12));

	Label* clabel1 = new Label(125, 70, 60, 14, "Check 1");
	clabel1->setBorderless(true);
	clabel1->setTextAlignmentHoriz(Label::TEXT_ALIGNMENT_HORIZ_LEFT);
	gadgetTestWindow->addGadget(clabel1);

	Label* clabel2 = new Label(125, 84, 60, 14, "Check 2");
	clabel2->setBorderless(true);
	clabel2->setTextAlignmentHoriz(Label::TEXT_ALIGNMENT_HORIZ_LEFT);
	gadgetTestWindow->addGadget(clabel2);

	Label* clabel3 = new Label(125, 98, 60, 14, "Check 3");
	clabel3->setBorderless(true);
	clabel3->setTextAlignmentHoriz(Label::TEXT_ALIGNMENT_HORIZ_LEFT);
	gadgetTestWindow->addGadget(clabel3);

	Requester* req = new Requester(10, 10, 140, 100, "Text");
	req->addOption("Req Test 1", 1);
	req->addOption("Req Test 2", 2);
	req->addOption("Req Test 3", 3);
	req->addOption("Req Test 4", 4);
	req->addOption("Req Test 5", 5);
	req->addOption("Req Test 6", 6);
	req->addOption("Req Test 7", 7);
	req->addOption("Req Test 8", 8);
	req->addOption("Req Test 9", 9);
	newScreen2->addGadget(req);

	// Add Welcome notice
	_alert = new Alert(2, 2, 200, 80, "Welcome!", "Welcome to Woopsi!");
	newScreen2->addGadget(_alert);

	enableDrawing();	// Ensure Woopsi can now draw itself
	redraw();			// Draw initial state

	// Make welcome notice modal
	//_alert->goModal();
}

void Demo::shutdown() {
	
	// Clean up
	//delete _calculator;
	//delete _pong;
	//delete _pacMan;

	Woopsi::shutdown();
}
