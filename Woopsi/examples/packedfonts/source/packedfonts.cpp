// Includes
#include "packedfonts.h"
#include "amigascreen.h"
#include "amigawindow.h"

#include "defaultstyle.h"

#include "fonts/arial9.h"
#include "fonts/bankgothic12.h"
#include "fonts/batang12.h"
#include "fonts/batang14.h"
#include "fonts/batang15.h"
#include "fonts/courier.h"
#include "fonts/courier12.h"
#include "fonts/courier12b.h"
#include "fonts/couriermono.h"
#include "fonts/couriernew10.h"
#include "fonts/couriernew12.h"
#include "fonts/dotum13.h"
#include "fonts/fixedsys12.h"
#include "fonts/garamond10.h"
#include "fonts/garamond18.h"
#include "fonts/gillsans11.h"
#include "fonts/gillsans11b.h"
#include "fonts/gloucester10.h"
#include "fonts/goudy9.h"
#include "fonts/gulimche12.h"
#include "fonts/gulimche12b.h"
#include "fonts/gungsuh12.h"
#include "fonts/gungsuh15.h"
#include "fonts/gungsuhche11.h"
#include "fonts/gungsuhche15.h"
#include "fonts/kartika9.h"
#include "fonts/kartika10.h"
#include "fonts/latha.h"
#include "fonts/latha9.h"
#include "fonts/latha10.h"
#include "fonts/latha15.h"
#include "fonts/lucida6.h"
#include "fonts/lucida10.h"
#include "fonts/lucidacalligraphy14.h"
#include "fonts/macdemo.h"
#include "fonts/mangal9.h"
#include "fonts/modern9.h"
#include "fonts/modern9b.h"
#include "fonts/msgothic8.h"
#include "fonts/msgothic9.h"
#include "fonts/mssans9.h"
#include "fonts/mssans9b.h"
#include "fonts/msserif11.h"
#include "fonts/msserif11b.h"
#include "fonts/newtopaz.h"
#include "fonts/ocrfont8.h"
#include "fonts/poorrichard9.h"
#include "fonts/poorrichard12.h"
#include "fonts/profont9.h"
#include "fonts/roman9b.h"
#include "fonts/roman10.h"
#include "fonts/roman11i.h"
#include "fonts/roman13.h"
#include "fonts/roman13i.h"
#include "fonts/script9b.h"
#include "fonts/timesnewroman9.h"
#include "fonts/trebuchet8.h"
#include "fonts/tunga8.h"

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
	_fonts->push_back(new FontDefinition("New Topaz", new NewTopaz()));
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
	
	DefaultStyle::font = new NewTopaz();

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
