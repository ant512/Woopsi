#ifndef _WOOPSIPOINT_H_
#define _WOOPSIPOINT_H_

#import <nds.h>

namespace WoopsiUI {
	class WoopsiPoint {
	public:
		inline WoopsiPoint(const s32 x, const s32 y) {
			_x = x;
			_y = y;
		};

		inline WoopsiPoint(const WoopsiPoint& point) {
			_x = point.getX();
			_y = point.getY();
		};

		inline ~WoopsiPoint() { };

		inline const s32 getX() const { return _x; };
		inline const s32 getY() const { return _y; };

		inline void setX(const s32 x) { _x = x; };
		inline void setY(const s32 y) { _y = y; };

		inline bool operator==(const WoopsiPoint& point) const {
			return point.getX() == _x && point.getY() == _y;
		}

		inline bool operator!=(const WoopsiPoint& point) const {
			return point.getX() != _x || point.getY() != _y;
		}

		inline WoopsiPoint& operator=(const WoopsiPoint& point) {
			if (&point != this) {
				_x = point.getX();
				_y = point.getY();
			}

			return *this;
		}

	private:
		s32 _x;
		s32 _y;
	};
}

#endif
