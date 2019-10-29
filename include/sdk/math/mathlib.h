#ifndef SDK_MATHLIB_H
#define SDK_MATHLIB_H
#pragma once

#include "../common.h"
#include "sdk/math/matrices.h"
#include "sdk/math/qangle.h"

#ifndef M_PI
#define M_PI 3.141593f
#endif

#ifndef M_INV_TWO_PI
#define M_INV_TWO_PI 0.1591549f
#endif

#ifndef M_TWO_PI
#define M_TWO_PI 6.283185f
#endif

#ifndef M_HALF_PI
#define M_HALF_PI 1.570796f
#endif

#ifndef M_THREE_HALF_PI
#define M_THREE_HALF_PI 4.7123889f
#endif

#ifndef RAD2DEG
#define RAD2DEG(x)  ( (float)(x) * (float)(180.f / M_PI) )
#endif

#ifndef DEG2RAD
#define DEG2RAD(x)  ( (float)(x) * (float)(M_PI / 180.f) )
#endif

/*---------------------------------------------------------------------------------------------------------------------------------------------------*/

struct cplane_t
{
	Vector normal;
	float dist;
	byte type;
	byte signbits;
	byte pad[2];
};

/*---------------------------------------------------------------------------------------------------------------------------------------------------*/

vec_t DotProduct(const vec_t *v1, const vec_t *v2);
vec_t DotProduct(const Vector& a, const Vector& b);
vec_t DotProduct(const Vector& a, const vec_t *b);

vec_t VectorLength(const Vector& v);

void VectorNormalize(Vector& vec);
void VectorAdd(const Vector& a, const Vector& b, Vector& c);
void VectorSubtract(const Vector& a, const Vector& b, Vector& c);
void VectorMultiply(const Vector& a, vec_t b, Vector& c);
void VectorMultiply(const Vector& a, const Vector& b, Vector& c);
void VectorDivide(const Vector& a, vec_t b, Vector& c);
void VectorDivide(const Vector& a, const Vector& b, Vector& c);
void CrossProduct(const Vector& a, const Vector& b, Vector& result);
void VectorTransform(const Vector& in1, const matrix3x4_t& in2, Vector& out);
void AngleVectors(const QAngle &angles, Vector& forward);
void AngleVectors(const QAngle &angles, Vector& forward, Vector& right, Vector& up);
void VectorAngles(const Vector& forward, QAngle& angles);

void NormalizeAngles(QAngle& angles);
float GetFOV(const QAngle& viewAngle, const QAngle& aimAngle);
QAngle CalcAngle(const Vector& src, const Vector& dst);

/*---------------------------------------------------------------------------------------------------------------------------------------------------*/

bool ScreenTransform(const Vector& a, Vector& b);
bool WorldToScreen(const Vector& a, Vector& b);

/*---------------------------------------------------------------------------------------------------------------------------------------------------*/

template <class T>
inline T Lerp(float flPercent, T const &A, T const &B)
{
	return A + (B - A) * flPercent;
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------*/

#endif