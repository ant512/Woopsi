#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <nds.h>
#include "textwriter.h"
#include "textbox.h"

using namespace std;

class Button : public Textbox {
public:
	Button(s16 x, s16 y, u16 width, u16 height, char* text, FontBase* font = NULL);
	Button(s16 x, s16 y, u16 width, u16 height, char letter, FontBase* font = NULL);
	virtual ~Button();
};

#endif
