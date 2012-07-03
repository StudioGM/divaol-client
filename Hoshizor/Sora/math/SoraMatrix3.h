/*
 *  SoraMatrix3.h
 *  Sora
 *
 *  Created by GriffinBu on 1/20/11.
 *  Copyright 2011 Griffin Bu(Project L). All rights reserved.
 *
 */

#ifndef SORA_2D_MATRIX_H_
#define SORA_2D_MATRIX_H_

#include "SoraPlatform.h"
#include "SoraVector.h"

#include <vector>

namespace sora {
	
	/*
	 just another 2d matrix implemention
	 */
	
	class SORA_API SoraMatrix3 {
	public:
		SoraMatrix3() {
			createIdentity();
		}
        SoraMatrix3(real* v);
		union {
			real c[3][3];
            real x[9];
		};
		        
        void createIdentity();
        void translate(real x, real y);
        void scale(real x, real y);
        void rotate(real rotation);
        void rotate(const SoraVector& fwd, const SoraVector& side);
        void transformVector2Ds(std::vector<SoraVector>& vPoints);
        void transformVector2D(SoraVector& vPoint);
        
        SoraMatrix3 operator*(const SoraMatrix3& rhs);
        SoraMatrix3& operator*=(const SoraMatrix3& rhs);
	};
	
	inline SoraMatrix3::SoraMatrix3(real* v) {
		sora_assert(v);
		for(int i=0; i<9; ++i)
			x[i] = v[i];
	}
	
	inline SoraMatrix3 SoraMatrix3::operator*(const SoraMatrix3& rhs) {
		SoraMatrix3 mat_tmp;
		
		mat_tmp.c[0][0] = (c[0][0]*rhs.c[0][0]) + (c[0][1]*rhs.c[1][0]) + (c[0][2]*rhs.c[2][0]);
		mat_tmp.c[0][0] = (c[0][0]*rhs.c[0][1]) + (c[0][1]*rhs.c[1][1]) + (c[0][2]*rhs.c[2][1]);
		mat_tmp.c[0][0] = (c[0][0]*rhs.c[0][2]) + (c[0][1]*rhs.c[1][1]) + (c[0][2]*rhs.c[2][2]);
		
		mat_tmp.c[1][0] = (c[1][0]*rhs.c[0][0]) + (c[1][1]*rhs.c[1][0]) + (c[1][2]*rhs.c[2][0]);
		mat_tmp.c[1][1] = (c[1][0]*rhs.c[0][1]) + (c[1][1]*rhs.c[1][1]) + (c[1][2]*rhs.c[2][1]);
		mat_tmp.c[1][2] = (c[1][0]*rhs.c[0][2]) + (c[1][1]*rhs.c[1][1]) + (c[1][2]*rhs.c[2][2]);
		
		mat_tmp.c[2][0] = (c[2][0]*rhs.c[0][0]) + (c[2][1]*rhs.c[1][0]) + (c[2][2]*rhs.c[2][0]);
		mat_tmp.c[2][1] = (c[2][0]*rhs.c[0][1]) + (c[2][1]*rhs.c[1][1]) + (c[2][2]*rhs.c[2][1]);
		mat_tmp.c[2][2] = (c[2][0]*rhs.c[0][2]) + (c[2][1]*rhs.c[1][1]) + (c[2][2]*rhs.c[2][2]);
		
        return mat_tmp;
	}
    
    inline SoraMatrix3& SoraMatrix3::operator*=(const SoraMatrix3& rhs) {
		SoraMatrix3 mat_tmp;
		
		mat_tmp.c[0][0] = (c[0][0]*rhs.c[0][0]) + (c[0][1]*rhs.c[1][0]) + (c[0][2]*rhs.c[2][0]);
		mat_tmp.c[0][0] = (c[0][0]*rhs.c[0][1]) + (c[0][1]*rhs.c[1][1]) + (c[0][2]*rhs.c[2][1]);
		mat_tmp.c[0][0] = (c[0][0]*rhs.c[0][2]) + (c[0][1]*rhs.c[1][1]) + (c[0][2]*rhs.c[2][2]);
		
		mat_tmp.c[1][0] = (c[1][0]*rhs.c[0][0]) + (c[1][1]*rhs.c[1][0]) + (c[1][2]*rhs.c[2][0]);
		mat_tmp.c[1][1] = (c[1][0]*rhs.c[0][1]) + (c[1][1]*rhs.c[1][1]) + (c[1][2]*rhs.c[2][1]);
		mat_tmp.c[1][2] = (c[1][0]*rhs.c[0][2]) + (c[1][1]*rhs.c[1][1]) + (c[1][2]*rhs.c[2][2]);
		
		mat_tmp.c[2][0] = (c[2][0]*rhs.c[0][0]) + (c[2][1]*rhs.c[1][0]) + (c[2][2]*rhs.c[2][0]);
		mat_tmp.c[2][1] = (c[2][0]*rhs.c[0][1]) + (c[2][1]*rhs.c[1][1]) + (c[2][2]*rhs.c[2][1]);
		mat_tmp.c[2][2] = (c[2][0]*rhs.c[0][2]) + (c[2][1]*rhs.c[1][1]) + (c[2][2]*rhs.c[2][2]);
		
		*this = mat_tmp;
        return *this;
	}
	
	inline void SoraMatrix3::transformVector2Ds(std::vector<SoraVector>& vPoints) {
		for(uint32 i=0; i<vPoints.size(); ++i) {
			real tempX = (c[0][0]*vPoints[i].x) + (c[1][0]*vPoints[i].y) + c[2][0];
			real tempY = (c[0][1]*vPoints[i].x) + (c[1][1]*vPoints[i].y) + c[2][1];
			
			vPoints[i].x = (real)tempX;
			vPoints[i].y = (real)tempY;
		}
	}
	
	inline void SoraMatrix3::transformVector2D(SoraVector& vPoint) {
		real tempX = (c[0][0]*vPoint.x) + (c[1][0]*vPoint.y) + c[2][0];
		real tempY = (c[0][1]*vPoint.x) + (c[1][1]*vPoint.y) + c[2][1];
		
		vPoint.x = (real)tempX;
		vPoint.y = (real)tempY;
	}
	
	inline void SoraMatrix3::createIdentity() {
		c[0][0] = 1; c[0][1] = 0; c[0][2] = 0;
		c[1][0] = 0; c[1][1] = 1; c[1][2] = 0;
		c[2][0] = 0; c[2][2] = 0; c[2][2] = 1;
	}
	
	inline void SoraMatrix3::translate(real x, real y) {
		SoraMatrix3 mat;
		
		mat.c[0][0] = 1; mat.c[0][1] = 0; mat.c[0][2] = 0;
		mat.c[1][0] = 0; mat.c[1][1] = 1; mat.c[1][2] = 0;
		mat.c[2][0] = x; mat.c[2][2] = y; mat.c[2][2] = 1;
		
		*this *= mat;
	}
	
	inline void SoraMatrix3::scale(real xscale, real yscale) {
		SoraMatrix3 mat;
		
		mat.c[0][0] = xscale; mat.c[0][1] = 0; mat.c[0][2] = 0;
		mat.c[1][0] = 0; mat.c[1][1] = yscale; mat.c[1][2] = 0;
		mat.c[2][0] = 0; mat.c[2][2] = 0; mat.c[2][2] = 1;
		
		*this *= mat;
	}
	
	inline void SoraMatrix3::rotate(real rot) {
		SoraMatrix3 mat;
		
		real sinv = sin(rot);
		real cosv = cos(rot);
		
		mat.c[0][0] = cosv; mat.c[0][1] = sinv; mat.c[0][2] = 0;
		mat.c[1][0] = -sinv; mat.c[1][1] = cosv; mat.c[1][2] = 0;
		mat.c[2][0] = 0; mat.c[2][2] = 0; mat.c[2][2] = 1;
		
		*this *= mat;
	}
	
	inline void SoraMatrix3::rotate(const SoraVector& fwd, const SoraVector& side) {
		SoraMatrix3 mat;
		
		mat.c[0][0] = fwd.x; mat.c[0][1] = fwd.y; mat.c[0][2] = 0;
		mat.c[1][0] = side.x; mat.c[1][1] = side.y; mat.c[1][2] = 0;
		mat.c[2][0] = 0; mat.c[2][2] = 0; mat.c[2][2] = 1;
		
		*this *= mat;
	}
    
    typedef SoraMatrix3 Matrix3x3;
    
} // namepsace sora

#endif // SORA_2D_MATRIX_H_