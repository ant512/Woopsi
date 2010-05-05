/*
 * Copyright Mark Adamson 2010
 */
 
#ifndef _COLOUR_PICKER_H_
#define _COLOUR_PICKER_H_

#include "amigawindow.h"
#include "gadgetstyle.h"

namespace WoopsiUI {

	class Button;
	class SliderHorizontal;

	/**
	 * Provides a window with RGB colour selection sliders.
	 */
	class ColourPicker : public AmigaWindow {
	public:

		/**
		 * Constructor.
		 * @param x The x co-ordinate of the window.
		 * @param y The y co-ordinate of the window.
		 * @param width The width of the window.
		 * @param height The height of the window.
		 * @param title The title of the window.
		 * @param flags Standard flags.
		 * @param style Optional gadget style.
		 */
		ColourPicker(s16 x, s16 y, u16 width, u16 height, const WoopsiString& title, u16 colour, u32 flags, GadgetStyle* style = NULL);

		/**
		 * Get the colour currently shown by the gadget.
		 * @return The colour currently shown by the gadget.
		 */
		virtual u16 getColour() const;

		/**
		 * Sets the colour shown by the gadget.
		 * @param colour The new colour to display.
		 */
		virtual void setColour(u16 colour);

		/**
		 * Handles events raised by its sub-gadgets.
		 * @param e Event arguments.
		 */
		virtual void handleReleaseEvent(const GadgetEventArgs& e);
	
		/**
		 * Handles events raised by its sub-gadgets.
		 * @param e Event arguments.
		 */
		virtual void handleValueChangeEvent(const GadgetEventArgs& e);

	protected:
		Button* _okButton;							/**< Pointer to the OK button. */
		Button* _cancelButton;						/**< Pointer to the cancel button. */
		Button* _colourButton;						/**< Displays the current colour. */
		SliderHorizontal* _redSlider;				/**< Pointer to the Red value slider */
		SliderHorizontal* _greenSlider;				/**< Pointer to the Green value slider */
		SliderHorizontal* _blueSlider;				/**< Pointer to the Blue value slider */

		/**
		 * Resize the textbox to the new dimensions.
		 * @param width The new width.
		 * @param height The new height.
		 */
		virtual void onResize(u16 width, u16 height);
		
		/**
		 * Destructor.
		 */
		virtual ~ColourPicker() { };
		
		/**
		 * Copy constructor is protected to prevent usage.
		 */
		inline ColourPicker(const ColourPicker& colourPicker) : AmigaWindow(colourPicker) { };
	};
}

#endif
