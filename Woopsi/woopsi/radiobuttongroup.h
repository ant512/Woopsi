#ifndef _RADIO_BUTTON_GROUP_H_
#define _RADIO_BUTTON_GROUP_H_

#include <nds.h>
#include "button.h"

using namespace std;

class RadioButton;

class RadioButtonGroup : public Gadget {

public:

	RadioButtonGroup(s16 x, s16 y, FontBase* font = NULL);
	virtual ~RadioButtonGroup();

	RadioButton* newRadioButton(s16 x, s16 y, u16 width, u16 height);

	virtual bool click(s16 x, s16 y);
	virtual RadioButton* getSelectedGadget();
	virtual s16 getSelectedIndex();
	virtual void setSelectedGadget(RadioButton* gadget);
	virtual void setSelectedIndex(u8 index);
	virtual bool resize(u16 width, u16 height);
	virtual void draw(Rect clipRect);
	virtual void draw();

protected:
	RadioButton* _selectedGadget;
};

#endif
