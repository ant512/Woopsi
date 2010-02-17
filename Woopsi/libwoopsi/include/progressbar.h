#ifndef _PROGRESS_BAR_H_
#define _PROGRESS_BAR_H_

#include <nds.h>
#include "gadget.h"

namespace WoopsiUI {

	/**
	 * Gadget providing a horizontal progress bar.
	 */
	class ProgressBar : public Gadget {

	public:

		/**
		 * Constructor.
		 * @param x The x co-ord of the progress bar, relative to its parent.
		 * @param y The y co-ord of the progress bar, relative to its parent.
		 * @param width The width of the progress bar.
		 * @param height The height of the progress bar.
		 */
		ProgressBar(s16 x, s16 y, u16 width, u16 height);

		/**
		 * Get the smallest value that the progress bar can represent.
		 * @return The smallest value.
		 */
		inline const s16 getMinimumValue() const { return _minimumValue; };

		/**
		 * Get the largest value that the progress bar can represent.
		 * @return The largest value.
		 */
		inline const s16 getMaximumValue() const { return _maximumValue; };

		/**
		 * Get the current value of the progress bar.
		 * return The current progress bar value.
		 */
		inline const s16 getValue() const { return _value; };

		/**
		 * Set the smallest value that the progress bar can represent.
		 * @param value The smallest value.
		 */
		inline void setMinimumValue(const s16 value) { _minimumValue = value; };

		/**
		 * Set the largest value that the progress bar can represent.
		 * @param value The largest value.
		 */
		inline void setMaximumValue(const s16 value) { _maximumValue = value; };

		/**
		 * Set the value that of the progress bar. 
		 * @param value The new value.
		 */
		void setValue(const s16 value);

	protected:
		s16 _minimumValue;	/**< Minimum value that the grip can represent */
		s16 _maximumValue;	/**< Maximum value that the grip can represent */
		s16 _value;			/**< Value of the progress bar */

		/**
		 * Draw the area of this gadget that falls within the clipping region.
		 * Called by the redraw() function to draw all visible regions.
		 * @param port The GraphicsPort to draw to.
		 * @see redraw()
		 */
		virtual void drawContents(GraphicsPort* port);

		/**
		 * Draw the area of this gadget that falls within the clipping region.
		 * Called by the redraw() function to draw all visible regions.
		 * @param port The GraphicsPort to draw to.
		 * @see redraw()
		 */
		virtual void drawBorder(GraphicsPort* port);

		/**
		 * Destructor.
		 */
		virtual inline ~ProgressBar() { };

		/**
		 * Copy constructor is protected to prevent usage.
		 */
		inline ProgressBar(const ProgressBar& progressBar) : Gadget(progressBar) { };
	};
}

#endif
