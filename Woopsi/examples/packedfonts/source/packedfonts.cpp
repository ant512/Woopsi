// Includes
#include "packedfonts.h"
#include "amigascreen.h"
#include "amigawindow.h"

#include "arial9.h"
#include "bankgothic12.h"
#include "batang12.h"
#include "batang14.h"
#include "batang15.h"
#include "courier.h"
#include "courier12.h"
#include "courier12b.h"
#include "couriermono.h"
#include "couriernew10.h"
#include "couriernew12.h"
#include "dotum13.h"
#include "fixedsys12.h"
#include "garamond10.h"
#include "garamond18.h"
#include "gillsans11.h"
#include "gillsans11b.h"
#include "gloucester10.h"
#include "goudy9.h"
#include "gulimche12.h"
#include "gulimche12b.h"
#include "gungsuh12.h"
#include "gungsuh15.h"
#include "gungsuhche11.h"
#include "gungsuhche15.h"
#include "kartika9.h"
#include "kartika10.h"
#include "latha.h"
#include "latha9.h"
#include "latha10.h"
#include "latha15.h"
#include "lucida6.h"
#include "lucida10.h"
#include "lucidacalligraphy14.h"
#include "macdemo.h"
#include "mangal9.h"
#include "modern9.h"
#include "modern9b.h"
#include "msgothic8.h"
#include "msgothic9.h"
#include "mssans9.h"
#include "mssans9b.h"
#include "msserif11.h"
#include "msserif11b.h"
#include "ocrfont8.h"
#include "poorrichard9.h"
#include "poorrichard12.h"
#include "profont9.h"
#include "roman9b.h"
#include "roman10.h"
#include "roman11i.h"
#include "roman13.h"
#include "roman13i.h"
#include "script9b.h"
#include "timesnewroman9.h"
#include "trebuchet8.h"
#include "tunga8.h"

void PackedFonts::startup() {

	// Call base startup method
	Woopsi::startup();
	
	_fonts = new WoopsiArray<FontDefinition*>();
	
	// Create list of fonts
	_fonts->push_back(new FontDefinition("Arial 9", new Arial9()));
	_fonts->push_back(new FontDefinition("Bank Gothic 12", new BankGothic12()));
	_fonts->push_back(new FontDefinition("Batang 12", new Batang12()));
	_fonts->push_back(new FontDefinition("Batang 14", new Batang14()));
	_fonts->push_back(new FontDefinition("Batang 15", new Batang15()));
	_fonts->push_back(new FontDefinition("Courier", new Courier()));
	_fonts->push_back(new FontDefinition("Courier 12", new Courier12()));
	_fonts->push_back(new FontDefinition("Courier 12b", new Courier12b()));
	_fonts->push_back(new FontDefinition("Courier Mono", new CourierMono()));
	_fonts->push_back(new FontDefinition("Courier New 10", new CourierNew10()));
	_fonts->push_back(new FontDefinition("Courier New 12", new CourierNew12()));
	_fonts->push_back(new FontDefinition("Dotum 13", new Dotum13()));
	_fonts->push_back(new FontDefinition("Fixed Sys 12", new FixedSys12()));
	_fonts->push_back(new FontDefinition("Garamond 10", new Garamond10()));
	_fonts->push_back(new FontDefinition("Garamond 18", new Garamond18()));
	_fonts->push_back(new FontDefinition("Gill Sans 11", new GillSans11()));
	_fonts->push_back(new FontDefinition("Gill Sans 11b", new GillSans11b()));
	_fonts->push_back(new FontDefinition("Gloucester 10", new Gloucester10()));
	_fonts->push_back(new FontDefinition("Goudy 9", new Goudy9()));
	_fonts->push_back(new FontDefinition("Gulimche 12", new Gulimche12()));
	_fonts->push_back(new FontDefinition("Gulimche 12b", new Gulimche12b()));
	_fonts->push_back(new FontDefinition("Gungsuh 12", new Gungsuh12()));
	_fonts->push_back(new FontDefinition("Gungsuh 15", new Gungsuh15()));
	_fonts->push_back(new FontDefinition("Gungsuhche 11", new Gungsuhche11()));
	_fonts->push_back(new FontDefinition("Gungsuhche 15", new Gungsuhche15()));
	_fonts->push_back(new FontDefinition("Kartika 9", new Kartika9()));
	_fonts->push_back(new FontDefinition("Kartika 10", new Kartika10()));
	_fonts->push_back(new FontDefinition("Latha", new Latha()));
	_fonts->push_back(new FontDefinition("Latha 9", new Latha9()));
	_fonts->push_back(new FontDefinition("Latha 10", new Latha10()));
	_fonts->push_back(new FontDefinition("Latha 15", new Latha15()));
	_fonts->push_back(new FontDefinition("Lucida 6", new Lucida6()));
	_fonts->push_back(new FontDefinition("Lucida 10", new Lucida10()));
	_fonts->push_back(new FontDefinition("Lucida Calligraphy 14", new LucidaCalligraphy14()));
	_fonts->push_back(new FontDefinition("Mac Demo", new MacDemo()));
	_fonts->push_back(new FontDefinition("Mangal 9", new Mangal9()));
	_fonts->push_back(new FontDefinition("Modern 9", new Modern9()));
	_fonts->push_back(new FontDefinition("Modern 9b", new Modern9b()));
	_fonts->push_back(new FontDefinition("MS Gothic 8", new MSGothic8()));
	_fonts->push_back(new FontDefinition("MS Gothic 9", new MSGothic9()));
	_fonts->push_back(new FontDefinition("MS Sans 9", new MSSans9()));
	_fonts->push_back(new FontDefinition("MS Sans 9b", new MSSans9b()));
	_fonts->push_back(new FontDefinition("MS Serif 11", new MSSerif11()));
	_fonts->push_back(new FontDefinition("MS Serif 11b", new MSSerif11b()));
	_fonts->push_back(new FontDefinition("OCR Font 8", new OCRFont8()));
	_fonts->push_back(new FontDefinition("Poor Richard 9", new PoorRichard9()));
	_fonts->push_back(new FontDefinition("Poor Richard 12", new PoorRichard12()));
	_fonts->push_back(new FontDefinition("Pro Font 9", new ProFont9()));
	_fonts->push_back(new FontDefinition("Roman 9b", new Roman9b()));
	_fonts->push_back(new FontDefinition("Roman 10", new Roman10()));
	_fonts->push_back(new FontDefinition("Roman 11i", new Roman11i()));
	_fonts->push_back(new FontDefinition("Roman 13", new Roman13()));
	_fonts->push_back(new FontDefinition("Roman 13i", new Roman13i()));
	_fonts->push_back(new FontDefinition("Script 9b", new Script9b()));
	_fonts->push_back(new FontDefinition("Times New Roman 9", new TimesNewRoman9()));
	_fonts->push_back(new FontDefinition("Trebuchet 8", new Trebuchet8()));
	_fonts->push_back(new FontDefinition("Tunga 9", new Tunga8()));

	// Create screen
	AmigaScreen* screen = new AmigaScreen("Hello World Screen", Gadget::GADGET_DRAGGABLE, AmigaScreen::AMIGA_SCREEN_SHOW_DEPTH | AmigaScreen::AMIGA_SCREEN_SHOW_FLIP);
	woopsiApplication->addGadget(screen);

	// Add window
	AmigaWindow* window = new AmigaWindow(0, 13, 256, 179, "Hello World Window", Gadget::GADGET_DRAGGABLE, AmigaWindow::AMIGA_WINDOW_SHOW_CLOSE | AmigaWindow::AMIGA_WINDOW_SHOW_DEPTH);
	screen->addGadget(window);

	// Get available area within window
	Rect rect;
	window->getClientRect(rect);
	
	// Add textbox
	_textbox = new TextBox(rect.x, rect.y, rect.width, rect.height / 2, "The quick brown fox", _fonts->at(0)->font);
	window->addGadget(_textbox);
	
	// Add cyclebutton
	_cycleButton = new CycleButton(rect.x, rect.y + (rect.height / 2), rect.width, rect.height / 2);
	for (s32 i = 0; i < _fonts->size(); ++i) {
		_cycleButton->addOption(_fonts->at(i)->fontName, i);
	}
	_cycleButton->setRefcon(1);
	_cycleButton->addGadgetEventHandler(this);
	window->addGadget(_cycleButton);

	// Ensure Woopsi can draw itself
	enableDrawing();
	
	// Draw GUI
	redraw();
}

void PackedFonts::shutdown() {

	for (s32 i = 0; i < _fonts->size(); ++i) {
		delete _fonts->at(i);
	}
	
	delete _fonts;

	// Call base shutdown method
	Woopsi::shutdown();
}

void PackedFonts::handleReleaseEvent(const GadgetEventArgs& e) {
	if (e.getSource() != NULL) {
		switch (e.getSource()->getRefcon()) {
			case 1:
				// Cycle button
				_textbox->setFont(_fonts->at(_cycleButton->getValue())->font);
				break;
		}
	}
}
