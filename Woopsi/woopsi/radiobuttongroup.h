#ifndef _RADIO_BUTTON_GROUP_H_
#define _RADIO_BUTTON_GROUP_H_

#include <nds.h>
#include "gadget.h"

using namespace std;

class RadioButton;

class RadioButtonGroup : public Gadget {

public:

	RadioButtonGroup(s16 x, s16 y, FontBase* font = NULL);

	RadioButton* newRadioButton(s16 x, s16 y, u16 width, u16 height);

	virtual RadioButton* getSelectedGadget();
	virtual s16 getSelectedIndex();
	virtual void setSelectedGadget(RadioButton* gadget);
	virtual void setSelectedIndex(u8 index);
	virtual bool resize(u16 width, u16 height);

	/**
	 * Override the Gadget::draw() method.
	 */
	virtual inline void draw() { Gadget::draw(); };

	virtual void draw(Rect clipRect);

protected:
	RadioButton* _selectedGadget;

	/**
	 * Destructor.
	 */
	virtual ~RadioButtonGroup() { };
};

#endif
