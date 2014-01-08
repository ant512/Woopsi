#include "tabs.h"
#include <amigascreen.h>
#include <amigawindow.h>
#include <multilinetextbox.h>
#include <tabgroup.h>
#include <tab.h>

void Tabs::startup() {

	// Create screen
	AmigaScreen* screen = new AmigaScreen("Tabs Screen", true, true);
	woopsiApplication->addGadget(screen);

	// Get available area within screen
	Rect rect;
	screen->getClientRect(rect);

	TabGroup* tabGroup = new TabGroup(rect.x, rect.y, rect.width, 20);
	tabGroup->setGadgetEventHandler(this);
	Tab* tab1 = tabGroup->newTab("Tab 1");
	tab1->setRefcon(1);
	Tab* tab2 = tabGroup->newTab("Tab 2");
	tab2->setRefcon(2);
	Tab* tab3 = tabGroup->newTab("Tab 3");
	tab3->setRefcon(3);
	screen->addGadget(tabGroup);
	
	// Add textbox
	s16 textX = rect.x;
	s16 textY = tabGroup->getY() + tabGroup->getHeight();
	s16 textWidth = rect.width;
	s16 textHeight = rect.height - tabGroup->getHeight();
	_text1 = new ScrollingTextBox(textX, textY, textWidth, textHeight, "This is the first textbox.  It is displayed in the first tab.");
	_text2 = new ScrollingTextBox(textX, textY, textWidth, textHeight, "This is the second textbox.  It is displayed in the second tab.");
	_text3 = new ScrollingTextBox(textX, textY, textWidth, textHeight, "This is the third textbox.  It is displayed in the third tab.");
	screen->addGadget(_text1);
	screen->addGadget(_text2);
	screen->addGadget(_text3);

	_text2->shelve();
	_text3->shelve();
}

void Tabs::shutdown() {

	// Call base shutdown method
	Woopsi::shutdown();
}

void Tabs::handleValueChangeEvent(Gadget& source) {
	TabGroup* tabGroup = (TabGroup*)&source;
	const Tab* selectedTab = tabGroup->getSelectedGadget();

	switch (selectedTab->getRefcon()) {
		case 1:
			_text1->unshelve();
			_text2->shelve();
			_text3->shelve();
			break;
		case 2:
			_text1->shelve();
			_text2->unshelve();
			_text3->shelve();
			break;
		case 3:
			_text1->shelve();
			_text2->shelve();
			_text3->unshelve();
			break;
	}
}
