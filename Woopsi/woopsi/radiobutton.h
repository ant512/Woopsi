#ifndef _RADIO_BUTTON_H_
#define _RADIO_BUTTON_H_

#include <nds.h>
#include "button.h"

namespace WoopsiUI {

	class RadioButtonGroup;

	/**
	 * Represents a radio button.  Radio buttons can only exist as part of a
	 * RadioButtonGroup class, and should not be instantiated individually.
	 * Radio buttons are tri-state - off, on and "mu".
	 * The mu state cannot be enabled by a user - it can only be set by the
	 * developer.
	 */
	class RadioButton : public Button {

	public:
		/**
		 * Enum listing all possible radio button states.
		 */
		enum RadioButtonState {
			RADIO_BUTTON_STATE_OFF = 0,			/**< Radio button is off */
			RADIO_BUTTON_STATE_ON = 1,			/**< Radio button is on */
			RADIO_BUTTON_STATE_MU = 2			/**< Radio button is in the third state */
		};

		/**
		 * Constructor.
		 * @param x The x co-ordinate of the radio button, relative to its parent.
		 * @param y The y co-ordinate of the radio button, relative to its parent.
		 * @param width The width of the radio button.
		 * @param height The height of the radio button.
		 * @param font The font to use for the radio button.
		 */
		RadioButton(s16 x, s16 y, u16 width, u16 height, FontBase* font = NULL);

		/**
		 * Override the Gadget::draw() method.
		 */
		virtual inline void draw() { Gadget::draw(); };

		/**
		 * Draw the area of this gadget that falls within the clipping region.
		 * Called by the draw() function to draw all visible regions.
		 * @param clipRect The clipping region to draw.
		 * @see draw()
		 */
		virtual void draw(Rect clipRect);

		/**
		 * Click this gadget at the supplied co-ordinates.
		 * @param x X co-ordinate of the click.
		 * @param y Y co-ordinate of the click.
		 * @return True if the click was successful.
		 */
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
		RadioButtonState _state;				/**< The state of the radio button */

		/**
		 * Destructor.
		 */
		virtual inline ~RadioButton() { };

		/**
		 * Copy constructor is protected to prevent usage.
		 */
		inline RadioButton(const RadioButton& radioButton) : Button(radioButton) { };
	};
}

#endif
