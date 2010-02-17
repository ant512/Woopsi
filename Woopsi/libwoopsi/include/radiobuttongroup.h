#ifndef _RADIO_BUTTON_GROUP_H_
#define _RADIO_BUTTON_GROUP_H_

#include <nds.h>
#include "gadget.h"
#include "gadgetstyle.h"

namespace WoopsiUI {

	class RadioButton;

	/**
	 * Container class that holds radio button gadgets and tracks their status.
	 * The group provides an easy way to determine which radio button is
	 * selected.  Note that, in order to set the "mu" state for a radio button,
	 * it is necessary to set the state via the radio button, not the group.
	 */
	class RadioButtonGroup : public Gadget {

	public:

		/**
		 * Constructor.  Note that the group determines its width and height
		 * from the position and dimensions of its children.
		 * @param x The x co-ordinate of the group.
		 * @param y The y co-ordinate of the group.
		 * @param style The style that the gadget should use.  If this is not
		 * specified, the gadget will use the values stored in the global
		 * defaultGadgetStyle object.  The gadget will copy the properties of
		 * the style into its own internal style object.
		 */
		RadioButtonGroup(s16 x, s16 y, GadgetStyle* style = NULL);

		/**
		 * Simple method for adding a new radio button to the group.
		 * This should be used in preference to the usual addGadget() method,
		 * as this method automatically resizes the group.
		 * @param x The x co-ordinate of the new button, relative to this
		 * gadget.
		 * @param y The y co-ordinate of the new button, relative to this
		 * gadget.
		 * @param width The width of the new button.
		 * @param height The height of the new button.
		 */
		RadioButton* newRadioButton(s16 x, s16 y, u16 width, u16 height);

		/**
		 * Gets a pointer to the selected gadget.
		 * @return Pointer to the selected gadget.
		 */
		virtual const RadioButton* getSelectedGadget() const;

		/**
		 * Gets the index of the selected gadget.
		 * @return The index of the selected gadget.
		 */
		virtual const s32 getSelectedIndex() const;

		/**
		 * Sets the selected radio button to the supplied gadget.
		 * @param gadget The radio button to select.
		 */
		virtual void setSelectedGadget(RadioButton* gadget);

		/**
		 * Selects the gadget at the specified index.
		 * @param index The index of the gadget to select.
		 */
		virtual void setSelectedIndex(s32 index);

		/**
		 * Insert the dimensions that this gadget wants to have into the rect
		 * passed in as a parameter.  All co-ordinates are relative to the
		 * gadget's parent.  Value is based on the length of the largest string
		 * in the set of options.
		 * @param rect Reference to a rect to populate with data.
		 */
		virtual void getPreferredDimensions(Rect& rect) const;

	protected:
		RadioButton* _selectedGadget;		/**< Pointer to the currently
												 selected radio button */

		/**
		 * Draw the area of this gadget that falls within the clipping region.
		 * Called by the redraw() function to draw all visible regions.
		 * @param port The GraphicsPort to draw to.
		 * @see redraw()
		 */
		virtual void drawContents(GraphicsPort* port);

		/**
		 * Destructor.
		 */
		virtual ~RadioButtonGroup() { };

		/**
		 * Copy constructor is protected to prevent usage.
		 */
		inline RadioButtonGroup(const RadioButtonGroup& radioButtonGroup) : Gadget(radioButtonGroup) { };
	};
}

#endif
