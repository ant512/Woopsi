#ifndef _PACKED_FONTS_H_
#define _PACKED_FONTS_H_

#include "woopsi.h"
#include "fontbase.h"
#include "gadgeteventhandler.h"
#include "gadgeteventargs.h"
#include "textbox.h"
#include "cyclebutton.h"
#include "woopsiarray.h"

using namespace WoopsiUI;

class PackedFonts : public Woopsi, public GadgetEventHandler {
public:
	void startup();
	void shutdown();

	void handleReleaseEvent(const GadgetEventArgs& e);
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
	TextBox* _textbox;
	CycleButton* _cycleButton;
};

#endif
