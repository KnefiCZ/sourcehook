#ifndef SDK_MATHLIB_H
#define SDK_MATHLIB_H
#pragma once

#include <xmmintrin.h>
#include <math.h>
#include "../common.h"
#include "vector.h"

struct matrix3x4_t
{
	matrix3x4_t() {}
	matrix3x4_t(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23)
	{
		m_flMatVal[0][0] = m00;	m_flMatVal[0][1] = m01; m_flMatVal[0][2] = m02; m_flMatVal[0][3] = m03;
		m_flMatVal[1][0] = m10;	m_flMatVal[1][1] = m11; m_flMatVal[1][2] = m12; m_flMatVal[1][3] = m13;
		m_flMatVal[2][0] = m20;	m_flMatVal[2][1] = m21; m_flMatVal[2][2] = m22; m_flMatVal[2][3] = m23;
	}

	//-----------------------------------------------------------------------------
	// Creates a matrix where the X axis = forward
	// the Y axis = left, and the Z axis = up
	//-----------------------------------------------------------------------------
	void Init(const Vector& xAxis, const Vector& yAxis, const Vector& zAxis, const Vector &vecOrigin)
	{
		m_flMatVal[0][0] = xAxis.x; m_flMatVal[0][1] = yAxis.x; m_flMatVal[0][2] = zAxis.x; m_flMatVal[0][3] = vecOrigin.x;
		m_flMatVal[1][0] = xAxis.y; m_flMatVal[1][1] = yAxis.y; m_flMatVal[1][2] = zAxis.y; m_flMatVal[1][3] = vecOrigin.y;
		m_flMatVal[2][0] = xAxis.z; m_flMatVal[2][1] = yAxis.z; m_flMatVal[2][2] = zAxis.z; m_flMatVal[2][3] = vecOrigin.z;
	}

	//-----------------------------------------------------------------------------
	// Creates a matrix where the X axis = forward
	// the Y axis = left, and the Z axis = up
	//-----------------------------------------------------------------------------
	matrix3x4_t(const Vector& xAxis, const Vector& yAxis, const Vector& zAxis, const Vector &vecOrigin)
	{
		Init(xAxis, yAxis, zAxis, vecOrigin);
	}

	float *operator[](int i) { return m_flMatVal[i]; }
	const float *operator[](int i) const { return m_flMatVal[i]; }
	float *Base() { return &m_flMatVal[0][0]; }
	const float *Base() const { return &m_flMatVal[0][0]; }

	float m_flMatVal[3][4];
};

#ifndef M_PI
#define M_PI		3.14159265358979323846
#endif

#define M_PI_F		((float)(M_PI))

#ifndef RAD2DEG
#define RAD2DEG( x  )  ( (float)(x) * (float)(180.f / M_PI_F) )
#endif

#ifndef DEG2RAD
#define DEG2RAD( x  )  ( (float)(x) * (float)(M_PI_F / 180.f) )
#endif

#define	IS_NAN(x) (((*(int *)&x)&nanmask)==nanmask)

void inline SinCos(float radians, float *sine, float *cosine)
{
	*sine = sin(radians);
	*cosine = cos(radians);
}

template< class T >
inline T clamp(T const &val, T const &minVal, T const &maxVal)
{
	if (maxVal < minVal)
		return maxVal;
	else if (val < minVal)
		return minVal;
	else if (val > maxVal)
		return maxVal;
	else
		return val;
}

template<class T>
FORCEINLINE T Square(T const &a)
{
	return a * a;
}

FORCEINLINE uint SmallestPowerOfTwoGreaterOrEqual(uint x)
{
	x -= 1;
	x |= x >> 1;
	x |= x >> 2;
	x |= x >> 4;
	x |= x >> 8;
	x |= x >> 16;
	return x + 1;
}

FORCEINLINE uint LargestPowerOfTwoLessThanOrEqual(uint x)
{
	if (x >= 0x80000000)
		return 0x80000000;

	return SmallestPowerOfTwoGreaterOrEqual(x + 1) >> 1;
}

inline float RemapVal(float val, float A, float B, float C, float D)
{
	if (A == B)
		return val >= B ? D : C;
	return C + (D - C) * (val - A) / (B - A);
}

inline float RemapValClamped(float val, float A, float B, float C, float D)
{
	if (A == B)
		return val >= B ? D : C;
	float cVal = (val - A) / (B - A);
	cVal = clamp(cVal, 0.0f, 1.0f);

	return C + (D - C) * cVal;
}

template <class T>
FORCEINLINE T Lerp(float flPercent, T const &A, T const &B)
{
	return A + (B - A) * flPercent;
}

template <class T>
FORCEINLINE void V_swap(T& x, T& y)
{
	T temp = x;
	x = y;
	y = temp;
}

template <class T> FORCEINLINE T AVG(T a, T b)
{
	return (a + b) / 2;
}

void VectorNormalizeFast(Vector& vec)
{
	float iradius = 1.f / (sqrtf(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z) + FLT_EPSILON);

	vec.x *= iradius;
	vec.y *= iradius;
	vec.z *= iradius;
}

inline void AngleVectors(const QAngle& angles, Vector *forward)
{
	float	sp, sy, cp, cy;

	SinCos(DEG2RAD(angles[1]), &sy, &cy);
	SinCos(DEG2RAD(angles[0]), &sp, &cp);

	forward->x = cp * cy;
	forward->y = cp * sy;
	forward->z = -sp;
}

inline void AngleVectors(const QAngle& angles, Vector *forward, Vector *right, Vector *up)
{
	float sr, sp, sy, cr, cp, cy;

	SinCos(DEG2RAD(angles[0]), &sp, &cp);
	SinCos(DEG2RAD(angles[1]), &sy, &cy);
	SinCos(DEG2RAD(angles[2]), &sr, &cr);

	if (forward)
	{
		forward->x = cp * cy;
		forward->y = cp * sy;
		forward->z = -sp;
	}

	if (right)
	{
		right->x = (-1 * sr*sp*cy + -1 * cr*-sy);
		right->y = (-1 * sr*sp*sy + -1 * cr*cy);
		right->z = -1 * sr*cp;
	}

	if (up)
	{
		up->x = (cr*sp*cy + -sr * -sy);
		up->y = (cr*sp*sy + -sr * cy);
		up->z = cr * cp;
	}
}

inline void VectorAngles(const Vector &forward, QAngle &angles)
{
	float	tmp, yaw, pitch;

	if (forward[1] == 0 && forward[0] == 0)
	{
		yaw = 0;
		if (forward[2] > 0)
			pitch = 270;
		else
			pitch = 90;
	}
	else
	{
		yaw = (atan2(forward[1], forward[0]) * 180 / M_PI);
		if (yaw < 0)
			yaw += 360;

		tmp = sqrt(forward[0] * forward[0] + forward[1] * forward[1]);
		pitch = (atan2(-forward[2], tmp) * 180 / M_PI);
		if (pitch < 0)
			pitch += 360;
	}

	angles[0] = pitch;
	angles[1] = yaw;
	angles[2] = 0;
}

inline void VectorAngles(const Vector &forward, const Vector &pseudoup, QAngle &angles)
{
	Vector left;

	CrossProduct(pseudoup, forward, left);
	VectorNormalizeFast(left);

	float xyDist = sqrtf(forward[0] * forward[0] + forward[1] * forward[1]);

	if (xyDist > 0.001f)
	{
		angles[1] = RAD2DEG(atan2f(forward[1], forward[0]));
		angles[0] = RAD2DEG(atan2f(-forward[2], xyDist));

		float up_z = (left[1] * forward[0]) - (left[0] * forward[1]);

		angles[2] = RAD2DEG(atan2f(left[2], up_z));
	}
	else
	{
		angles[1] = RAD2DEG(atan2f(-left[0], left[1]));
		angles[0] = RAD2DEG(atan2f(-forward[2], xyDist));
		angles[2] = 0;
	}
}

FORCEINLINE int RoundFloatToInt(float f)
{
	return _mm_cvtss_si32(_mm_load_ss(&f));
}

FORCEINLINE unsigned char RoundFloatToByte(float f)
{
	int nResult = RoundFloatToInt(f);
	return (unsigned char)nResult;
}

FORCEINLINE unsigned long RoundFloatToUnsignedLong(float f)
{
	unsigned char nResult[8];

	__asm
	{
		fld f
		fistp       qword ptr nResult
	}

	return *((unsigned long*)nResult);
}

FORCEINLINE bool IsIntegralValue(float flValue, float flTolerance = 0.001f)
{
	return fabs(RoundFloatToInt(flValue) - flValue) < flTolerance;
}

FORCEINLINE int Float2Int(float a)
{
	return (int)a;
}

inline int Floor2Int(float a)
{
	int RetVal;
	RetVal = static_cast<int>(floor(a));

	return RetVal;
}

FORCEINLINE unsigned int FastFToC(float c)
{
	return Float2Int(c * 255.0f);
}

FORCEINLINE int FastFloatToSmallInt(float c)
{
	return Float2Int(c);
}

inline float ClampToMsec(float in)
{
	int msec = Floor2Int(in * 1000.0f + 0.5f);
	return 0.001f * msec;
}

inline int Ceil2Int(float a)
{
	int RetVal;
	RetVal = static_cast<int>(ceil(a));
	
	return RetVal;
}

inline void RGBtoHSV(const Vector &rgb, Vector &hsv)
{
	float flMax = max(rgb.x, rgb.y);
	flMax = max(flMax, rgb.z);
	float flMin = min(rgb.x, rgb.y);
	flMin = min(flMin, rgb.z);
	
	hsv.z = flMax;
	
	if (flMax != 0.0F)
	{
		hsv.y = (flMax - flMin) / flMax;
	}
	else
	{
		hsv.y = 0.0F;
	}

	if (hsv.y == 0.0F)
	{
		hsv.x = -1.0f;
	}
	else
	{
		float32 d = flMax - flMin;
		if (rgb.x == flMax)
		{
			hsv.x = (rgb.y - rgb.z) / d;
		}
		else if (rgb.y == flMax)
		{
			hsv.x = 2.0F + (rgb.z - rgb.x) / d;
		}
		else
		{
			hsv.x = 4.0F + (rgb.x - rgb.y) / d;
		}
		hsv.x *= 60.0F;
		if (hsv.x < 0.0F)
		{
			hsv.x += 360.0F;
		}
	}
}

inline void HSVtoRGB(const Vector &hsv, Vector &rgb)
{
	if (hsv.y == 0.0F)
	{
		rgb.Init(hsv.z, hsv.z, hsv.z);
		return;
	}

	float32 hue = hsv.x;
	if (hue == 360.0F)
	{
		hue = 0.0F;
	}
	hue /= 60.0F;
	int     i = hue;
	float32 f = hue - i;
	float32 p = hsv.z * (1.0F - hsv.y);
	float32 q = hsv.z * (1.0F - hsv.y * f);
	float32 t = hsv.z * (1.0F - hsv.y * (1.0F - f));
	switch (i)
	{
	case 0: rgb.Init(hsv.z, t, p); break;
	case 1: rgb.Init(q, hsv.z, p); break;
	case 2: rgb.Init(p, hsv.z, t); break;
	case 3: rgb.Init(p, q, hsv.z); break;
	case 4: rgb.Init(t, p, hsv.z); break;
	case 5: rgb.Init(hsv.z, p, q); break;
	}
}

#endif