/****************************************************
 *  Math.h
 *
 *  Created by tempbuffer on 9/19/12.
 *  Copyright 2012 tempbuffer. All rights reserved.
 *
 *  Several Classes about Math and other utilities
 ****************************************************/

#ifndef _BASE_MATH_H_
#define _BASE_MATH_H_

#include "Type.h"
#include "Ptr.h"
#include <iostream>

namespace Base
{
	namespace Math {
		using std::min;
		using std::max;

		typedef float real;

		const real eps = 1e-6f;
		const real pi = 3.1415926f;
		const real e = 2.7182812f;

		const double d_eps = 1e-10;
		const double d_pi = 3.1415926535898;
		const double d_e = 2.718281828459;

		class Util {
		public:
			// By Join Carmack
			static float FastInvSqrt(float x);
			static float FastSqrt(float x) {return 1/FastInvSqrt(x);}
			
			static inline double Sqrt(double x) {return sqrt(x);}
			static inline float Sqrt(float x) {return sqrtf(x);}
			template<typename T>
			static inline T Abs(T x) {return x>=0?x:-x;}

			static inline bool Equal(float x, float y) {
				return Abs(x-y)<eps;
			}
			static inline bool Equal(double x, double y) {
				return Abs(x-y)<d_eps;
			}

			template<typename T>
			inline static T Min(const T &a, const T &b) {
				return a<b?a:b;
			}
			template<typename T>
			inline static T Max(const T &a, const T &b) {
				return a>b?a:b;
			}
		};

		class Vector2 {
		public:
			Vector2():x(0),y(0) {}
			Vector2(real x, real y):x(x),y(y) {}

			void set(real _x, real _y) {
				x = _x, y = _y;
			}

			inline real mod2() const {return x*x+y*y;}
			inline real mod() const {return Util::Sqrt(x*x+y*y);}
			inline real length() const {
				return mod();
			}
			inline Vector2 unit() const {
				real len = mod(); 
				return Vector2(x/len,y/len);
			}
			inline Vector2 normal() const {
				return Vector2(y,-x);
			}
			inline Vector2 negate() const {
				return Vector2(-x,-y);
			}
			inline Vector2 rotate(real angle) const {
				Vector2 v;
				v.x = x * cos(angle) - y * sin(angle);
				v.y = x * sin(angle) + y * cos(angle);
				return v;
			}
			real angle() const {
				return atan2f(y,x);
			}
			real angle(const Vector2 &rhs) {
				return acosf(this->normal()*rhs.normal());
			}
			Vector2& normalize() {
				*this = normal();
				return *this;
			}

			friend real cross(const Vector2 &a, const Vector2 &b) {
				return a.x*b.y-b.x*a.y;
			}
			friend real dot(const Vector2 &a, const Vector2 &b) {
				return a.x*b.x+a.y*b.y;
			}
			friend bool operator==(const Vector2 &a, const Vector2 &b) {
				return Util::Abs(a.x-b.x)<eps&&Util::Abs(a.y-b.y)<eps;
			}
			friend bool operator!=(const Vector2 &a, const Vector2 &b) {
				return !(a==b);
			}
			friend Vector2 operator+(const Vector2 &a, const Vector2 &b) {
				return Vector2(a.x+b.x, a.y+b.y);
			}
			friend Vector2 operator-(const Vector2 &a, const Vector2 &b) {
				return Vector2(a.x-b.x, a.y-b.y);
			}
			friend real operator*(const Vector2 &a, const Vector2 &b) {
				return dot(a,b);
			}
			friend Vector2 operator*(const Vector2 &a, real b) {
				return Vector2(a.x*b, a.y*b);
			}
			friend Vector2 operator*(real a, const Vector2 &b) {
				return Vector2(b.x*a, b.y*a);
			}
			friend Vector2 operator/(const Vector2 &a, real b) {
				return Vector2(a.x/b,a.y/b);
			}
			Vector2 operator-() {
				return negate();
			}
			Vector2& operator+=(const Vector2 &rhs) {
				*this = *this+rhs;
				return *this;
			}
			Vector2& operator-=(const Vector2 &rhs) {
				*this = *this-rhs;
				return *this;
			}
			Vector2& operator*=(real rhs) {
				*this = *this*rhs;
				return *this;
			}
			Vector2& operator/=(real rhs) {
				*this = *this/rhs;
				return *this;
			}

		public:
			real x, y;
		};
		typedef Vector2 Vector;

		class Vector3 {
		public:
			Vector3():x(0),y(0),z(0) {}
			Vector3(real x, real y, real z):x(x),y(y),z(z) {}

			void set(real _x, real _y, real _z) {
				x = _x, y = _y, z = _z;
			}

			inline real mod2() const {return x*x+y*y+z*z;}
			inline real mod() const {return Util::Sqrt(x*x+y*y+z*z);}
			inline real length() const {
				return mod();
			}
			inline Vector3 unit() const {
				real len = mod(); 
				return Vector3(x/len,y/len,z/len);
			}
			inline Vector3 normal() const {
				return Vector3(y,-x,0);
			}
			inline Vector3 negate() const {
				return Vector3(-x,-y,-z);
			}
			Vector3& normalize() {
				*this = normal();
				return *this;
			}
			inline bool zero() const {
				return Util::Abs(x)<eps&&Util::Abs(y)<eps;
			}

			friend Vector3 cross(const Vector3 &a, const Vector3 &b) {
				return Vector3(a.y*b.z-b.y*a.z, a.z*b.x-b.z*a.x, a.x*b.y-b.x*a.y);
			}
			friend real dot(const Vector3 &a, const Vector3 &b) {
				return a.x*b.x+a.y*b.y+a.z*b.z;
			}
			operator bool() const {
				return zero();
			}
			friend bool operator==(const Vector3 &a, const Vector3 &b) {
				return Util::Abs(a.x-b.x)<eps&&Util::Abs(a.y-b.y)<eps&&Util::Abs(a.z-b.z);
			}
			friend bool operator!=(const Vector3 &a, const Vector3 &b) {
				return !(a==b);
			}
			friend Vector3 operator+(const Vector3 &a, const Vector3 &b) {
				return Vector3(a.x+b.x, a.y+b.y, a.z+b.z);
			}
			friend Vector3 operator-(const Vector3 &a, const Vector3 &b) {
				return Vector3(a.x-b.x, a.y-b.y, a.z-b.z);
			}
			friend real operator*(const Vector3 &a, const Vector3 &b) {
				return dot(a,b);
			}
			friend Vector3 operator*(const Vector3 &a, real b) {
				return Vector3(a.x*b, a.y*b, a.z*b);
			}
			friend Vector3 operator*(real a, const Vector3 &b) {
				return Vector3(b.x*a, b.y*a, b.z*a);
			}
			friend Vector3 operator/(const Vector3 &a, real b) {
				return Vector3(a.x/b,a.y/b,a.z/b);
			}
			Vector3 operator-() {
				return negate();
			}
			Vector3& operator+=(const Vector3 &rhs) {
				*this = *this+rhs;
				return *this;
			}
			Vector3& operator-=(const Vector3 &rhs) {
				*this = *this-rhs;
				return *this;
			}
			Vector3& operator*=(real rhs) {
				*this = *this*rhs;
				return *this;
			}
			Vector3& operator/=(real rhs) {
				*this = *this/rhs;
				return *this;
			}

		public:
			real x, y, z;
		};

		class Rectangle {
		public:
			Rectangle():x(0),y(0),w(0),h(0) {}
			Rectangle(real x, real y, real w, real h):
				x(x),y(y),w(w),h(h) {}
			Rectangle(const Vector2 &a, const Vector2 &b):
				x(a.x),y(a.y),w(b.x-a.x),h(b.y-a.y) {}
			static Rectangle MakeWithPoints(real x1, real y1, real x2, real y2) {
				return Rectangle(x1,y1,x2-x1,y2-y1);
			}

			inline real x1() const{return x;}
			inline real x2() const{return x+w;}
			inline real y1() const{return y;}
			inline real y2() const{return y+h;}
			inline real halfw() const {return w/2;}
			inline real halfh() const {return h/2;}
			inline Vector2 center() const {return Vector2(x+halfw(),y+halfh());}
			inline Vector2 upperLeft() const {return Vector2(x,y);}
			inline Vector2 lowerRight() const {return Vector2(x+w,y+h);}
			inline bool empty() const {return w<eps&&h<eps;}
			Rectangle& encapsulate(const Vector2& pos) {
				if(pos.x>x1()) x = pos.x;
				if(pos.x<x2()) w = pos.x-x;
				if(pos.y>y1()) y = pos.y;
				if(pos.y<y2()) h = pos.y-y;
				return *this;
			}

			operator bool() {
				return !empty();
			}
			bool operator!() {
				return empty();
			}
			friend bool operator==(const Rectangle &a, const Rectangle &b) {
				return a.upperLeft()==b.upperLeft()&&a.lowerRight()==b.lowerRight();
			}
			friend Rectangle operator+(const Rectangle &a, const Rectangle &b) {
				return Union(a,b);
			}
			friend Rectangle operator*(const Rectangle &a, const Rectangle &b) {
				return Intersect(a,b);
			}
			Rectangle& operator+=(const Rectangle &rhs) {
				*this = Union(*this, rhs);
				return *this;
			}
			Rectangle& operator*=(const Rectangle &rhs) {
				*this = Intersect(*this, rhs);
				return *this;
			}

			inline bool check(const Rectangle &rhs, bool edge = true) const {
				return Check(*this, rhs, edge);
			}
			inline bool contains(real x, real y) const {
				return x>x1()-eps&&x<x2()+eps
					&&y>y1()-eps&&y<y2()+eps;
			}
			inline bool contains(const Vector &pos) const {
				return contains(pos.x,pos.y);
			}
			friend bool Check(const Rectangle &a, const Rectangle &b, bool edge = true) {
				real edgeFactor = edge?eps:-eps;
				return (a.x1()<b.x2()+edgeFactor&&a.x2()>b.x1()-edgeFactor
					&&a.y1()<b.y2()+edgeFactor&&a.y2()>a.y1()-edgeFactor);
			}
			friend Rectangle Intersect(const Rectangle &a, const Rectangle &b) {
				return MakeWithPoints(max(a.x1(),b.x1()),max(a.y1(),b.y1()),min(a.x2(),b.x2()),min(a.y2(),b.y2()));
			}
			friend Rectangle Union(const Rectangle &a, const Rectangle &b) {
				return MakeWithPoints(min(a.x1(),b.x1()),min(a.y1(),b.y1()),max(a.x2(),b.x2()),max(a.y2(),b.y2()));
			}
			
			public:
				real x,y,w,h;
		};
		typedef Rectangle AABB2;

		class Complex {
		public:
			Complex():a(0),b(0) {}
			Complex(real a, real b):a(a),b(b) {}
			Complex(const Vector &pos):a(pos.x),b(pos.y) {}

			inline bool zero() const {
				return Util::Abs(a)<eps&&Util::Abs(b)<eps;
			}
			inline real mod2() const {
				return a*a+b*b;
			}
			inline real mod() const {
				return Util::Sqrt(a*a+b*b);
			}
			Complex conjugate() const {
				return Complex(a,-b);
			}

			operator bool() const {return !zero();}
			Complex operator-() {
				return Complex(-a,-b);
			}
			friend Complex operator+(const Complex &a, const Complex &b) {
				return Complex(a.a+b.a, a.b+b.b);
			}
			friend Complex operator-(const Complex &a, const Complex &b) {
				return Complex(a.a-b.a,a.b-b.b);
			}
			friend Complex operator*(const Complex &a, real b) {
				return Complex(a.a*b, a.b*b);
			}
			friend Complex operator/(const Complex &a, real b) {
				return Complex(a.a/b, a.b/b);
			}
			friend Complex operator*(const Complex &a, const Complex &b) {
				return Complex(a.a*b.a-a.b*b.b, a.b*b.a+a.a*b.b);
			}
			friend Complex operator/(const Complex &a, const Complex &b) {
				return a*b.conjugate()/b.mod();
			}
			Complex& operator+=(const Complex &rhs) {
				*this = *this + rhs;
				return *this;
			}
			Complex& operator-=(const Complex &rhs) {
				*this = *this + rhs;
				return *this;
			}
			Complex& operator*=(const Complex &rhs) {
				*this = *this * rhs;
				return *this;
			}
			Complex& operator/=(const Complex &rhs) {
				*this = *this / rhs;
				return *this;
			}
			Complex& operator*=(real rhs) {
				*this = *this * rhs;
				return *this;
			}
			Complex& operator/=(real rhs) {
				*this = *this / rhs;
				return *this;
			}

		public:
			real a,b;
		};

		class Quaternion {
		public:
			Quaternion():
				w(0), x(0), y(0), z(0) {}
			Quaternion(real _w, real _x ,real _y, real _z):
				w(_w), x(_x), y(_y), z(_z) {}
			Quaternion(real roll, real pitch, real yaw) {
				*this = MakeRotate(roll, pitch, yaw);
			}
			Quaternion(const Vector3 &pos):
				w(0), x(pos.x), y(pos.y),z(pos.z) {
			}
			Quaternion(real angle, const Vector3 &n) {
				*this = MakeRotate(angle, n);
			}

			void set(real _w, real _x, real _y, real _z) {
				w = _w, x = _x, y = _y, z = _z;
			}
			Vector3 get() const {
				return Vector3(x, y, z);
			}
			bool zero() const {
				return mod2()<eps;
			}
			Quaternion negate() const {
				return Quaternion(-w, -x, -y, -z);
			}
			Quaternion conjugate() const {
				return Quaternion(w, -x, -y, -z);
			}
			Quaternion inverse() const {
				return conjugate() / mod();
			}
			real mod2() const {
				return x*x+y*y+z*z+w*w;
			}
			real mod() const {
				return Util::Sqrt(mod2());
			}
			Vector3 rotate(const Vector3 &pos) {
				return (inverse() * Quaternion(pos) * (*this)).get();
			}
			void getRotate(real &angle, Vector3 &n) {
				real  half_sin = Util::Sqrt(get().mod());
				angle = 2.f * atan2(half_sin, w);

				if(Util::Abs(half_sin)>eps) {
					n = get() / half_sin;
				}
				else
					n = Vector3();
 			}

 			static real Dot(const Quaternion &a, const Quaternion &b) {
 				return a.w+b.w+a.x*b.x+a.y*b.y+a.z*b.z;
 			}
			static Quaternion MakeRotate(real angle, Vector3 n) {
				n.normalize();
				real half_cos = cos(0.5f*angle);
				real half_sin = sin(0.5f*angle);

				return Quaternion(half_cos, n.x*half_sin, n.y*half_sin, n.z*half_sin);
			}
			static Quaternion MakeRotate(real roll, real pitch, real yaw) {
				Quaternion _roll(cos(roll*0.5f), sin(roll*0.5f), 0, 0);
				Quaternion _pitch(cos(pitch*0.5f), 0, sin(pitch*0.5f), 0);
				Quaternion _yaw(cos(yaw*0.5f), 0, 0, sin(yaw*0.5f));

				// y * x * z
				return _pitch * _roll * _yaw;
			}
			static Quaternion Slerp(const Quaternion &from, const Quaternion &to, real t) {
				real cosOmega = Dot(from,to);

				Quaternion _to = to;

				if(cosOmega < 0.0f) {
					_to = -_to;
					cosOmega = -cosOmega;
				}
				real k0, k1;
				if(cosOmega > 0.9999f) {
					k0 = 1.f - t;
					k1 = t;
				}
				else {
					real sinOmega = sqrt(1 - cosOmega*cosOmega);
					real omega = atan2(sinOmega, cosOmega);
					real oneOverSinOmega = 1.0f / sinOmega;

					k0 = sin((1.f-t) * omega) * oneOverSinOmega;
					k1 = sin(t * omega) * oneOverSinOmega;
				}

				return from * k0 + _to * k1;
			}
			static Quaternion Squad(const Quaternion &q0, const Quaternion &q1, const Quaternion &s0, const Quaternion &s1, real h) {
				return Slerp(
					Slerp(q0,q1,h),
					Slerp(s0,s1,h),
					2*h*(1-h));
			}

			operator bool() {
				return !zero();
			}
			Quaternion operator-() const {
				return Quaternion(-w,-x,-y,-z);
			}
			friend Quaternion operator+(const Quaternion &a, const Quaternion &b) {
				return Quaternion(a.w+b.w, a.x+b.x, a.y+b.y, a.z+b.z);
			}
			friend Quaternion operator-(const Quaternion &a, const Quaternion &b) {
				return Quaternion(a.w-b.w, a.x-b.x, a.y-b.y, a.z-b.z);
			}
			friend Quaternion operator*(const Quaternion &a, real b) {
				return Quaternion(a.w*b, a.x*b, a.y*b, a.z*b);
			}
			friend Quaternion operator/(const Quaternion &a, real b) {
				return Quaternion(a.w/b, a.x/b, a.y/b, a.z/b);
			}
			friend Quaternion operator*(const Quaternion &a, const Quaternion &b) {
				return Quaternion(
					a.w*b.w-a.x*b.x-a.y*b.y-a.z*b.z,
					a.w*b.x+a.x*b.w+a.y*b.z-a.z*b.y,
					a.w*b.y+a.y*b.w+a.z*b.x-a.x*b.z,
					a.w*b.z+a.z*b.w+a.x*b.y-a.y*b.x);
			}
			friend Quaternion operator/(const Quaternion &a, const Quaternion &b) {
				return b.inverse()*a;
			}
		public:
			real w, x, y, z;
		};

		class Matrix4 {
		public:
			static const uint32 Dimension = 4;
			static const uint32 DataSize = Dimension*Dimension;

			union {
				real v[4][4];
				real data[16];
			};

			Matrix4() {
				memset(data,0,sizeof(int32)*DataSize);
			}
			Matrix4(const real *array16) {
				for(uint32 i = 0; i < DataSize; i++)
					data[i] = array16[i];
			}
			Matrix4(const Quaternion &q) {

			}
		};
		typedef Matrix4 Matrix;

		template<typename VectorType>
		class Line {
		public:
			VectorType a,b;
		};
		class Segment;
		class Circle;
		class Sphere;
	}
}

#endif