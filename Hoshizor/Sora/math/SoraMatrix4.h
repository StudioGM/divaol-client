//
//  SoraMatrix4x4.h
//  Sora
//
//  Created by Ruiwei Bu on 10/7/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraMatrix4x4_h
#define Sora_SoraMatrix4x4_h

#include "SoraPlatform.h"
#include "SoraVector4.h"
#include "SoraQuaternion.h"

#include "SoraMathCommon.h"

namespace sora {
    
    class SORA_API SoraMatrix4 {
    public:
        union {
            real c[4][4];	// Column major order for OpenGL: c[column][row]
            real x[16];
        };
        
        static SoraMatrix4 TransMat( real x, real y, real z ) {
            SoraMatrix4 m;
            
            m.c[3][0] = x;
            m.c[3][1] = y;
            m.c[3][2] = z;
            
            return m;
        }
        
        static SoraMatrix4 ScaleMat( real x, real y, real z ) {
            SoraMatrix4 m;
            
            m.c[0][0] = x;
            m.c[1][1] = y;
            m.c[2][2] = z;
            
            return m;
        }
        
        static SoraMatrix4 RotMat( real x, real y, real z ) {
            // Rotation order: YXZ [* Vector]
            return SoraMatrix4( SoraQuaternion( x, y, z ) );
        }
        
        static SoraMatrix4 RotMat( SoraVector3 axis, real angle ) {
            axis = axis * sinf( angle * 0.5f );
            return SoraMatrix4( SoraQuaternion( axis.x, axis.y, axis.z, cosf( angle * 0.5f ) ) );
        }
        
        static SoraMatrix4 FrustumMat( real l, real r, real b, real t, real n, real f ) {
            SoraMatrix4 m;
            
            m.x[0] = 2 * n / (r - l);
            m.x[5] = 2 * n / (t - b);
            m.x[8] = (r + l) / (r - l);
            m.x[9] = (t + b) / (t - b);
            m.x[10] = -(f + n) / (f - n);
            m.x[11] = -1;
            m.x[14] = -2 * f * n / (f - n);
            
            return m;
        }
        
        static SoraMatrix4 PerspectiveMat(real field_of_view, real aspect, real n, real f) {
            SoraMatrix4 m;
            real ff = 1.0f / tan(DGR_RAD(field_of_view) / 2.0f);
            m.x[0] = ff / aspect;
            m.x[5] = ff;
            m.x[10] = (n + f) / (n - f);
            m.x[14] = (2.0f * f * n) / (n - f);
            m.x[11] = -1.f;
            return m;
        }
        
        static SoraMatrix4 OrthoMat( real l, real r, real b, real t, real n, real f ) {
            SoraMatrix4 m;
            
            m.x[0] = 2 / (r - l);
            m.x[5] = 2 / (t - b);
            m.x[10] = -2 / (f - n);
            m.x[12] = -(r + l) / (r - l);
            m.x[13] = -(t + b) / (t - b);
            m.x[14] = -(f + n) / (f - n);
            
            return m;
        }
        
        static SoraMatrix4 FastMult43( const SoraMatrix4 &m1, const SoraMatrix4 &m2 ) {
            // Note: dst may not be the same as m1 or m2
            SoraMatrix4 dst;
            
            real *dstx = dst.x;
            const real *m1x = m1.x;
            const real *m2x = m2.x;
            
            dstx[0] = m1x[0] * m2x[0] + m1x[4] * m2x[1] + m1x[8] * m2x[2];
            dstx[1] = m1x[1] * m2x[0] + m1x[5] * m2x[1] + m1x[9] * m2x[2];
            dstx[2] = m1x[2] * m2x[0] + m1x[6] * m2x[1] + m1x[10] * m2x[2];
            dstx[3] = 0.0f;
            
            dstx[4] = m1x[0] * m2x[4] + m1x[4] * m2x[5] + m1x[8] * m2x[6];
            dstx[5] = m1x[1] * m2x[4] + m1x[5] * m2x[5] + m1x[9] * m2x[6];
            dstx[6] = m1x[2] * m2x[4] + m1x[6] * m2x[5] + m1x[10] * m2x[6];
            dstx[7] = 0.0f;
            
            dstx[8] = m1x[0] * m2x[8] + m1x[4] * m2x[9] + m1x[8] * m2x[10];
            dstx[9] = m1x[1] * m2x[8] + m1x[5] * m2x[9] + m1x[9] * m2x[10];
            dstx[10] = m1x[2] * m2x[8] + m1x[6] * m2x[9] + m1x[10] * m2x[10];
            dstx[11] = 0.0f;
            
            dstx[12] = m1x[0] * m2x[12] + m1x[4] * m2x[13] + m1x[8] * m2x[14] + m1x[12] * m2x[15];
            dstx[13] = m1x[1] * m2x[12] + m1x[5] * m2x[13] + m1x[9] * m2x[14] + m1x[13] * m2x[15];
            dstx[14] = m1x[2] * m2x[12] + m1x[6] * m2x[13] + m1x[10] * m2x[14] + m1x[14] * m2x[15];
            dstx[15] = 1.0f;
            
            return dst;
        }
        
        // ------------
        // Constructors
        // ------------
        SoraMatrix4() {
            c[0][0] = 1.f; c[1][0] = 0.f; c[2][0] = 0.f; c[3][0] = 0.f;
            c[0][1] = 0.f; c[1][1] = 1.f; c[2][1] = 0.f; c[3][1] = 0.f;
            c[0][2] = 0.f; c[1][2] = 0.f; c[2][2] = 1.f; c[3][2] = 0.f;
            c[0][3] = 0.f; c[1][3] = 0.f; c[2][3] = 0.f; c[3][3] = 1.f;
        }
        
        SoraMatrix4( const real *realArray16 ) {
            for( unsigned int i = 0; i < 4; ++i ) {
                for( unsigned int j = 0; j < 4; ++j ) {
                    c[i][j] = realArray16[i * 4 + j];
                }
            }
        }
        
        SoraMatrix4( const SoraQuaternion &q ) {
            // Calculate coefficients
            real x2 = q.x + q.x, y2 = q.y + q.y, z2 = q.z + q.z;
            real xx = q.x * x2,  xy = q.x * y2,  xz = q.x * z2;
            real yy = q.y * y2,  yz = q.y * z2,  zz = q.z * z2;
            real wx = q.w * x2,  wy = q.w * y2,  wz = q.w * z2;
            
            c[0][0] = 1 - (yy + zz);  c[1][0] = xy - wz;	
            c[2][0] = xz + wy;        c[3][0] = 0;
            c[0][1] = xy + wz;        c[1][1] = 1 - (xx + zz);
            c[2][1] = yz - wx;        c[3][1] = 0;
            c[0][2] = xz - wy;        c[1][2] = yz + wx;
            c[2][2] = 1 - (xx + yy);  c[3][2] = 0;
            c[0][3] = 0;              c[1][3] = 0;
            c[2][3] = 0;              c[3][3] = 1;
        }
        
        // ----------
        // Matrix sum
        // ----------
        SoraMatrix4 operator+( const SoraMatrix4 &m ) const {
            SoraMatrix4 mf;
            
            mf.x[0] = x[0] + m.x[0];
            mf.x[1] = x[1] + m.x[1];
            mf.x[2] = x[2] + m.x[2];
            mf.x[3] = x[3] + m.x[3];
            mf.x[4] = x[4] + m.x[4];
            mf.x[5] = x[5] + m.x[5];
            mf.x[6] = x[6] + m.x[6];
            mf.x[7] = x[7] + m.x[7];
            mf.x[8] = x[8] + m.x[8];
            mf.x[9] = x[9] + m.x[9];
            mf.x[10] = x[10] + m.x[10];
            mf.x[11] = x[11] + m.x[11];
            mf.x[12] = x[12] + m.x[12];
            mf.x[13] = x[13] + m.x[13];
            mf.x[14] = x[14] + m.x[14];
            mf.x[15] = x[15] + m.x[15];
            
            return mf;
        }
        
        SoraMatrix4 &operator+=( const SoraMatrix4 &m ) {
            return *this = *this + m;
        }
        
        // ---------------------
        // Matrix multiplication
        // ---------------------
        SoraMatrix4 operator*( const SoraMatrix4 &m ) const  {
            SoraMatrix4 mf;
            
            mf.x[0] = x[0] * m.x[0] + x[4] * m.x[1] + x[8] * m.x[2] + x[12] * m.x[3];
            mf.x[1] = x[1] * m.x[0] + x[5] * m.x[1] + x[9] * m.x[2] + x[13] * m.x[3];
            mf.x[2] = x[2] * m.x[0] + x[6] * m.x[1] + x[10] * m.x[2] + x[14] * m.x[3];
            mf.x[3] = x[3] * m.x[0] + x[7] * m.x[1] + x[11] * m.x[2] + x[15] * m.x[3];
            
            mf.x[4] = x[0] * m.x[4] + x[4] * m.x[5] + x[8] * m.x[6] + x[12] * m.x[7];
            mf.x[5] = x[1] * m.x[4] + x[5] * m.x[5] + x[9] * m.x[6] + x[13] * m.x[7];
            mf.x[6] = x[2] * m.x[4] + x[6] * m.x[5] + x[10] * m.x[6] + x[14] * m.x[7];
            mf.x[7] = x[3] * m.x[4] + x[7] * m.x[5] + x[11] * m.x[6] + x[15] * m.x[7];
            
            mf.x[8] = x[0] * m.x[8] + x[4] * m.x[9] + x[8] * m.x[10] + x[12] * m.x[11];
            mf.x[9] = x[1] * m.x[8] + x[5] * m.x[9] + x[9] * m.x[10] + x[13] * m.x[11];
            mf.x[10] = x[2] * m.x[8] + x[6] * m.x[9] + x[10] * m.x[10] + x[14] * m.x[11];
            mf.x[11] = x[3] * m.x[8] + x[7] * m.x[9] + x[11] * m.x[10] + x[15] * m.x[11];
            
            mf.x[12] = x[0] * m.x[12] + x[4] * m.x[13] + x[8] * m.x[14] + x[12] * m.x[15];
            mf.x[13] = x[1] * m.x[12] + x[5] * m.x[13] + x[9] * m.x[14] + x[13] * m.x[15];
            mf.x[14] = x[2] * m.x[12] + x[6] * m.x[13] + x[10] * m.x[14] + x[14] * m.x[15];
            mf.x[15] = x[3] * m.x[12] + x[7] * m.x[13] + x[11] * m.x[14] + x[15] * m.x[15];
            
            return mf;
        }
        
        SoraMatrix4 operator*( const real f ) const {
            SoraMatrix4 m( *this );
            
            m.x[0]  *= f; m.x[1]  *= f; m.x[2]  *= f; m.x[3]  *= f;
            m.x[4]  *= f; m.x[5]  *= f; m.x[6]  *= f; m.x[7]  *= f;
            m.x[8]  *= f; m.x[9]  *= f; m.x[10] *= f; m.x[11] *= f;
            m.x[12] *= f; m.x[13] *= f; m.x[14] *= f; m.x[15] *= f;
            
            return m;
        }
        
        // ----------------------------
        // Vector-Matrix multiplication
        // ----------------------------
        SoraVector3 operator*( const SoraVector3 &v ) const {
            return SoraVector3( v.x * c[0][0] + v.y * c[1][0] + v.z * c[2][0] + c[3][0],
                         v.x * c[0][1] + v.y * c[1][1] + v.z * c[2][1] + c[3][1],
                         v.x * c[0][2] + v.y * c[1][2] + v.z * c[2][2] + c[3][2] );
        }
        
        SoraVector4 operator*( const SoraVector4 &v ) const {
            return SoraVector4( v.x * c[0][0] + v.y * c[1][0] + v.z * c[2][0] + v.w * c[3][0],
                         v.x * c[0][1] + v.y * c[1][1] + v.z * c[2][1] + v.w * c[3][1],
                         v.x * c[0][2] + v.y * c[1][2] + v.z * c[2][2] + v.w * c[3][2],
                         v.x * c[0][3] + v.y * c[1][3] + v.z * c[2][3] + v.w * c[3][3] );
        }
        
        SoraVector3 mult33Vec( const SoraVector3 &v ) const {
            return SoraVector3( v.x * c[0][0] + v.y * c[1][0] + v.z * c[2][0],
                               v.x * c[0][1] + v.y * c[1][1] + v.z * c[2][1],
                               v.x * c[0][2] + v.y * c[1][2] + v.z * c[2][2] );
        }
        
        // ---------------
        // Transformations
        // ---------------
        SoraMatrix4& translate( const real x, const real y, const real z ) {
            *this = TransMat( x, y, z ) * *this;
            return *this;
        }
        
        SoraMatrix4& scale( const real x, const real y, const real z ) {
            *this = ScaleMat( x, y, z ) * *this;
            return *this;
        }
        
        SoraMatrix4& rotate( const real x, const real y, const real z ) {
            *this = RotMat( x, y, z ) * *this;
            return *this;
        }
        
        // ---------------
        // Other
        // ---------------
        
        SoraMatrix4 transposed() const {
            SoraMatrix4 m( *this );
            
            for( unsigned int y = 0; y < 4; ++y ) {
                for( unsigned int x = y + 1; x < 4; ++x ) {
                    real tmp = m.c[x][y];
                    m.c[x][y] = m.c[y][x];
                    m.c[y][x] = tmp;
                }
            }
            
            return m;
        }
        
        real determinant() const {
            return 
			c[0][3]*c[1][2]*c[2][1]*c[3][0] - c[0][2]*c[1][3]*c[2][1]*c[3][0] - c[0][3]*c[1][1]*c[2][2]*c[3][0] + c[0][1]*c[1][3]*c[2][2]*c[3][0] +
			c[0][2]*c[1][1]*c[2][3]*c[3][0] - c[0][1]*c[1][2]*c[2][3]*c[3][0] - c[0][3]*c[1][2]*c[2][0]*c[3][1] + c[0][2]*c[1][3]*c[2][0]*c[3][1] +
			c[0][3]*c[1][0]*c[2][2]*c[3][1] - c[0][0]*c[1][3]*c[2][2]*c[3][1] - c[0][2]*c[1][0]*c[2][3]*c[3][1] + c[0][0]*c[1][2]*c[2][3]*c[3][1] +
			c[0][3]*c[1][1]*c[2][0]*c[3][2] - c[0][1]*c[1][3]*c[2][0]*c[3][2] - c[0][3]*c[1][0]*c[2][1]*c[3][2] + c[0][0]*c[1][3]*c[2][1]*c[3][2] +
			c[0][1]*c[1][0]*c[2][3]*c[3][2] - c[0][0]*c[1][1]*c[2][3]*c[3][2] - c[0][2]*c[1][1]*c[2][0]*c[3][3] + c[0][1]*c[1][2]*c[2][0]*c[3][3] +
			c[0][2]*c[1][0]*c[2][1]*c[3][3] - c[0][0]*c[1][2]*c[2][1]*c[3][3] - c[0][1]*c[1][0]*c[2][2]*c[3][3] + c[0][0]*c[1][1]*c[2][2]*c[3][3];
        }
        
        SoraMatrix4 inverted() const {
            SoraMatrix4 m;
            
            real d = determinant();
            if( d == 0 ) return m;
            d = 1.0f / d;
            
            m.c[0][0] = d * (c[1][2]*c[2][3]*c[3][1] - c[1][3]*c[2][2]*c[3][1] + c[1][3]*c[2][1]*c[3][2] - c[1][1]*c[2][3]*c[3][2] - c[1][2]*c[2][1]*c[3][3] + c[1][1]*c[2][2]*c[3][3]);
            m.c[0][1] = d * (c[0][3]*c[2][2]*c[3][1] - c[0][2]*c[2][3]*c[3][1] - c[0][3]*c[2][1]*c[3][2] + c[0][1]*c[2][3]*c[3][2] + c[0][2]*c[2][1]*c[3][3] - c[0][1]*c[2][2]*c[3][3]);
            m.c[0][2] = d * (c[0][2]*c[1][3]*c[3][1] - c[0][3]*c[1][2]*c[3][1] + c[0][3]*c[1][1]*c[3][2] - c[0][1]*c[1][3]*c[3][2] - c[0][2]*c[1][1]*c[3][3] + c[0][1]*c[1][2]*c[3][3]);
            m.c[0][3] = d * (c[0][3]*c[1][2]*c[2][1] - c[0][2]*c[1][3]*c[2][1] - c[0][3]*c[1][1]*c[2][2] + c[0][1]*c[1][3]*c[2][2] + c[0][2]*c[1][1]*c[2][3] - c[0][1]*c[1][2]*c[2][3]);
            m.c[1][0] = d * (c[1][3]*c[2][2]*c[3][0] - c[1][2]*c[2][3]*c[3][0] - c[1][3]*c[2][0]*c[3][2] + c[1][0]*c[2][3]*c[3][2] + c[1][2]*c[2][0]*c[3][3] - c[1][0]*c[2][2]*c[3][3]);
            m.c[1][1] = d * (c[0][2]*c[2][3]*c[3][0] - c[0][3]*c[2][2]*c[3][0] + c[0][3]*c[2][0]*c[3][2] - c[0][0]*c[2][3]*c[3][2] - c[0][2]*c[2][0]*c[3][3] + c[0][0]*c[2][2]*c[3][3]);
            m.c[1][2] = d * (c[0][3]*c[1][2]*c[3][0] - c[0][2]*c[1][3]*c[3][0] - c[0][3]*c[1][0]*c[3][2] + c[0][0]*c[1][3]*c[3][2] + c[0][2]*c[1][0]*c[3][3] - c[0][0]*c[1][2]*c[3][3]);
            m.c[1][3] = d * (c[0][2]*c[1][3]*c[2][0] - c[0][3]*c[1][2]*c[2][0] + c[0][3]*c[1][0]*c[2][2] - c[0][0]*c[1][3]*c[2][2] - c[0][2]*c[1][0]*c[2][3] + c[0][0]*c[1][2]*c[2][3]);
            m.c[2][0] = d * (c[1][1]*c[2][3]*c[3][0] - c[1][3]*c[2][1]*c[3][0] + c[1][3]*c[2][0]*c[3][1] - c[1][0]*c[2][3]*c[3][1] - c[1][1]*c[2][0]*c[3][3] + c[1][0]*c[2][1]*c[3][3]);
            m.c[2][1] = d * (c[0][3]*c[2][1]*c[3][0] - c[0][1]*c[2][3]*c[3][0] - c[0][3]*c[2][0]*c[3][1] + c[0][0]*c[2][3]*c[3][1] + c[0][1]*c[2][0]*c[3][3] - c[0][0]*c[2][1]*c[3][3]);
            m.c[2][2] = d * (c[0][1]*c[1][3]*c[3][0] - c[0][3]*c[1][1]*c[3][0] + c[0][3]*c[1][0]*c[3][1] - c[0][0]*c[1][3]*c[3][1] - c[0][1]*c[1][0]*c[3][3] + c[0][0]*c[1][1]*c[3][3]);
            m.c[2][3] = d * (c[0][3]*c[1][1]*c[2][0] - c[0][1]*c[1][3]*c[2][0] - c[0][3]*c[1][0]*c[2][1] + c[0][0]*c[1][3]*c[2][1] + c[0][1]*c[1][0]*c[2][3] - c[0][0]*c[1][1]*c[2][3]);
            m.c[3][0] = d * (c[1][2]*c[2][1]*c[3][0] - c[1][1]*c[2][2]*c[3][0] - c[1][2]*c[2][0]*c[3][1] + c[1][0]*c[2][2]*c[3][1] + c[1][1]*c[2][0]*c[3][2] - c[1][0]*c[2][1]*c[3][2]);
            m.c[3][1] = d * (c[0][1]*c[2][2]*c[3][0] - c[0][2]*c[2][1]*c[3][0] + c[0][2]*c[2][0]*c[3][1] - c[0][0]*c[2][2]*c[3][1] - c[0][1]*c[2][0]*c[3][2] + c[0][0]*c[2][1]*c[3][2]);
            m.c[3][2] = d * (c[0][2]*c[1][1]*c[3][0] - c[0][1]*c[1][2]*c[3][0] - c[0][2]*c[1][0]*c[3][1] + c[0][0]*c[1][2]*c[3][1] + c[0][1]*c[1][0]*c[3][2] - c[0][0]*c[1][1]*c[3][2]);
            m.c[3][3] = d * (c[0][1]*c[1][2]*c[2][0] - c[0][2]*c[1][1]*c[2][0] + c[0][2]*c[1][0]*c[2][1] - c[0][0]*c[1][2]*c[2][1] - c[0][1]*c[1][0]*c[2][2] + c[0][0]*c[1][1]*c[2][2]);
            
            return m;
        }
        
        void decompose( SoraVector3 &trans, SoraVector3 &rot, SoraVector3 &scale ) const {
            // Getting translation is trivial
            trans = SoraVector3( c[3][0], c[3][1], c[3][2] );
            
            // Scale is length of columns
            scale.x = sqrtf( c[0][0] * c[0][0] + c[0][1] * c[0][1] + c[0][2] * c[0][2] );
            scale.y = sqrtf( c[1][0] * c[1][0] + c[1][1] * c[1][1] + c[1][2] * c[1][2] );
            scale.z = sqrtf( c[2][0] * c[2][0] + c[2][1] * c[2][1] + c[2][2] * c[2][2] );
            
            if( scale.x == 0 || scale.y == 0 || scale.z == 0 ) return;
            
            // Detect negative scale with determinant and flip one arbitrary axis
            if( determinant() < 0 ) scale.x = -scale.x;
            
            // Combined rotation matrix YXZ
            //
            // Cos[y]*Cos[z]+Sin[x]*Sin[y]*Sin[z]   Cos[z]*Sin[x]*Sin[y]-Cos[y]*Sin[z]  Cos[x]*Sin[y]	
            // Cos[x]*Sin[z]                        Cos[x]*Cos[z]                       -Sin[x]
            // -Cos[z]*Sin[y]+Cos[y]*Sin[x]*Sin[z]  Cos[y]*Cos[z]*Sin[x]+Sin[y]*Sin[z]  Cos[x]*Cos[y]
            
            rot.x = asinf( -c[2][1] / scale.z );
            
            // Special case: Cos[x] == 0 (when Sin[x] is +/-1)
            real f = fabsf( c[2][1] / scale.z );
            if( f > 0.999f && f < 1.001f ) {
                // Pin arbitrarily one of y or z to zero
                // Mathematical equivalent of gimbal lock
                rot.y = 0;
                
                // Now: Cos[x] = 0, Sin[x] = +/-1, Cos[y] = 1, Sin[y] = 0
                // => m[0][0] = Cos[z] and m[1][0] = Sin[z]
                rot.z = atan2f( -c[1][0] / scale.y, c[0][0] / scale.x );
            }
            // Standard case
            else {
                rot.y = atan2f( c[2][0] / scale.z, c[2][2] / scale.z );
                rot.z = atan2f( c[0][1] / scale.x, c[1][1] / scale.y );
            }
        }
        
        SoraVector4 getCol( unsigned int col ) const {
            return SoraVector4( x[col * 4 + 0], x[col * 4 + 1], x[col * 4 + 2], x[col * 4 + 3] );
        }
        
        SoraVector4 getRow( unsigned int row ) const {
            return SoraVector4( x[row + 0], x[row + 4], x[row + 8], x[row + 12] );
        }
        
        SoraVector3 getTrans() const {
            return SoraVector3( c[3][0], c[3][1], c[3][2] );
        }
        
        SoraVector3 getScale() const {
            SoraVector3 scale;
            // Scale is length of columns
            scale.x = sqrtf( c[0][0] * c[0][0] + c[0][1] * c[0][1] + c[0][2] * c[0][2] );
            scale.y = sqrtf( c[1][0] * c[1][0] + c[1][1] * c[1][1] + c[1][2] * c[1][2] );
            scale.z = sqrtf( c[2][0] * c[2][0] + c[2][1] * c[2][1] + c[2][2] * c[2][2] );
            return scale;
        }
    };
    
} // namespace sora



#endif
