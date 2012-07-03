#ifndef RF_VECTOR3_H_
#define RF_VECTOR3_H_

#include "SoraPlatform.h"
#include <cmath>
#include <cassert>

namespace sora {

	class SORA_API SoraVector3 {
	public:
		static const SoraVector3 Zero;
		
		SoraVector3(): x(0.f), y(0.f), z(0.f) {}
		SoraVector3(real _x, real _y, real _z): x(_x), y(_y), z(_z) {}
		SoraVector3(const SoraVector3& rhs): x(rhs.x), y(rhs.y), z(rhs.z) {}
		
		void set(real _x, real _y, real _z) {
			this->x = _x; this->y = _y; this->z = _z;
		}

		SoraVector3& operator=(const SoraVector3& rhs)  {
			this->x = rhs.x; this->y = rhs.y; this->z = rhs.z;
			return *this;
		}

		SoraVector3 operator-(const SoraVector3& rhs) const {
			return SoraVector3(this->x-rhs.x, this->y-rhs.y, this->z-rhs.z);
		}
		SoraVector3 operator+(const SoraVector3& rhs) const {
			return SoraVector3(this->x+rhs.x, this->y+rhs.y, this->z+rhs.z);
		}
		SoraVector3 operator*(real f) const {
			return SoraVector3(this->x*f, this->y*f, this->z*f);
		}
		SoraVector3 operator/(real f) const {
			assert(f != 0);
			return SoraVector3(this->x/f, this->y/f, this->z/f);
		}
		SoraVector3& operator+=(const SoraVector3& rhs) {
			this->x += rhs.x; this->y += rhs.y; this->z += rhs.z;
			return *this;
		}
		SoraVector3& operator-=(const SoraVector3& rhs) {
			this->x -= rhs.x; this->y -= rhs.y; this->z -= rhs.z;
			return *this;
		}
        
        SoraVector3 operator-() const {
            return SoraVector3(-this->x, -this->y, -this->z);
        }
	
        real operator*(const SoraVector3& rhs) {
            return this->dot(rhs);
        }
        
		real dot(const SoraVector3& rhs) const {
			return this->x*rhs.x+this->y*rhs.y+this->z*rhs.z;
		}
		SoraVector3 cross(const SoraVector3& rhs) const {
			return SoraVector3(-this->z*rhs.y+this->y*rhs.z, this->z*rhs.x-this->x*rhs.z, -this->y*rhs.x+this->x*rhs.y);
		}
	
		real length() const {
			return (real)sqrtf(this->x*this->x + this->y*this->y + this->z*this->z);
		}
	
		real sqrLength() const {
			return this->x*this->x + this->y*this->y + this->z*this->z;
		}
	
		SoraVector3 normalize() const {
			real inv = 1/length();
			return SoraVector3(this->x*inv, this->y*inv, this->z*inv);
		}
	
		SoraVector3 negate() const {
			return SoraVector3(-this->x, -this->y, -this->z);
		}
        
        bool operator==(const SoraVector3& rhs) const {
            return this->x == rhs.x && this->y == rhs.y && this->z == rhs.z;
        }

		real x, y, z;
	};
    
} // namespace rfMath

#endif