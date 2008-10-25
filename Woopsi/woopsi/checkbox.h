#ifndef _CHECKBOX_H_
#define _CHECKBOX_H_

#include <nds.h>
#include "button.h"

namespace WoopsiUI {

	/**
	 * Class representing a radio button.  Like radio buttons, checkboxes
	 * are tri-state - off, on and "mu".  The mu state cannot be enabled by
	 * a user - it can only be set by the developer.
	 */
	class CheckBox : public Button {

	public:
		/**
		 * Enum listing all possible checkbox states.
		 */
		enum CheckBoxState {
			CHECK_BOX_STATE_OFF = 0,			/**< Checkbox is unticked */
			CHECK_BOX_STATE_ON = 1,				/**< Checkbox is ticked */
			CHECK_BOX_STATE_MU = 2				/**< Checkbox is in the third state */
		};

		/**
		 * Constructor.
		 * @param x The x co-ordinate of the checkbox, relative to its parent.
		 * @param y The y co-ordinate of the checkbox, relative to its parent.
		 * @param width The width of the checkbox.
		 * @param height The height of the checkbox.
		 * @param font The font to use for the checkbox.
		 */
		CheckBox(s16 x, s16 y, u16 width, u16 height, FontBase* font = NULL);

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
		 * Get the current state of the checkbox.
		 * @return The state of the checkbox.
		 */
		virtual inline const CheckBoxState getState() const { return _state; };

		/**
		 * Set the state of the checkbox.
		 * @param state The new checkbox state.
		 */
		virtual void setState(CheckBoxState state);

	protected:
		CheckBoxState _state;				/**< The state of the checkbox */

		/**
		 * Destructor.
		 */
		virtual inline ~CheckBox() { };

		/**
		 * Copy constructor is protected to prevent usage.
		 */
		inline CheckBox(const CheckBox& checkBox) : Button(checkBox) { };
	};
}

#endif
