#ifndef _WOOPSIPOINT_H_
#define _WOOPSIPOINT_H_

#include <nds.h>

namespace WoopsiUI {
	
	/**
	 * A co-ordinate in 2D space.
	 */
	class WoopsiPoint {
	public:

		/**
		 * Constructor.
		 */
		inline WoopsiPoint() {
			_x = 0;
			_y = 0;
		};
		
		/**
		 * Constructor.
		 * @param x The x co-ordinate of the point.
		 * @param y The y co-ordinate of the point.
		 */
		inline WoopsiPoint(const s32 x, const s32 y) {
			_x = x;
			_y = y;
		};

		/**
		 * Copy-constructor.
		 * @param point The point to copy.
		 */
		inline WoopsiPoint(const WoopsiPoint& point) {
			_x = point.getX();
			_y = point.getY();
		};

		/**
		 * Destructor.
		 */
		inline ~WoopsiPoint() { };

		/**
		 * Gets the point's x co-ordinate.
		 * @return The point's x co-ordinate.
		 */
		inline const s32 getX() const { return _x; };
		
		/**
		 * Gets the point's y co-ordinate.
		 * @return The point's y co-ordinate.
		 */
		inline const s32 getY() const { return _y; };

		/**
		 * Sets the point's x co-ordinate.
		 * @param x The point's x co-ordinate.
		 */
		inline void setX(const s32 x) { _x = x; };
		
		/**
		 * Sets the point's y co-ordinate.
		 * @param y The point's y co-ordinate.
		 */
		inline void setY(const s32 y) { _y = y; };

		/**
		 * Equality operator.
		 * @param point The point to compare with.
		 * @return True if both points have the same co-ordinates.
		 */
		inline bool operator==(const WoopsiPoint& point) const {
			return point.getX() == _x && point.getY() == _y;
		}

		/**
		 * Inequality operator.
		 * @param point The point to compare with.
		 * @return True if the points have different co-ordinates.
		 */
		inline bool operator!=(const WoopsiPoint& point) const {
			return point.getX() != _x || point.getY() != _y;
		}

		/**
		 * Set equal to operator.
		 * @param point The point to make equal to.
		 * @return The updated point.
		 */
		inline WoopsiPoint& operator=(const WoopsiPoint& point) {
			if (&point != this) {
				_x = point.getX();
				_y = point.getY();
			}

			return *this;
		}

	private:
		s32 _x;		/**< x co-ordinate of the point. */
		s32 _y;		/**< y co-ordinate of the point. */
	};
}

#endif
