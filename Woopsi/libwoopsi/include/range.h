#ifndef _RANGE_H_
#define _RANGE_H_

#include <nds.h>

namespace WoopsiUI {

	/**
	 * Can scale values to fit within a given range.  Used by the slider
	 * gadgets to convert from logical values to pixels.
	 */
	class Range {

	public:

		/**
		 * Constructor.
		 * @param minimumValue The smallest value in the range.
		 * @param maximumValue The largest value in the range.
		 * @param span The size of the scaled range.
		 */
		Range(s32 minimumValue, s32 maximumValue, s32 span);
		
		/**
		 * Destructor.
		 */
		~Range() { };

		/**
		 * Get the minimum value in the range.
		 * @return The minimum value in the range.
		 */
		inline const s32 getMinimumValue() const { return _minimumValue; };
		
		/**
		 * Get the maximum value in the range.
		 * @return The maximum value in the range.
		 */
		inline const s32 getMaximumValue() const { return _maximumValue; };
		
		/**
		 * Get the size of the scaled range.
		 * @return The size of the scaled range.
		 */
		inline const s32 getSpan() const { return _span; };
		
		/**
		 * Set the minimum value of the range.
		 * @param value The minimum value of the range.
		 */
		inline void setMinimumValue(const s32 value) { _minimumValue = value; };
		
		/**
		 * Set the maximum value of the range.
		 * @param value The maximum value of the range.
		 */
		inline void setMaximumValue(const s32 value) { _maximumValue = value; };
		
		/**
		 * Set the size of the scaled range.
		 * @param span The size of the scaled range.
		 */
		inline void setSpan(const s32 span) { _span = span; };

		/**
		 * Scale the value to fit within the current range.
		 * @param value The value to scale.
		 * @return The scaled value.
		 */
		const s32 convertValueToScaled(s32 value) const;
		
		/**
		 * Convert the scaled value back to an unscaled value.
		 * @param scaledValue The scaled value to convert.
		 * @return The unscaled value.
		 */
		const s32 convertScaledToValue(s32 scaledValue) const;

	protected:
		s32 _minimumValue;							/**< Minimum value that the range can represent. */
		s32 _maximumValue;							/**< Maximum value that the range can represent. */
		s32 _span;									/**< Size of the area representing the range. */
	};
}

#endif
