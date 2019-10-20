#ifndef SDK_VECTOR_H
#define SDK_VECTOR_H
#pragma once

#include <math.h>
#include "../common.h"

#define X_INDEX	0
#define Y_INDEX	1
#define Z_INDEX	2

class Vector
{
public:
	vec_t x, y, z;
	
	Vector(void) {};
	Vector(vec_t X, vec_t Y, vec_t Z);
	
	void Init(vec_t ix = 0.0f, vec_t iy = 0.0f, vec_t iz = 0.0f);
	
	vec_t operator[](int i) const;
	vec_t& operator[](int i);
	
	bool operator==(const Vector& v) const;
	bool operator!=(const Vector& v) const;
	
	FORCEINLINE Vector&	operator+=(const Vector &v);
	FORCEINLINE Vector&	operator-=(const Vector &v);
	FORCEINLINE Vector&	operator*=(const Vector &v);
	FORCEINLINE Vector&	operator*=(float s);
	FORCEINLINE Vector&	operator/=(const Vector &v);
	FORCEINLINE Vector&	operator/=(float s);			
	
	void Negate();

	inline vec_t Length() const;
	FORCEINLINE vec_t LengthSqr(void) const
	{
		return (x*x + y * y + z * z);
	}

	vec_t Length2D(void) const;
	vec_t Length2DSqr(void) const;
	
	FORCEINLINE bool WithinAABox(Vector const &boxmin, Vector const &boxmax);
	vec_t DistTo(const Vector &vOther) const;

	FORCEINLINE vec_t DistToSqr(const Vector &vOther) const
	{
		Vector delta;

		delta.x = x - vOther.x;
		delta.y = y - vOther.y;
		delta.z = z - vOther.z;

		return delta.LengthSqr();
	}
	
	vec_t Dot(const Vector& vOther) const;
	Vector Cross(const Vector &vOther) const;

	Vector& operator=(const Vector &vOther);

	Vector operator+(const Vector& v) const;
	Vector operator-(const Vector& v) const;
	Vector operator*(const Vector& v) const;
	Vector operator/(const Vector& v) const;
	Vector operator*(float fl) const;
	Vector operator/(float fl) const;
};

class __declspec(align(16)) VectorAligned : public Vector
{
public:
	inline VectorAligned(void) {};
	inline VectorAligned(float X, float Y, float Z)
	{
		Init(X, Y, Z);
	}

public:
	explicit VectorAligned(const Vector &vOther)
	{
		Init(vOther.x, vOther.y, vOther.z);
	}

	VectorAligned& operator=(const Vector &vOther)
	{
		Init(vOther.x, vOther.y, vOther.z);
		return *this;
	}

	VectorAligned& operator=(const VectorAligned &vOther)
	{
		Init(vOther.x, vOther.y, vOther.z);
		return *this;
	}

	float w;
};

inline Vector::Vector(vec_t X, vec_t Y, vec_t Z)
{
	x = X; y = Y; z = Z;
}

inline Vector::Vector(const Vector &vOther)
{
	x = vOther.x; y = vOther.y; z = vOther.z;
}

inline void Vector::Init(vec_t ix, vec_t iy, vec_t iz)
{
	x = ix; y = iy; z = iz;
}

inline Vector& Vector::operator=(const Vector &vOther)
{
	x = vOther.x; y = vOther.y; z = vOther.z;
	return *this;
}

inline vec_t& Vector::operator[](int i)
{
	return ((vec_t*)this)[i];
}

inline vec_t Vector::operator[](int i) const
{
	return ((vec_t*)this)[i];
}

inline bool Vector::operator==(const Vector& src) const
{
	return (src.x == x) && (src.y == y) && (src.z == z);
}

inline bool Vector::operator!=(const Vector& src) const
{
	return (src.x != x) || (src.y != y) || (src.z != z);
}

inline void Vector::Negate()
{
	x = -x; y = -y; z = -z;
}

FORCEINLINE Vector& Vector::operator+=(const Vector& v)
{
	x += v.x; y += v.y; z += v.z;
	return *this;
}

FORCEINLINE Vector& Vector::operator-=(const Vector& v)
{
	x -= v.x; y -= v.y; z -= v.z;
	return *this;
}

FORCEINLINE Vector& Vector::operator*=(float fl)
{
	x *= fl;
	y *= fl;
	z *= fl;
	return *this;
}

FORCEINLINE Vector& Vector::operator*=(const Vector& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	return *this;
}

FORCEINLINE Vector& Vector::operator/=(float fl)
{
	float oofl = 1.0f / fl;
	x *= oofl;
	y *= oofl;
	z *= oofl;
	return *this;
}

FORCEINLINE Vector& Vector::operator/=(const Vector& v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
	return *this;
}

inline void VectorLerp(const Vector& src1, const Vector& src2, vec_t t, Vector& dest)
{
	dest.x = src1.x + (src2.x - src1.x) * t;
	dest.y = src1.y + (src2.y - src1.y) * t;
	dest.z = src1.z + (src2.z - src1.z) * t;
}

inline Vector VectorLerp(const Vector& src1, const Vector& src2, vec_t t)
{
	Vector result;
	VectorLerp(src1, src2, t, result);
	return result;
}

FORCEINLINE void VectorAdd(const Vector& a, const Vector& b, Vector& c)
{
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	c.z = a.z + b.z;
}

FORCEINLINE void VectorSubtract(const Vector& a, const Vector& b, Vector& c)
{
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	c.z = a.z - b.z;
}

FORCEINLINE void VectorMultiply(const Vector& a, vec_t b, Vector& c)
{
	c.x = a.x * b;
	c.y = a.y * b;
	c.z = a.z * b;
}

FORCEINLINE void VectorMultiply(const Vector& a, const Vector& b, Vector& c)
{
	c.x = a.x * b.x;
	c.y = a.y * b.y;
	c.z = a.z * b.z;
}

FORCEINLINE void VectorDivide(const Vector& a, vec_t b, Vector& c)
{
	vec_t oob = 1.0f / b;
	c.x = a.x * oob;
	c.y = a.y * oob;
	c.z = a.z * oob;
}

FORCEINLINE void VectorDivide(const Vector& a, const Vector& b, Vector& c)
{
	c.x = a.x / b.x;
	c.y = a.y / b.y;
	c.z = a.z / b.z;
}

FORCEINLINE vec_t DotProduct(const Vector& a, const Vector& b)
{
	return(a.x*b.x + a.y*b.y + a.z*b.z);
}

inline vec_t Vector::Dot(const Vector& vOther) const
{
	return DotProduct(*this, vOther);
}

inline void CrossProduct(const Vector& a, const Vector& b, Vector& result)
{
	result.x = a.y*b.z - a.z*b.y;
	result.y = a.z*b.x - a.x*b.z;
	result.z = a.x*b.y - a.y*b.x;
}

inline vec_t DotProductAbs(const Vector &v0, const Vector &v1)
{
	return FloatMakePositive(v0.x*v1.x) + FloatMakePositive(v0.y*v1.y) + FloatMakePositive(v0.z*v1.z);
}

inline vec_t DotProductAbs(const Vector &v0, const float *v1)
{
	return FloatMakePositive(v0.x * v1[0]) + FloatMakePositive(v0.y * v1[1]) + FloatMakePositive(v0.z * v1[2]);
}

inline vec_t VectorLength(const Vector& v)
{
	return (vec_t)sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

inline vec_t Vector::Length(void) const
{
	return VectorLength(*this);
}

inline vec_t Vector::Length2D(void) const
{
	return sqrt(x*x + y * y);
}

inline vec_t Vector::Length2DSqr(void) const
{
	return (x*x + y * y);
}

bool Vector::WithinAABox(Vector const &boxmin, Vector const &boxmax)
{
	return (
		(x >= boxmin.x) && (x <= boxmax.x) &&
		(y >= boxmin.y) && (y <= boxmax.y) &&
		(z >= boxmin.z) && (z <= boxmax.z)
		);
}

inline vec_t Vector::DistTo(const Vector &vOther) const
{
	Vector delta;
	VectorSubtract(*this, vOther, delta);
	return delta.Length();
}

inline Vector Vector::operator+(const Vector& v) const
{
	Vector res;
	VectorAdd(*this, v, res);
	return res;
}

inline Vector Vector::operator-(const Vector& v) const
{
	Vector res;
	VectorSubtract(*this, v, res);
	return res;
}

inline Vector Vector::operator*(float fl) const
{
	Vector res;
	VectorMultiply(*this, fl, res);
	return res;
}

inline Vector Vector::operator*(const Vector& v) const
{
	Vector res;
	VectorMultiply(*this, v, res);
	return res;
}

inline Vector Vector::operator/(float fl) const
{
	Vector res;
	VectorDivide(*this, fl, res);
	return res;
}

inline Vector Vector::operator/(const Vector& v) const
{
	Vector res;
	VectorDivide(*this, v, res);
	return res;
}

inline Vector operator*(float fl, const Vector& v)
{
	return v * fl;
}

inline Vector Vector::Cross(const Vector& vOther) const
{
	Vector res;
	CrossProduct(*this, vOther, res);
	return res;
}

inline Vector CrossProduct(const Vector& a, const Vector& b)
{
	return Vector(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
}

class QAngle
{
public:
	vec_t x, y, z;
	
	QAngle(void) {};
	QAngle(vec_t X, vec_t Y, vec_t Z);
	
	void Init(vec_t ix = 0.0f, vec_t iy = 0.0f, vec_t iz = 0.0f);
	
	vec_t operator[](int i) const;
	vec_t& operator[](int i);

	QAngle&	operator+=(const QAngle &v);
	QAngle&	operator-=(const QAngle &v);
	QAngle&	operator*=(float s);
	QAngle&	operator/=(float s);
	
	vec_t Length() const;
	vec_t LengthSqr() const;

	QAngle& operator=(const QAngle& src);

	QAngle operator-(void) const;

	QAngle operator+(const QAngle& v) const;
	QAngle operator-(const QAngle& v) const;
	QAngle operator*(float fl) const;
	QAngle operator/(float fl) const;
};

inline void VectorAdd(const QAngle& a, const QAngle& b, QAngle& result)
{
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;
}

inline void VectorMA(const QAngle &start, float scale, const QAngle &direction, QAngle &dest)
{
	dest.x = start.x + scale * direction.x;
	dest.y = start.y + scale * direction.y;
	dest.z = start.z + scale * direction.z;
}

inline QAngle::QAngle(vec_t X, vec_t Y, vec_t Z)
{
	x = X; y = Y; z = Z;
}

inline void QAngle::Init(vec_t ix, vec_t iy, vec_t iz)
{
	x = ix; y = iy; z = iz;
}

inline QAngle& QAngle::operator=(const QAngle &vOther)
{
	x = vOther.x; y = vOther.y; z = vOther.z;
	return *this;
}

inline vec_t& QAngle::operator[](int i)
{
	return ((vec_t*)this)[i];
}

inline vec_t QAngle::operator[](int i) const
{
	return ((vec_t*)this)[i];
}

inline QAngle& QAngle::operator+=(const QAngle& v)
{
	x += v.x; y += v.y; z += v.z;
	return *this;
}

inline QAngle& QAngle::operator-=(const QAngle& v)
{
	x -= v.x; y -= v.y; z -= v.z;
	return *this;
}

inline QAngle& QAngle::operator*=(float fl)
{
	x *= fl;
	y *= fl;
	z *= fl;
	return *this;
}

inline QAngle& QAngle::operator/=(float fl)
{
	float oofl = 1.0f / fl;
	x *= oofl;
	y *= oofl;
	z *= oofl;
	return *this;
}

inline vec_t QAngle::Length() const
{
	return (vec_t)sqrt(LengthSqr());
}

inline vec_t QAngle::LengthSqr() const
{
	return x * x + y * y + z * z;
}

inline QAngle QAngle::operator-(void) const
{
	QAngle ret(-x, -y, -z);
	return ret;
}

inline QAngle QAngle::operator+(const QAngle& v) const
{
	QAngle res;
	res.x = x + v.x;
	res.y = y + v.y;
	res.z = z + v.z;
	return res;
}

inline QAngle QAngle::operator-(const QAngle& v) const
{
	QAngle res;
	res.x = x - v.x;
	res.y = y - v.y;
	res.z = z - v.z;
	return res;
}

inline QAngle QAngle::operator*(float fl) const
{
	QAngle res;
	res.x = x * fl;
	res.y = y * fl;
	res.z = z * fl;
	return res;
}

inline QAngle QAngle::operator/(float fl) const
{
	QAngle res;
	res.x = x / fl;
	res.y = y / fl;
	res.z = z / fl;
	return res;
}

inline QAngle operator*(float fl, const QAngle& v)
{
	QAngle ret(v * fl);
	return ret;
}

class Vector2D
{
public:
	vec_t x, y;
	
	Vector2D(void) {};
	Vector2D(vec_t X, vec_t Y);

	void Init(vec_t ix = 0.0f, vec_t iy = 0.0f);
	
	vec_t operator[](int i) const;
	vec_t& operator[](int i);
	
	Vector2D& operator+=(const Vector2D &v);
	Vector2D& operator-=(const Vector2D &v);
	Vector2D& operator*=(const Vector2D &v);
	Vector2D& operator*=(float s);
	Vector2D& operator/=(const Vector2D &v);
	Vector2D& operator/=(float s);
	
	void Negate();
	
	vec_t Length() const;
	vec_t LengthSqr(void) const;

	vec_t DistTo(const Vector2D &vOther) const;
	vec_t DistToSqr(const Vector2D &vOther) const;

	vec_t Dot(const Vector2D& vOther) const;
	
	Vector2D& operator=(const Vector2D &vOther);

	Vector2D(const Vector2D &vOther);

	Vector2D operator-(void) const;
	Vector2D operator+(const Vector2D& v) const;
	Vector2D operator-(const Vector2D& v) const;
	Vector2D operator*(const Vector2D& v) const;
	Vector2D operator/(const Vector2D& v) const;
	Vector2D operator*(float fl) const;
	Vector2D operator/(float fl) const;
	
	//Vector2D	Cross(const Vector2D &vOther) const;
};

inline Vector2D::Vector2D(vec_t X, vec_t Y)
{
	x = X; y = Y;
}

inline Vector2D::Vector2D(const Vector2D &vOther)
{
	x = vOther.x; y = vOther.y;
}

inline void Vector2D::Init(vec_t ix, vec_t iy)
{
	x = ix; y = iy;
}

inline Vector2D& Vector2D::operator=(const Vector2D &vOther)
{
	x = vOther.x; y = vOther.y;
	return *this;
}

inline vec_t& Vector2D::operator[](int i)
{
	return ((vec_t*)this)[i];
}

inline vec_t Vector2D::operator[](int i) const
{
	return ((vec_t*)this)[i];
}

inline void Vector2D::Negate()
{
	x = -x; y = -y;
}

inline Vector2D& Vector2D::operator+=(const Vector2D& v)
{
	x += v.x; y += v.y;
	return *this;
}

inline Vector2D& Vector2D::operator-=(const Vector2D& v)
{
	x -= v.x; y -= v.y;
	return *this;
}

inline Vector2D& Vector2D::operator*=(float fl)
{
	x *= fl;
	y *= fl;
	return *this;
}

inline Vector2D& Vector2D::operator*=(const Vector2D& v)
{
	x *= v.x;
	y *= v.y;
	return *this;
}

inline Vector2D& Vector2D::operator/=(float fl)
{
	float oofl = 1.0f / fl;
	x *= oofl;
	y *= oofl;
	return *this;
}

inline Vector2D& Vector2D::operator/=(const Vector2D& v)
{
	x /= v.x;
	y /= v.y;
	return *this;
}

inline void Vector2DAdd(const Vector2D& a, const Vector2D& b, Vector2D& c)
{
	c.x = a.x + b.x;
	c.y = a.y + b.y;
}

inline void Vector2DSubtract(const Vector2D& a, const Vector2D& b, Vector2D& c)
{
	c.x = a.x - b.x;
	c.y = a.y - b.y;
}

inline void Vector2DMultiply(const Vector2D& a, vec_t b, Vector2D& c)
{
	c.x = a.x * b;
	c.y = a.y * b;
}

inline void Vector2DMultiply(const Vector2D& a, const Vector2D& b, Vector2D& c)
{
	c.x = a.x * b.x;
	c.y = a.y * b.y;
}

inline void Vector2DDivide(const Vector2D& a, vec_t b, Vector2D& c)
{
	vec_t oob = 1.0f / b;
	c.x = a.x * oob;
	c.y = a.y * oob;
}

inline void Vector2DDivide(const Vector2D& a, const Vector2D& b, Vector2D& c)
{
	c.x = a.x / b.x;
	c.y = a.y / b.y;
}

inline void Vector2DMA(const Vector2D& start, float s, const Vector2D& dir, Vector2D& result)
{
	result.x = start.x + s * dir.x;
	result.y = start.y + s * dir.y;
}

inline vec_t DotProduct2D(const Vector2D& a, const Vector2D& b)
{
	return(a.x*b.x + a.y*b.y);
}

inline vec_t Vector2D::Dot(const Vector2D& vOther) const
{
	return DotProduct2D(*this, vOther);
}

inline vec_t Vector2DLength(const Vector2D& v)
{
	return (vec_t)sqrt(v.x*v.x + v.y*v.y);
}

inline vec_t Vector2D::Length(void) const
{
	return Vector2DLength(*this);
}

inline vec_t Vector2D::LengthSqr(void) const
{
	return (x*x + y * y);
}

inline vec_t Vector2D::DistTo(const Vector2D &vOther) const
{
	Vector2D delta;
	Vector2DSubtract(*this, vOther, delta);
	return delta.Length();
}

inline vec_t Vector2D::DistToSqr(const Vector2D &vOther) const
{
	Vector2D delta;
	Vector2DSubtract(*this, vOther, delta);
	return delta.LengthSqr();
}

inline Vector2D Vector2D::operator-(void) const
{
	return Vector2D(-x, -y);
}

inline Vector2D Vector2D::operator+(const Vector2D& v) const
{
	Vector2D res;
	Vector2DAdd(*this, v, res);
	return res;
}

inline Vector2D Vector2D::operator-(const Vector2D& v) const
{
	Vector2D res;
	Vector2DSubtract(*this, v, res);
	return res;
}

inline Vector2D Vector2D::operator*(float fl) const
{
	Vector2D res;
	Vector2DMultiply(*this, fl, res);
	return res;
}

inline Vector2D Vector2D::operator*(const Vector2D& v) const
{
	Vector2D res;
	Vector2DMultiply(*this, v, res);
	return res;
}

inline Vector2D Vector2D::operator/(float fl) const
{
	Vector2D res;
	Vector2DDivide(*this, fl, res);
	return res;
}

inline Vector2D Vector2D::operator/(const Vector2D& v) const
{
	Vector2D res;
	Vector2DDivide(*this, v, res);
	return res;
}

inline Vector2D operator*(float fl, const Vector2D& v)
{
	return v * fl;
}

#endif