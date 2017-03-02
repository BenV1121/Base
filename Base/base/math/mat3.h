#pragma once

#include "vec3.h"

namespace base
{
	union mat3
	{
		vec3  c[3];
		struct { vec3 c1, c2, c3; };
		float v[9];
		float m[3][3];

		mat3(float m1, float m2, float m3, float m4, float m5, float m6, float m7, float m8, float m9) : c1(m1, m2, m3), c2(m4, m5, m6), c3(m7, m8, m9) {}
		mat3(const vec3 &a = vec3::right(), const vec3 &b = vec3::up(), const vec3 &c = vec3::forward()) :c1(a), c2(b), c3(c) {}

		      vec3 &operator[](size_t idx)	 	 { return c[idx]; }
		const vec3 &operator[](size_t idx) const { return c[idx]; }


		mat3 &operator-=(const mat3& O) { return *this = mat3{ c1 - O.c1, c2 - O.c2, c3 - O.c3 }; }
		mat3 &operator+=(const mat3& O) { return *this = mat3{ c1 + O.c1, c2 + O.c2, c3 + O.c3 }; }
		mat3 &operator*=(float s)		{ return *this = mat3{ c1 * s, c2 * s, c3 * s }; }
		mat3 operator-()		  const { return mat3{ -c1, -c2,-c3 }; }

		mat3 transpose() const { return mat3{ v[0],v[3],v[6], v[1],v[4],v[7], v[2],v[5],v[8] }; }

		float determinant() const { return dot(c1, cross(c2, c3)); }

		mat3 inverse() const
		{
			return mat3{ cross(c2, c3), cross(c3, c1), cross(c1, c2) }.transpose() *= (1 / determinant());
		}


		static mat3 translate(const vec2 &p)	{ mat3 r; r.c3.xy = p; return r; }
		static mat3 translate(float x, float y)	{ return translate(vec2{ x,y }); }

		static mat3 scale(const vec2 &d)		{ mat3 r; r.c1 *= d.x; r.c2 *= d.y; return r; }

		static mat3 scale(float w, float h)		{ return scale(vec2{ w,h }); }
		static mat3 scale(float s)				{ return scale(s, s); }

		static mat3 rotate(const vec2 &d)		{ return rotate(d.angle()); }
		static mat3 rotate(float angle)
		{
			mat3 r;
			r.c1.xy = vec2::fromAngle(angle);
			r.c2.xy = r.c1.xy.perp();
			return r;
		}


		vec2  getTrans2D() const { return c3.xy; }
		float getAngle2D() const { return c1.xy.angle(); }
		vec2  getScale2D() const { return vec2{ c1.xy.magnitude(), c2.xy.magnitude() }; }
	};


inline mat3 operator+(mat3 A, const mat3 &B) { return A += B; }

inline mat3 operator-(mat3 A, const mat3 &B) { return A -= B; }

inline mat3 operator*(mat3 A, float s) { return A *= s; }
inline mat3 operator*(float s, mat3 A) { return A *= s; }

inline mat3 operator*(mat3 A, const mat3 &B)
{
	A = A.transpose();

	return mat3{
	dot(A.c1, B.c1), dot(A.c2, B.c1), dot(A.c3, B.c1),
	dot(A.c1, B.c2), dot(A.c2, B.c2), dot(A.c3, B.c2),
	dot(A.c1, B.c3), dot(A.c2, B.c3), dot(A.c3, B.c3)
	};
}

inline vec3 operator*(mat3 A, const vec3 &v)
{
	A = A.transpose();
	return vec3{dot(A.c1,v), dot(A.c2,v), dot(A.c3,v)};
}

inline bool operator==(const mat3 &A, const mat3 &B) { return A.c1 == B.c1 && A.c2 == B.c2 && A.c3 == B.c3; }
inline bool operator!=(const mat3 &A, const mat3 &B) { return !(A == B); }
}

