// Includes
#include "packedfonts.h"
#include "amigascreen.h"
#include "amigawindow.h"

#include "arial9.h"
#include "bankgothic12.h"
#include "courier.h"
#include "couriermono.h"
#include "garamond10.h"
#include "kartika10.h"
#include "latha.h"
#include "latha15.h"
#include "lucida6.h"
#include "lucida10.h"
#include "lucidacalligraphy14.h"
#include "macdemo.h"
#include "ocrfont8.h"
#include "poorrichard12.h"
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
	_fonts->push_back(new FontDefinition("Courier", new Courier()));
	_fonts->push_back(new FontDefinition("Courier Mono", new CourierMono()));
	_fonts->push_back(new FontDefinition("Garamond 10", new Garamond10()));
	_fonts->push_back(new FontDefinition("Kartika 10", new Kartika10()));
	_fonts->push_back(new FontDefinition("Latha", new Latha()));
	_fonts->push_back(new FontDefinition("Latha 15", new Latha15()));
	_fonts->push_back(new FontDefinition("Lucida 6", new Lucida6()));
	_fonts->push_back(new FontDefinition("Lucida 10", new Lucida10()));
	_fonts->push_back(new FontDefinition("Lucida Calligraphy 14", new LucidaCalligraphy14()));
	_fonts->push_back(new FontDefinition("Mac Demo", new MacDemo()));
	_fonts->push_back(new FontDefinition("OCR Font 8", new OCRFont8()));
	_fonts->push_back(new FontDefinition("Poor Richard 12", new PoorRichard12()));
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
	_textbox = new TextBox(rect.x, rect.y, rect.width, rect.height / 2, "Hello World!");//, _fonts->at(0)->font);
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
