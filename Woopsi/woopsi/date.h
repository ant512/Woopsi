#ifndef _DATE_H_
#define _DATE_H_

#include <nds.h>

namespace WoopsiUI {

	/**
	 * Class representing a date.
	 */
	class Date {
	public:

		/**
		 * Constructor.
		 * @param day The day of this date.
		 * @param month The month of this date.
		 * @param year The year of this date.
		 */
		inline Date(u8 day, u8 month, u16 year) {
			setDate(day, month, year);
		};

		/**
		 * Destructor.
		 */
		inline ~Date() { };

		/**
		 * Get the name of the current day.
		 * @return The name of the current day.
		 */
		inline const char* getDayName() const { return _dayNames[_weekDay]; };

		/**
		 * Get the name of the current month.
		 * @return The name of the current month.
		 */
		inline const char* getMonthName() const { return _monthNames[_month - 1]; };

		/**
		 * Get the integer value of the current day of the week.  Value is 0-based
		 * where 0 represents Sunday and 6 represents Saturday.
		 * @return The current day of the week.
		 */
		inline const u8 getWeekDay() const { return _weekDay; };

		/**
		 * Returns true if the year is a leap year.
		 * @return True if the year is a leap year.
		 */
		inline const bool isLeapYear() const {
			return ((_year % 4 == 0) && ((_year % 100 != 0) || (_year % 400 == 0)));
		};

		/**
		 * Get the number of days in the current year.
		 * @return The number of days in the current year.
		 */
		inline const u16 getYearDays() {
			if (isLeapYear()) return 366;
			return 365;
		}

		/**
		 * Get the number of days in the current month.
		 * @return The number of days in the current month.
		 */
		const u8 getMonthDays() const;

		/**
		 * Add a number of years to this date.
		 * @param years Number of years to add.
		 */
		void addYears(s16 years);

		/**
		 * Add a number of months to this date.  Automatically rolls over years if necessary.
		 * @param months Number of months to add.
		 */
		void addMonths(s16 months);

		/**
		 * Add a number of days to this date.
		 * @param days Number of days to add.
		 */
		void addDays(s16 days);

		/**
		 * Get the day.
		 * @return The day.
		 */
		inline const u8 getDay() const { return _day; };

		/**
		 * Get the month.
		 * @return The month.
		 */
		inline const u8 getMonth() const { return _month; };

		/**
		 * Get the year.
		 * @return The year.
		 */
		inline const u16 getYear() const { return _year; };

		/**
		 * Set the year.
		 * @param day The day.
		 * @param month The month.
		 * @param year The year.
		 */
		void setDate(u8 day, u8 month, u16 year);

		bool operator==(const Date& date) const;

		bool operator!=(const Date& date) const;

	private:
		u8 _day;
		u8 _month;
		u16 _year;
		u8 _weekDay;
		static const char* _dayNames[7];
		static const char* _monthNames[12];

		/**
		 * Returns 1 if the date falls before the 29th of Feb and the year is a leap year.
		 * Necessary for correct calculation of year offset.
		 * @return 1 if date before 29th Feb in leap year; 0 otherwise.
		 */
		const int getLeapCompensationValue() const;

		/**
		 * Calculates the week day represented by the current day.
		 */
		void calculateWeekDay();

		/**
		 * Set the year.
		 * @param year The new year.
		 */
		void setYear(u16 year);

		/**
		 * Set the month.  Automatically wraps around years if the month value is not
		 * valid.
		 * @param month The new month.
		 */
		void setMonth(u8 month);
		/**
		 * Set the day.  Automatically wraps around days if the day value is not valid.
		 * @param day The new day.
		 */
		void setDay(u8 day);
	};
}

#endif
