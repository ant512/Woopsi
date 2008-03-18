// Includes
#include <nds.h>
#include "woopsi.h"
#include "calculator.h"
#include "pong.h"
#include "pacman.h"
#include "bitmap/all_gfx.c"
#include "bitmap/all_gfx.h"
#include "woopsifuncs.h"
#include "skin.h"

int main2() {

	initWoopsiGfxMode();

	// Create woopsi
	woopsiApplication = new Woopsi();

	// Create screens
	AmigaScreen* newScreen = new AmigaScreen("Woopsi Demo V0.30");
	woopsiApplication->addGadget(newScreen);

	//new PacMan(newScreen);

	woopsiApplication->draw();

	// Infinite loop to keep the program running
	while (1)
	{
		woopsiApplication->play();
		woopsiWaitVBL();
	}

	delete woopsiApplication;
}

int mainOld() {

	initWoopsiGfxMode();

	// Create woopsi
	woopsiApplication = new Woopsi();

	// Create screens
	AmigaScreen* newScreen = new AmigaScreen("Woopsi Demo V0.28");
	woopsiApplication->addGadget(newScreen);
	newScreen->setPermeable(true);

	AmigaWindow* textWindow = new AmigaWindow(0, 46, 256, 146, "Text", Gadget::GADGET_CLOSEABLE | Gadget::GADGET_DRAGGABLE);
	newScreen->addGadget(textWindow);

	ScrollingPanel* panel1 = new ScrollingPanel(20, 20, 50, 50, Gadget::GADGET_DRAGGABLE);
	//panel1->setMinScrollX(-20);
	//panel1->setMaxScrollX(20);
	//panel1->setMinScrollY(-20);
	//panel1->setMaxScrollY(20);

	ScrollingPanel* panel2 = new ScrollingPanel(10, 10, 30, 30, Gadget::GADGET_DRAGGABLE);
	//panel2->setMinScrollX(-20);
	//panel2->setMaxScrollX(20);
	//panel2->setMinScrollY(-20);
	//panel2->setMaxScrollY(20);


	panel2->addGadget(new Button(5, 5, 20, 20, "Text"));
	panel1->addGadget(panel2);

	textWindow->addGadget(panel1);


	woopsiApplication->draw();

	// Infinite loop to keep the program running
	while (1)
	{
		woopsiApplication->play();

		if ((Pad.Held.Left) && (Pad.Held.Up)) {
			panel1->scroll(-3, -3);
		} else if ((Pad.Held.Left) && (Pad.Held.Down)) {
			panel1->scroll(-3, 3);
		} else if ((Pad.Held.Right) && (Pad.Held.Up)) {
			panel1->scroll(3, -3);
		} else if ((Pad.Held.Right) && (Pad.Held.Down)) {
			panel1->scroll(3, 3);
		} else if (Pad.Held.Left) {
			panel1->scroll(-3, 0);
		} else if (Pad.Held.Right) {
			panel1->scroll(3, 0);
		} else if (Pad.Held.Up) {
			panel1->scroll(0, -3);
		} else if (Pad.Held.Down) {
			panel1->scroll(0, 3);
		}

		woopsiWaitVBL();
	}

	delete woopsiApplication;
}

int main() {
	initWoopsiGfxMode();

	// Create woopsi
	woopsiApplication = new Woopsi();

	// Create screen skin
	ScreenSkin* screenSkin = new ScreenSkin;
	screenSkin->screen.colours.back = woopsiRGB(8, 8, 8);
	screenSkin->screen.colours.shine = woopsiRGB(31, 31, 31);
	screenSkin->screen.colours.highlight = woopsiRGB(12, 17, 23);
	screenSkin->screen.colours.shadow = woopsiRGB(0, 0, 0);
	screenSkin->screen.colours.fill = woopsiRGB(24, 24, 24);
	screenSkin->screen.font.font = Woopsi::getSystemFont();
	screenSkin->screen.font.monochrome = true;
	screenSkin->screen.font.colour = woopsiRGB(0, 0, 0);
	screenSkin->screen.visible = true;
	screenSkin->screen.permeable = true;
	screenSkin->screen.borderless = true;

	screenSkin->titleBar.bitmap.focus = screentitle_Bitmap;
	screenSkin->titleBar.bitmap.blur = screentitle_Bitmap;
	screenSkin->titleBar.bitmap.focusClick = screentitle_Bitmap;
	screenSkin->titleBar.bitmap.blurClick = screentitle_Bitmap;
	screenSkin->titleBar.bitmap.width = 256;
	screenSkin->titleBar.bitmap.height = 13;
	screenSkin->titleBar.visible = true;
	screenSkin->titleBar.borderless = true;
	screenSkin->titleBar.offsetX = 0;
	screenSkin->titleBar.offsetY = 0;

	screenSkin->depthButton.bitmap.focus = screendepthnormal_Bitmap;
	screenSkin->depthButton.bitmap.blur = screendepthnormal_Bitmap;
	screenSkin->depthButton.bitmap.focusClick = screendepthclick_Bitmap;
	screenSkin->depthButton.bitmap.blurClick = screendepthclick_Bitmap;
	screenSkin->depthButton.bitmap.width = 12;
	screenSkin->depthButton.bitmap.height = 12;
	screenSkin->depthButton.visible = true;
	screenSkin->depthButton.borderless = true;
	screenSkin->depthButton.offsetX = 0;
	screenSkin->depthButton.offsetY = 0;

	screenSkin->flipButton.bitmap.focus = screenflipnormal_Bitmap;
	screenSkin->flipButton.bitmap.blur = screenflipnormal_Bitmap;
	screenSkin->flipButton.bitmap.focusClick = screenflipclick_Bitmap;
	screenSkin->flipButton.bitmap.blurClick = screenflipclick_Bitmap;
	screenSkin->flipButton.bitmap.width = 12;
	screenSkin->flipButton.bitmap.height = 12;
	screenSkin->flipButton.visible = true;
	screenSkin->flipButton.borderless = true;
	screenSkin->flipButton.offsetX = 0;
	screenSkin->flipButton.offsetY = 0;

	// Create window skin
	WindowSkin* windowSkin = new WindowSkin;
	windowSkin->window.colours.back = woopsiRGB(8, 8, 8);
	windowSkin->window.colours.shine = woopsiRGB(31, 31, 31);
	windowSkin->window.colours.highlight = woopsiRGB(12, 17, 23);
	windowSkin->window.colours.shadow = woopsiRGB(0, 0, 0);
	windowSkin->window.colours.fill = woopsiRGB(24, 24, 24);
	windowSkin->window.font.font = Woopsi::getSystemFont();
	windowSkin->window.font.monochrome = true;
	windowSkin->window.font.colour = woopsiRGB(0, 0, 0);
	windowSkin->window.visible = true;
	windowSkin->window.permeable = false;
	windowSkin->window.borderless = false;

	windowSkin->closeButton.bitmap.focus = winclosenormal_Bitmap;
	windowSkin->closeButton.bitmap.blur = winclosenormal_Bitmap;
	windowSkin->closeButton.bitmap.focusClick = wincloseclick_Bitmap;
	windowSkin->closeButton.bitmap.blurClick = wincloseclick_Bitmap;
	windowSkin->closeButton.bitmap.width = 9;
	windowSkin->closeButton.bitmap.height = 9;
	windowSkin->closeButton.visible = true;
	windowSkin->closeButton.borderless = true;
	windowSkin->closeButton.offsetX = 4;
	windowSkin->closeButton.offsetY = 0;

	windowSkin->depthButton.bitmap.focus = windepthnormal_Bitmap;
	windowSkin->depthButton.bitmap.blur = windepthnormal_Bitmap;
	windowSkin->depthButton.bitmap.focusClick = windepthclick_Bitmap;
	windowSkin->depthButton.bitmap.blurClick = windepthclick_Bitmap;
	windowSkin->depthButton.bitmap.width = 9;
	windowSkin->depthButton.bitmap.height = 9;
	windowSkin->depthButton.visible = true;
	windowSkin->depthButton.borderless = true;
	windowSkin->depthButton.offsetX = 4;
	windowSkin->depthButton.offsetY = 0;

	windowSkin->topLeftBorder.bitmap.focus = wintitleleft_Bitmap;
	windowSkin->topLeftBorder.bitmap.blur = wintitleleft_Bitmap;
	windowSkin->topLeftBorder.bitmap.focusClick = wintitleleft_Bitmap;
	windowSkin->topLeftBorder.bitmap.blurClick = wintitleleft_Bitmap;
	windowSkin->topLeftBorder.bitmap.width = 8;
	windowSkin->topLeftBorder.bitmap.height = 13;

	windowSkin->topCentreBorder.bitmap.focus = wintitlecentre_Bitmap;
	windowSkin->topCentreBorder.bitmap.blur = wintitlecentre_Bitmap;
	windowSkin->topCentreBorder.bitmap.focusClick = wintitlecentre_Bitmap;
	windowSkin->topCentreBorder.bitmap.blurClick = wintitlecentre_Bitmap;
	windowSkin->topCentreBorder.bitmap.width = 256;
	windowSkin->topCentreBorder.bitmap.height = 13;

	windowSkin->topRightBorder.bitmap.focus = wintitleright_Bitmap;
	windowSkin->topRightBorder.bitmap.blur = wintitleright_Bitmap;
	windowSkin->topRightBorder.bitmap.focusClick = wintitleright_Bitmap;
	windowSkin->topRightBorder.bitmap.blurClick = wintitleright_Bitmap;
	windowSkin->topRightBorder.bitmap.width = 8;
	windowSkin->topRightBorder.bitmap.height = 13;

	windowSkin->leftBorder.bitmap.focus = winleft_Bitmap;
	windowSkin->leftBorder.bitmap.blur = winleft_Bitmap;
	windowSkin->leftBorder.bitmap.focusClick = winleft_Bitmap;
	windowSkin->leftBorder.bitmap.blurClick = winleft_Bitmap;
	windowSkin->leftBorder.bitmap.width = 5;
	windowSkin->leftBorder.bitmap.height = 192;

	windowSkin->rightBorder.bitmap.focus = winright_Bitmap;
	windowSkin->rightBorder.bitmap.blur = winright_Bitmap;
	windowSkin->rightBorder.bitmap.focusClick = winright_Bitmap;
	windowSkin->rightBorder.bitmap.blurClick = winright_Bitmap;
	windowSkin->rightBorder.bitmap.width = 5;
	windowSkin->rightBorder.bitmap.height = 192;

	windowSkin->bottomLeftBorder.bitmap.focus = winbottomleft_Bitmap;
	windowSkin->bottomLeftBorder.bitmap.blur = winbottomleft_Bitmap;
	windowSkin->bottomLeftBorder.bitmap.focusClick = winbottomleft_Bitmap;
	windowSkin->bottomLeftBorder.bitmap.blurClick = winbottomleft_Bitmap;
	windowSkin->bottomLeftBorder.bitmap.width = 5;
	windowSkin->bottomLeftBorder.bitmap.height = 5;

	windowSkin->bottomCentreBorder.bitmap.focus = winbottomcentre_Bitmap;
	windowSkin->bottomCentreBorder.bitmap.blur = winbottomcentre_Bitmap;
	windowSkin->bottomCentreBorder.bitmap.focusClick = winbottomcentre_Bitmap;
	windowSkin->bottomCentreBorder.bitmap.blurClick = winbottomcentre_Bitmap;
	windowSkin->bottomCentreBorder.bitmap.width = 256;
	windowSkin->bottomCentreBorder.bitmap.height = 5;

	windowSkin->bottomRightBorder.bitmap.focus = winbottomright_Bitmap;
	windowSkin->bottomRightBorder.bitmap.blur = winbottomright_Bitmap;
	windowSkin->bottomRightBorder.bitmap.focusClick = winbottomright_Bitmap;
	windowSkin->bottomRightBorder.bitmap.blurClick = winbottomright_Bitmap;
	windowSkin->bottomRightBorder.bitmap.width = 5;
	windowSkin->bottomRightBorder.bitmap.height = 5;
	
	// Create skinned screen
	SkinnedScreen* sknScreen = new SkinnedScreen("Skin", screenSkin);
	woopsiApplication->addGadget(sknScreen);

	// Create skinned window
	SkinnedWindow* sknWindow = new SkinnedWindow(10, 10, 100, 100, "window", Gadget::GADGET_DRAGGABLE, windowSkin);
	sknScreen->addGadget(sknWindow);

	// Attach bitmap
	Gadget::Rect rect;
	sknWindow->getClientRect(rect);
	SuperBitmap* superBitmap = new SuperBitmap(rect.x, rect.y, rect.width, rect.height, 164, 191, false);
	superBitmap->drawBitmap(0, 0, 164, 191, zombie_Bitmap, 0, 0, 164, 191);
	sknWindow->addGadget(superBitmap);

	// Draw on superbitmap
	superBitmap->drawFilledCircle(10, 10, 20, woopsiRGB(31, 31, 31));
	superBitmap->drawFilledRect(10, 40, 20, 20, woopsiRGB(31, 0, 0));

	// Anim button test
	SkinnedWindow* ikWindow = new SkinnedWindow(30, 30, 121, 71, "IK", Gadget::GADGET_DRAGGABLE, windowSkin);
	sknScreen->addGadget(ikWindow);
	ikWindow->getClientRect(rect);

	AnimButton* butt = new AnimButton(rect.x, rect.y, 111, 53, 0, 0);
	butt->getNormalAnimation()->addFrame(ik6_Bitmap, 111, 53, 0);
	butt->getNormalAnimation()->addFrame(ik7_Bitmap, 111, 53, 0);
	butt->getNormalAnimation()->addFrame(ik8_Bitmap, 111, 53, 0);
	butt->getNormalAnimation()->addFrame(ik9_Bitmap, 111, 53, 0);
	butt->getNormalAnimation()->addFrame(ik10_Bitmap, 111, 53, 0);
	butt->getClickedAnimation()->addFrame(ik1_Bitmap, 111, 53, 0);
	butt->getClickedAnimation()->addFrame(ik2_Bitmap, 111, 53, 0);
	butt->getClickedAnimation()->addFrame(ik3_Bitmap, 111, 53, 0);
	butt->getClickedAnimation()->addFrame(ik4_Bitmap, 111, 53, 0);
	butt->getClickedAnimation()->addFrame(ik5_Bitmap, 111, 53, 0);
	butt->getNormalAnimation()->setSpeed(4);
	butt->getClickedAnimation()->setSpeed(4);
	butt->setBorderless(true);
	ikWindow->addGadget(butt);
	Woopsi::registerForVBL(butt);




	// Create screens
	AmigaScreen* newScreen = new AmigaScreen("Woopsi Demo V0.30");
	woopsiApplication->addGadget(newScreen);
	newScreen->setPermeable(true);

	AmigaScreen* newScreen2 = new AmigaScreen("Another screen");
	woopsiApplication->addGadget(newScreen2);
	newScreen2->insertGadget(new Gradient(0, 0, 256, 192, woopsiRGB(0, 0, 31), woopsiRGB(31, 0, 0)));
	
	// Add child windows
	AmigaWindow* controlWindow = new AmigaWindow(0, 13, 256, 33, "Controls", Gadget::GADGET_CLOSEABLE | Gadget::GADGET_DRAGGABLE);
	newScreen->addGadget(controlWindow);

	AmigaWindow* textWindow = new AmigaWindow(0, 46, 256, 146, "Text", Gadget::GADGET_CLOSEABLE | Gadget::GADGET_DRAGGABLE);
	newScreen->addGadget(textWindow);

	RadioButtonGroup* radioButtons = new RadioButtonGroup(0, 0);
	controlWindow->addGadget(radioButtons);

	//radioButtons->newRadioButton(0, 0, 12, 12);
	//radioButtons->newRadioButton(12, 0, 12, 12);
	//radioButtons->newRadioButton(24, 0, 12, 12);
	//radioButtons->newRadioButton(36, 0, 12, 12);

	//controlWindow->addGadget(new CheckBox(48, 0, 12, 12));
	//controlWindow->addGadget(new CheckBox(60, 0, 12, 12));
	//controlWindow->addGadget(new CheckBox(72, 0, 12, 12));

	AmigaWindow* textTestWindow = new AmigaWindow(0, 0, 100, 100, "MTest", Gadget::GADGET_CLOSEABLE | Gadget::GADGET_DRAGGABLE);
	newScreen->addGadget(textTestWindow);

	textTestWindow->getClientRect(rect);
	textTestWindow->addGadget(new MultiLineTextBox(rect.x, rect.y, 80, 80, "This is some\ntest text", Gadget::GADGET_DRAGGABLE, 5));

	SliderHorizontal* slider = new SliderHorizontal(10, 30, 50, 15);
	slider->setMaximumValue(50);
	slider->setPageSize(6);
	slider->resizeGrip();
	textTestWindow->addGadget(slider);

	// Controls
	controlWindow->getClientRect(rect);
	controlWindow->addGadget(new Button(rect.x, rect.y, 41, 16, "Home"));
	controlWindow->addGadget(new Button(rect.x + 41, rect.y, 49, 16, "Index"));
	controlWindow->addGadget(new Button(rect.x + 90, rect.y, 17, 16, "<"));
	controlWindow->addGadget(new Button(rect.x + 107, rect.y, 17, 16, ">"));
	controlWindow->addGadget(new Button(rect.x + 124, rect.y, 40, 16, "Help"));

	// Text viewer
	textWindow->getClientRect(rect);
	MultiLineTextBox* multiBox = new MultiLineTextBox(rect.x + 1, rect.y + 1, 246, 127, "Woopsi\n"
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
	multiBox->setTextPositionHoriz(MultiLineTextBox::TEXT_POSITION_HORIZ_LEFT);
	textWindow->addGadget(multiBox);

	// Add Welcome notice
	newScreen->addGadget(new Alert(2, 2, 200, 80, "Welcome!", "Welcome to Woopsi!"));

	// Add another screen
	AmigaScreen* demoScreen = new AmigaScreen("Demos");
	woopsiApplication->addGadget(demoScreen);
	demoScreen->setPermeable(true);
	demoScreen->flipToTopScreen();

	// Add screen background
	//SuperBitmap* demoBackground = demoScreen->newSuperBitmap(0, 0, 256, 179, (u16*)background_Bitmap, 256, 192, true);
	demoScreen->insertGadget(new Gradient(0, 0, 256, 192, woopsiRGB(31, 0, 0), woopsiRGB(0, 0, 31)));

	// Create calculator app
	new Calculator(demoScreen);

	// Create Pong app
	new Pong(demoScreen);

	// Create Pacman app
	new PacMan(demoScreen);

	// Bitmap button test
	AmigaWindow* buttonWindow = new AmigaWindow(10, 10, 100, 100, "ButtonTest", Gadget::GADGET_CLOSEABLE | Gadget::GADGET_DRAGGABLE);
	demoScreen->addGadget(buttonWindow);

	buttonWindow->getClientRect(rect);
	buttonWindow->addGadget(new BitmapButton(rect.x, rect.y, 92, 28, 0, 0, 92, 28, bittest1_Bitmap, bittest4_Bitmap));
	buttonWindow->addGadget(new BitmapButton(rect.x, rect.y + 28, 92, 28, 0, 0, 92, 28, bittest2_Bitmap, bittest5_Bitmap));
	buttonWindow->addGadget(new BitmapButton(rect.x, rect.y + 56, 92, 27, 0, 0, 92, 28, bittest3_Bitmap, bittest6_Bitmap));


	// Move new screen to the back
	//woopsi.swapGadgetDepth(demoScreen);

	woopsiApplication->draw();

	// Infinite loop to keep the program running
	while (1)
	{
		woopsiApplication->play();

		//if (Pad.Held.A) {
		//	multiText->addText("Text");
		//	Debug::output(&woopsi, "go");
		//}

		woopsiWaitVBL();
	}
	
	delete woopsiApplication;

	return 0;
}
