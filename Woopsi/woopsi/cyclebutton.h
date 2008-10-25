#ifndef _CYCLE_BUTTON_H_
#define _CYCLE_BUTTON_H_

#include <nds.h>
#include "button.h"

namespace WoopsiUI {

	/**
	 * Cycle button gadget.  Displays text within the button.  Clicking it cycles
	 * through its available options.
	 */
	class CycleButton : public Button {
	public:

		/**
		 * Struct defining a single cycle button option.
		 */
		typedef struct {
			char* text;							/**< Text to display for option. */
			u32 value;							/**< Option value. */
		} CycleButtonOption;

		/**
		 * Constructor for cycle buttons.
		 * @param x The x co-ordinate of the button, relative to its parent.
		 * @param y The y co-ordinate of the button, relative to its parent.
		 * @param width The width of the button.
		 * @param height The height of the button.
		 * @param font The font that the button should use.
		 */
		CycleButton(s16 x, s16 y, u16 width, u16 height, FontBase* font = NULL);

		/**
		 * Draw the region of the textbox within the clipping rect. Should not be called
		 * directly.
		 * @param clipRect The clipping rect to limit drawing to.
		 */
		virtual void draw(Rect clipRect);
		
		/**
		 * Draw all visible regions of the textbox.
		 */
		virtual inline void draw() { Gadget::draw(); };

		/**
		 * Add a new option to the gadget.
		 * @param text The text of the option.
		 * @param value The value of the option.
		 */
		void addOption(const char* text, const u32 value);

		/**
		 * Get the value of the current option.
		 * @return Value of the current option.
		 */
		inline const u32 getValue() const { return _options[_selectedIndex]->value; };

		/**
		 * Release this gadget at the supplied co-ordinates.
		 * @param x X co-ordinate of the release.
		 * @param y Y co-ordinate of the release.
		 * @return True if the release was successful.
		 */
		virtual bool release(s16 x, s16 y);

	protected:
		DynamicArray<CycleButtonOption*> _options;		/**< Array of options. */
		s16 _selectedIndex;								/**< Index of the currently selected option. */

		/**
		 * Prevent recalculation of text positions.
		 */
		virtual inline void calculateTextPosition() { };

		/**
		 * Destructor.
		 */
		virtual ~CycleButton();

		/**
		 * Copy constructor is protected to prevent usage.
		 */
		inline CycleButton(const CycleButton& cycleButton) : Button(cycleButton) { };
	};
}

#endif
