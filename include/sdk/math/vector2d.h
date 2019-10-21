#ifndef SDK_VECTOR2D_H
#define SDK_VECTOR2D_H
#pragma once

class Vector2D
{
public:
	vec_t x, y;

	Vector2D();
	Vector2D(vec_t X, vec_t Y);
	
	void Init(vec_t ix = 0.0f, vec_t iy = 0.0f);
	
	vec_t operator[](int i) const;
	vec_t& operator[](int i);
	
	bool operator==(Vector2D& v);
	bool operator!=(Vector2D& v);
	
	Vector2D& operator+=(const Vector2D &v);
	Vector2D& operator-=(const Vector2D &v);
	Vector2D& operator*=(const Vector2D &v);
	Vector2D& operator*=(float s);
	Vector2D& operator/=(const Vector2D &v);
	Vector2D& operator/=(float s);
	
	void Negate();

	vec_t Length() const;
	vec_t LengthSqr() const;

	vec_t DistTo(const Vector2D &vOther) const;
	vec_t DistToSqr(const Vector2D &vOther) const;

	vec_t Dot(Vector2D& vOther) const;
	
	Vector2D operator+(const Vector2D& v) const;
	Vector2D operator-(const Vector2D& v) const;
	Vector2D operator*(const Vector2D& v) const;
	Vector2D operator/(const Vector2D& v) const;
	Vector2D operator*(float fl) const;
	Vector2D operator/(float fl) const;
};

void Vector2DCopy(const Vector2D& src, Vector2D& dst);
void Vector2DAdd(const Vector2D& a, const Vector2D& b, Vector2D& result);
void Vector2DSubtract(const Vector2D& a, const Vector2D& b, Vector2D& result);
void Vector2DMultiply(const Vector2D& a, vec_t b, Vector2D& result);
void Vector2DMultiply(const Vector2D& a, const Vector2D& b, Vector2D& result);
void Vector2DDivide(const Vector2D& a, vec_t b, Vector2D& result);
void Vector2DDivide(const Vector2D& a, const Vector2D& b, Vector2D& result);
void Vector2DMA(const Vector2D& start, float s, const Vector2D& dir, Vector2D& result);
vec_t Vector2DLength(const Vector2D& v);
vec_t DotProduct2D(const Vector2D& a, const Vector2D& b);
void Vector2DLerp(const Vector2D& src1, const Vector2D& src2, vec_t t, Vector2D& dest);

#endif