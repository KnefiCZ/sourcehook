#ifndef SDK_VECTOR_H
#define SDK_VECTOR_H
#pragma once

#include "../common.h"

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
	inline VectorAligned(void) {};
	inline VectorAligned(vec_t X, vec_t Y, vec_t Z)
	{
		Init(X, Y, Z);
	}
	
	explicit VectorAligned(const Vector &vOther)
	{
		Init(vOther.x, vOther.y, vOther.z);
	}

	VectorAligned& operator=(const Vector &vOther)
	{
		Init(vOther.x, vOther.y, vOther.z);
		return *this;
	}

	float w;
};

#endif