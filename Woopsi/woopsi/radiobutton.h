#ifndef _RADIO_BUTTON_H_
#define _RADIO_BUTTON_H_

#include <nds.h>
#include "button.h"

using namespace std;

class RadioButtonGroup;

class RadioButton : public Button {

public:
	enum RadioButtonState {
		RADIO_BUTTON_STATE_OFF = 0,
		RADIO_BUTTON_STATE_ON = 1,
		RADIO_BUTTON_STATE_MU = 2
	};

	RadioButton(s16 x, s16 y, u16 width, u16 height, FontBase* font = NULL);

	/**
	 * Override the Gadget::draw() method.
	 */
	virtual inline void draw() { Gadget::draw(); };

	virtual void draw(Rect clipRect);
	virtual bool click(s16 x, s16 y);

	/**
	 * Get the current state of the radio button.
	 * @return The state of the radio button.
	 */
	virtual inline RadioButtonState getState() { return _state; }

	/**
	 * Set the state of the radio button.
	 * @param state The new radio button state.
	 */
	virtual void setState(RadioButtonState state);

protected:
	RadioButtonState _state;

	/**
	 * Destructor.
	 */
	virtual inline ~RadioButton() { };
};

#endif
