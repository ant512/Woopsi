#ifndef _CALENDAR_H_
#define _CALENDAR_H_

#include <nds.h>
#include "gadget.h"
#include "gadgeteventhandler.h"

#define CALENDAR_ROWS 6
#define CALENDAR_COLS 7

namespace WoopsiUI {

	class Button;
	class Label;
	class Date;

	/**
	 * Class providing a calendar/date picker.  Raises an EVENT_ACTION event
	 * when a new date is clicked.
	 */
	class Calendar : public Gadget, public GadgetEventHandler {
	public:

		/**
		 * Constructor.
		 * @param x The x co-ordinate of the calendar.
		 * @param y The y co-ordinate of the calendar.
		 * @param width The width of the calendar.
		 * @param height The height of the calendar.
		 * @param day The default day to display.
		 * @param month The default month to display.
		 * @param year The default year to display.
		 * @param flags Standard flags.
		 * @param font Optional font to use for text output.
		 */
		Calendar(s16 x, s16 y, u16 width, u16 height, u8 day, u8 month, u16 year, u32 flags, FontBase* font = NULL);

		/**
		 * Handles events raised by its sub-gadgets.
		 * @param e Event data to process.
		 */
		virtual void handleReleaseEvent(const GadgetEventArgs& e);

		/**
		 * Draw the region of the calendar within the clipping rect. Should not be called
		 * directly.
		 * @param clipRect The clipping rect to limit drawing to.
		 */
		virtual void draw(Rect clipRect);

		/**
		 * Set the date displayed in the calendar.
		 * @param day The new day.
		 * @param month The new month.
		 * @param year The new year.
		 */
		virtual void setDate(u8 day, u8 month, u16 year);

		/**
		 * Get the day displayed in the calendar.
		 * @return The day.
		 */
		const u8 getDay() const;

		/**
		 * Get the month displayed in the calendar.
		 * @return The month.
		 */
		const u8 getMonth() const;

		/**
		 * Get the year displayed in the calendar.
		 * @return The year.
		 */
		const u16 getYear() const;

		/**
		 * Resize the calendar to the new dimensions.
		 * @param width The new width.
		 * @param height The new height.
		 * @return True if the resize was successful.
		 */
		bool resize(u16 width, u16 height);

		/**
		 * Insert the dimensions that this gadget wants to have into the rect
		 * passed in as a parameter.  All co-ordinates are relative to the gadget's
		 * parent.
		 * @param rect Reference to a rect to populate with data.
		 */
		virtual void getPreferredDimensions(Rect& rect) const;

	protected:
		Date* _date;						/**< Calendar working date */
		Date* _visibleDate;					/**< Date displayed in the UI */
		Button* _selectedDayButton;			/**< Pointer to the selected day button */
		Button* _leftArrow;					/**< Pointer to the left arrow */
		Button* _rightArrow;				/**< Pointer to the right arrow */
		Label* _monthLabel;					/**< Pointer to the month label */

		/**
		 * Destructor.
		 */
		virtual ~Calendar();

		/**
		 * Creates all gadgets but leaves them textless.
		 */
		virtual void buildGUI();

		/**
		 * Populates the previously-created gadget layout with correct text
		 * based on current date objects.
		 */
		virtual void populateGUI();

		/**
		 * Copy constructor is protected to prevent usage.
		 */
		inline Calendar(const Calendar& calendar) : Gadget(calendar) { };

		/**
		 * Calculate the widths of the day buttons.
		 * @param spaceWidth Width of the space to fill.
		 * @param dayWidths Array large enough to contain the widths of the days
		 * for a single row.
		 */
		void calculateDayButtonWidths(s32 spaceWidth, u8* dayWidths);
	};
}

#endif
