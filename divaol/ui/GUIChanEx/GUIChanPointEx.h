#pragma once

#include <cmath>

namespace gcn
{

	struct PointEx {
		public:
			PointEx():x(0),y(0) {}
			PointEx(int x, int y):x(x),y(y) {}

			void set(int _x, int _y) {
				x = _x, y = _y;
			}

		public:
			int x, y;
		
	};
}