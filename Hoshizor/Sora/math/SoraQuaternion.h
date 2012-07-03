/*
 *  SoraQuaternion.h
 *  lParticle
 *
 *  Created by GriffinBu on 12/21/10.
 *  Copyright 2010 Robert Bu. All rights reserved.
 *
 */

#ifndef RF_QUATERNION_H_
#define RF_QUATERNION_H_

#include "SoraVector3.h"

namespace sora {
	
	class SORA_API SoraQuaternion {
	public:
		SoraQuaternion(real _x, real _y, real _z, real _w):
			w(_w), x(_x), y(_y), z(_z) {}
		SoraQuaternion(const SoraQuaternion& quad):
			w(quad.w), x(quad.x), y(quad.y), z(quad.z) {}
		SoraQuaternion():
			w(0.f), x(0.f), y(0.f), z(0.f) {}
        SoraQuaternion(real eulerX, real eulerY, real eulerZ) {
            SoraQuaternion roll( sinf( eulerX * 0.5f ), 0, 0, cosf( eulerX * 0.5f ) );
            SoraQuaternion pitch( 0, sinf( eulerY * 0.5f ), 0, cosf( eulerY * 0.5f ) );
            SoraQuaternion yaw( 0, 0, sinf( eulerZ * 0.5f ), cosf( eulerZ * 0.5f ) );
            
            // Order: y * x * z
            *this = pitch * roll * yaw;
        }
		
		void set(real _x, real _y, real _z, real _w) {
			this->x = _x; 
            this->y = _y; 
            this->z = _z; 
            this->w = _w;
		}
		
		void set(const SoraQuaternion& quad) {
			this->x = quad.x; 
            this->y = quad.y; 
            this->z = quad.z; 
            this->w = quad.w;
		}
		
		SoraVector3 get() const {
			return SoraVector3(x, y, z);
		}
		
		inline SoraQuaternion& operator=(const SoraQuaternion& rhs) {
			this->x = rhs.x; 
            this->y = rhs.y;
            this->z = rhs.z; 
            this->w = rhs.w;
			return *this;
		}
		
		inline SoraQuaternion operator*(const SoraQuaternion& rhs) {
			SoraQuaternion ans;
			real d1, d2, d3, d4;
			d1 = this->w*rhs.w;
			d2 = -this->x*rhs.x;
			d3 = -this->y*rhs.y;
			d4 = -this->z*rhs.z;
			ans.w = d1+d2+d3+d4;
			
			d1 = this->w*rhs.x;
			d2 = this->x*rhs.w;
			d3 = this->y*rhs.z;
			d4 = -this->z*rhs.y;
			ans.x = d1+d2+d3+d4;
			
			d1 = this->w*rhs.y;
			d2 = this->y*rhs.w;
			d3 = this->z*rhs.x;
			d4 = -this->x*rhs.z;
			ans.y = d1+d2+d3+d4;
			
			d1 = this->w*rhs.z;
			d2 = this->z*rhs.w;
			d3 = this->x*rhs.y;
			d4 = -this->y*rhs.x;
			ans.z = d1+d2+d3+d4;
			
			*this = ans;
			return *this;
		}
		
		void makeRotate(real angle, real _x, real _y, real _z) {
			real inversenorm = (real)1.0f/sqrtf(x*x + y*y + z*z);
			real coshalfangle = cosf(0.5f*angle);
			real sinhalfangle = sinf(0.5f*angle);
			
			this->x = _x * sinhalfangle * inversenorm;
			this->y = _y * sinhalfangle * inversenorm;
			this->z = _z * sinhalfangle * inversenorm;
			this->w = coshalfangle;
		}
		
		void makeRotate(real angle, const SoraVector3& vec) {
			makeRotate(angle, vec.x, vec.y, vec.z);
		}
		
		void makeRotate(real roll, real pitch, real yaw) {
			real sh0 = sinf(0.5f*roll);
			real ch0 = cosf(0.5f*roll);
			real sh1 = sinf(0.5f*pitch);
			real ch1 = cosf(0.5f*pitch);
			real sh2 = sinf(0.5f*yaw);
			real ch2 = cosf(0.5f*yaw);
			
			this->x = sh0*ch1*ch2 + ch0*sh1*sh2;
			this->y = ch0*sh2*ch1 + sh0*ch2*sh1;
			this->z = ch0*ch2*sh1 - sh0*sh2*ch1;
			this->w = ch0*ch1*ch2 - sh0*sh1*sh2;
		}
		
		SoraVector3 rotate(const SoraVector3& v) const {
			real ux = this->w*v.x + this->y*v.z - this->z*v.y;
			real uy = this->w*v.y + this->z*v.x - this->x*v.z;
			real uz = this->w*v.z + this->x*v.y - this->y*v.x;
			real uw = -this->x*v.x - this->y*v.y - this->z*v.z;
			real vx = -uw*this->x + ux*this->w - uy*this->z + uz*this->y;
			real vy = -uw*this->y + uy*this->w - uz*this->x + ux*this->z;
			real vz = -uw*this->z + uz*this->w - ux*this->y + uz*this->x;
			return SoraVector3(vx, vy, vz);
		}
		
		void getRotate(real& angle, real& _x, real& _y, real& _z) const {
			real sinhalfangle = sqrtf(this->x*this->x + this->y*this->y + this->z*this->z);
			angle = 2.f * atan2f(sinhalfangle, this->w);
			if(sinhalfangle) {
				_x = x / sinhalfangle;
				_y = y / sinhalfangle;
				_z = z / sinhalfangle;
			} else {
				_x = 0.0f;
				_y = 0.0f;
				_z = 0.0f;
			}
		}
		
		void getRotate(real& angle, SoraVector3& vec) const {
			getRotate(angle, vec.x, vec.y, vec.z);
		}
		
		void slerp(real t, const SoraQuaternion& from, const SoraQuaternion& to) {
			const real epsilon = 0.00001f;
			real cosomega, scale_from, scale_to;
			
			SoraQuaternion quatTo(to);
			
			cosomega = from.x*to.x + from.y*to.y + from.z*to.z + from.w*to.w;
			if(cosomega < 0.0f) {
				cosomega = -cosomega;
				quatTo.set(-to.x, -to.y, -to.z, -to.w);
			}
			
			if((1.0 - cosomega) > epsilon) {
				real omega = acosf(cosomega);
				real invsinomega = 1.0f/sinf(omega);
				scale_from = sinf((1.0f-t)*omega)*invsinomega;
				scale_to = sinf(t*omega)*invsinomega;
			} else {
				scale_from = 1.0f - t;
				scale_to = t;
			}
			
			this->x = from.x*scale_from + quatTo.x*scale_to;
			this->y = from.y*scale_from + quatTo.y*scale_to;
			this->z = from.z*scale_from + quatTo.z*scale_to;
			this->w = from.w*scale_from + quatTo.w*scale_to;
		}
		
	//private:
		real w, x, y, z;
	};
		
}	// namespace sora


#endif