#ifndef SDK_MATRICES_H
#define SDK_MATRICES_H
#pragma once

#include "vector.h"

struct matrix3x4_t
{
	matrix3x4_t() {}
	matrix3x4_t(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23);
	matrix3x4_t(const Vector& xAxis, const Vector& yAxis, const Vector& zAxis, const Vector &vecOrigin);

	void Init(const Vector& xAxis, const Vector& yAxis, const Vector& zAxis, const Vector &vecOrigin);

	float *operator[](int i) { return m_flMatVal[i]; }
	const float *operator[](int i) const { return m_flMatVal[i]; }
	float *Base() { return &m_flMatVal[0][0]; }
	const float *Base() const { return &m_flMatVal[0][0]; }

	float m_flMatVal[3][4];
};

class VMatrix
{
public:
	VMatrix();
	VMatrix(vec_t m00, vec_t m01, vec_t m02, vec_t m03, vec_t m10, vec_t m11, vec_t m12, vec_t m13, vec_t m20, vec_t m21, vec_t m22, vec_t m23, vec_t m30, vec_t m31, vec_t m32, vec_t m33);
	VMatrix(const Vector& forward, const Vector& left, const Vector& up);
	VMatrix(const Vector& forward, const Vector& left, const Vector& up, const Vector& translation);
	VMatrix(const matrix3x4_t& matrix3x4);

	void Init(vec_t m00, vec_t m01, vec_t m02, vec_t m03, vec_t m10, vec_t m11, vec_t m12, vec_t m13, vec_t m20, vec_t m21, vec_t m22, vec_t m23, vec_t m30, vec_t m31, vec_t m32, vec_t m33);
	void Init(const matrix3x4_t& matrix3x4);

	inline float* operator[](int i)
	{
		return m[i];
	}

	inline const float* operator[](int i) const
	{
		return m[i];
	}
	
	inline float *Base()
	{
		return &m[0][0];
	}

	inline const float *Base() const
	{
		return &m[0][0];
	}
public:
	vec_t m[4][4];
};

#endif