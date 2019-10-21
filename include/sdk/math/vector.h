#ifndef SDK_VECTOR_H
#define SDK_VECTOR_H
#pragma once

#include "math_pfns.h"

class Vector
{
public:
	vec_t x, y, z;
	
	Vector();
	Vector(vec_t X, vec_t Y, vec_t Z);
	
	void Init(vec_t ix = 0.0f, vec_t iy = 0.0f, vec_t iz = 0.0f);

	vec_t operator[](int i) const;
	vec_t& operator[](int i);
	
	bool operator==(const Vector& v) const;
	bool operator!=(const Vector& v) const;

	Vector&	operator+=(const Vector &v);
	Vector&	operator-=(const Vector &v);
	Vector&	operator*=(const Vector &v);
	Vector&	operator*=(float s);
	Vector&	operator/=(const Vector &v);
	Vector&	operator/=(float s);	

	void Negate();

	inline vec_t Length() const;
	vec_t LengthSqr() const;
	
	vec_t DistTo(const Vector &vOther) const;
	vec_t DistToSqr(const Vector &vOther) const;
	
	vec_t Dot(const Vector& vOther) const;
	
	vec_t Length2D(void) const;
	vec_t Length2DSqr(void) const;

	Vector operator+(const Vector& v) const;
	Vector operator-(const Vector& v) const;
	Vector operator*(const Vector& v) const;
	Vector operator/(const Vector& v) const;
	Vector operator*(float fl) const;
	Vector operator/(float fl) const;
	
	Vector Cross(const Vector &vOther) const;
};

class ALIGN(16) VectorAligned : public Vector
{
public:
	inline VectorAligned() {};
	inline VectorAligned(vec_t X, vec_t Y, vec_t Z)
	{
		Init(X, Y, Z);
	}
	
	float w;
};

void VectorCopy(const Vector& src, Vector& dst);
void VectorAdd(const Vector& a, const Vector& b, Vector& result);
void VectorSubtract(const Vector& a, const Vector& b, Vector& result);
void VectorMultiply(const Vector& a, vec_t b, Vector& result);
void VectorMultiply(const Vector& a, const Vector& b, Vector& result);
void VectorDivide(const Vector& a, vec_t b, Vector& result);
void VectorDivide(const Vector& a, const Vector& b, Vector& result);
inline void VectorScale(const Vector& in, vec_t scale, Vector& result);
//void VectorMA(const Vector& start, float scale, const Vector& direction, Vector& dest);  -archi: is this really needed ??
inline vec_t VectorLength(const Vector& v);
vec_t DotProduct(const Vector& a, const Vector& b);
void CrossProduct(const Vector& a, const Vector& b, Vector& result);
void VectorLerp(const Vector& src1, const Vector& src2, vec_t t, Vector& dest);
Vector VectorLerp(const Vector& src1, const Vector& src2, vec_t t);

#endif