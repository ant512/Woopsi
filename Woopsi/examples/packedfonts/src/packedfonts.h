#ifndef _PACKED_FONTS_H_
#define _PACKED_FONTS_H_

#include "woopsi.h"
#include "fontbase.h"
#include "gadgeteventhandler.h"
#include "multilinetextbox.h"
#include "cyclebutton.h"
#include "woopsiarray.h"

using namespace WoopsiUI;

class PackedFonts : public Woopsi, public GadgetEventHandler {
public:
	void handleReleaseEvent(Gadget& source, const WoopsiPoint& point);
private:
	class FontDefinition {
	public:
	
		FontDefinition() {
			fontName = NULL;
			font = NULL;
		}
	
		FontDefinition(const char* fontName, FontBase* font) {
			this->fontName = new char[strlen(fontName) + 1];
			strcpy(this->fontName, fontName);
			
			this->font = font;
		}
		
		~FontDefinition() {
			delete font;
			delete [] fontName;
		}
		
		char* fontName;
		FontBase* font;
	};
	
	WoopsiArray<FontDefinition*>* _fonts;
	MultiLineTextBox* _textbox;
	CycleButton* _cycleButton;
	
	void startup();
	void shutdown();
};

#endif
