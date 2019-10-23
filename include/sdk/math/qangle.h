#ifndef SDK_QANGLE_H
#define SDK_QANGLE_H
#pragma once

#include "../common.h"

class QAngle
{
public:
	vec_t x, y, z;
	
	QAngle();
	QAngle(vec_t X, vec_t Y, vec_t Z);
	
	void Init(vec_t ix = 0.0f, vec_t iy = 0.0f, vec_t iz = 0.0f);

	vec_t operator[](int i) const;
	vec_t& operator[](int i);
	
	bool operator==(const QAngle& v) const;
	bool operator!=(const QAngle& v) const;
	
	QAngle&	operator+=(const QAngle &v);
	QAngle&	operator-=(const QAngle &v);
	QAngle&	operator*=(float s);
	QAngle&	operator/=(float s);

	vec_t Length() const;
	vec_t LengthSqr() const;
	
	QAngle operator+(const QAngle& v) const;
	QAngle operator-(const QAngle& v) const;
	QAngle operator*(float fl) const;
	QAngle operator/(float fl) const;
};

#endif