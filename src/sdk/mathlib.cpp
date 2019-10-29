/*==============================================================================*/
#include <limits>
#include <math.h>

#include "sdk/util/bitbytes.h"

#include "sdk/math/math_pfns.h"
#include "sdk/math/mathlib.h"

#include "sdk/math/matrices.h"
#include "sdk/math/vector.h"
#include "sdk/math/qangle.h"
/*==============================================================================*/


/*==============================================================================*/
//	Math Performance Functions
/*==============================================================================*/
float FastSqrt(float x)
{
	return sqrt(x);
}

void FastSinCos(float angle, float* s, float* c)
{
	*s = sin(angle);
	*c = cos(angle);
}

float FastSin(float x)
{
	return sin(x);
}

float FastCos(float x)
{
	return cos(x);
}


/*==============================================================================*/
//	Mathlib																		*/
/*==============================================================================*/
vec_t DotProduct(const vec_t *v1, const vec_t *v2)
{
	return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}

vec_t DotProduct(const Vector& a, const Vector& b)
{
	return(a.x * b.x + a.y * b.y + a.z * b.z);
}

vec_t DotProduct(const Vector& a, const vec_t *b)
{
	return(a.x * b[0] + a.y * b[1] + a.z * b[2]);
}

vec_t VectorLength(const Vector& v)
{
	return FastSqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

void VectorNormalize(Vector& vec)
{
	float iradius = 1.f / (FastSqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z) + FLT_EPSILON);

	vec.x *= iradius;
	vec.y *= iradius;
	vec.z *= iradius;
}

void VectorAdd(const Vector& a, const Vector& b, Vector& c)
{
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	c.z = a.z + b.z;
}

void VectorSubtract(const Vector& a, const Vector& b, Vector& c)
{
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	c.z = a.z - b.z;
}

void VectorMultiply(const Vector& a, vec_t b, Vector& c)
{
	c.x = a.x * b;
	c.y = a.y * b;
	c.z = a.z * b;
}

void VectorMultiply(const Vector& a, const Vector& b, Vector& c)
{
	c.x = a.x * b.x;
	c.y = a.y * b.y;
	c.z = a.z * b.z;
}

void VectorDivide(const Vector& a, vec_t b, Vector& c)
{
	vec_t oob = 1.0f / b;

	c.x = a.x * oob;
	c.y = a.y * oob;
	c.z = a.z * oob;
}

void VectorDivide(const Vector& a, const Vector& b, Vector& c)
{
	c.x = a.x / b.x;
	c.y = a.y / b.y;
	c.z = a.z / b.z;
}

void CrossProduct(const Vector& a, const Vector& b, Vector& result)
{
	result.x = a.y*b.z - a.z*b.y;
	result.y = a.z*b.x - a.x*b.z;
	result.z = a.x*b.y - a.y*b.x;
}

void VectorTransform(const Vector& in1, const matrix3x4_t& in2, Vector& out)
{
	out[0] = DotProduct(in1, in2[0]) + in2[0][3];
	out[1] = DotProduct(in1, in2[1]) + in2[1][3];
	out[2] = DotProduct(in1, in2[2]) + in2[2][3];
}

void AngleVectors(const QAngle& angles, Vector& forward)
{
	float sp, sy, cp, cy;

	FastSinCos(DEG2RAD(angles[0]), &sp, &cp);
	FastSinCos(DEG2RAD(angles[1]), &sy, &cy);

	forward.x = cp * cy;
	forward.y = cp * sy;
	forward.z = -sp;
}

void AngleVectors(const QAngle& angles, Vector& forward, Vector& right, Vector& up)
{
	float sr, sp, sy, cr, cp, cy;

	FastSinCos(DEG2RAD(angles[0]), &sp, &cp);
	FastSinCos(DEG2RAD(angles[1]), &sy, &cy);
	FastSinCos(DEG2RAD(angles[2]), &sr, &cr);

	forward.x = cp * cy;
	forward.y = cp * sy;
	forward.z = -sp;

	right.x = (-1 * sr*sp*cy + -1 * cr*-sy);
	right.y = (-1 * sr*sp*sy + -1 * cr*cy);
	right.z = -1 * sr*cp;

	up.x = (cr*sp*cy + -sr * -sy);
	up.y = (cr*sp*sy + -sr * cy);
	up.z = cr * cp;
}

void VectorAngles(const Vector& forward, QAngle& angles)
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

		tmp = FastSqrt(forward[0] * forward[0] + forward[1] * forward[1]);
		pitch = (atan2(-forward[2], tmp) * 180 / M_PI);
		if (pitch < 0)
			pitch += 360;
	}

	angles[0] = pitch;
	angles[1] = yaw;
	angles[2] = 0;
}

void NormalizeAngles(QAngle& angles)
{
	for (int i = 0; i < 3; i++)
	{
		if (angles[i] > 180.0)
		{
			angles[i] -= 360.0;
		}
		else if (angles[i] < -180.0)
		{
			angles[i] += 360.0;
		}
	}
}

float GetFOV(const QAngle& viewAngle, const QAngle& aimAngle)
{
	Vector ang, aim;

	AngleVectors(viewAngle, aim);
	AngleVectors(aimAngle, ang);

	return RAD2DEG(acos(aim.Dot(ang) / aim.LengthSqr()));
}

QAngle CalcAngle(const Vector& src, const Vector& dst)
{
	QAngle vAngle;
	Vector delta((src.x - dst.x), (src.y - dst.y), (src.z - dst.z));
	double hyp = FastSqrt(delta.x * delta.x + delta.y * delta.y);

	vAngle.x = float(atanf(float(delta.z / hyp)) * 57.295779513082f);
	vAngle.y = float(atanf(float(delta.y / delta.x)) * 57.295779513082f);
	vAngle.z = 0.0f;

	if (delta.x >= 0.0)
		vAngle.y += 180.0f;

	return vAngle;
}


/*==============================================================================*/
//	matrix3x4_t
/*==============================================================================*/
matrix3x4_t::matrix3x4_t(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23)
{
	m_flMatVal[0][0] = m00;	m_flMatVal[0][1] = m01; m_flMatVal[0][2] = m02; m_flMatVal[0][3] = m03;
	m_flMatVal[1][0] = m10;	m_flMatVal[1][1] = m11; m_flMatVal[1][2] = m12; m_flMatVal[1][3] = m13;
	m_flMatVal[2][0] = m20;	m_flMatVal[2][1] = m21; m_flMatVal[2][2] = m22; m_flMatVal[2][3] = m23;
}

matrix3x4_t::matrix3x4_t(const Vector& xAxis, const Vector& yAxis, const Vector& zAxis, const Vector &vecOrigin)
{
	Init(xAxis, yAxis, zAxis, vecOrigin);
}

void matrix3x4_t::Init(const Vector& xAxis, const Vector& yAxis, const Vector& zAxis, const Vector &vecOrigin)
{
	m_flMatVal[0][0] = xAxis.x; m_flMatVal[0][1] = yAxis.x; m_flMatVal[0][2] = zAxis.x; m_flMatVal[0][3] = vecOrigin.x;
	m_flMatVal[1][0] = xAxis.y; m_flMatVal[1][1] = yAxis.y; m_flMatVal[1][2] = zAxis.y; m_flMatVal[1][3] = vecOrigin.y;
	m_flMatVal[2][0] = xAxis.z; m_flMatVal[2][1] = yAxis.z; m_flMatVal[2][2] = zAxis.z; m_flMatVal[2][3] = vecOrigin.z;
}


/*==============================================================================*/
//	matrix3x4_t
/*==============================================================================*/
VMatrix::VMatrix()
{
}

VMatrix::VMatrix(vec_t m00, vec_t m01, vec_t m02, vec_t m03, vec_t m10, vec_t m11, vec_t m12, vec_t m13, vec_t m20, vec_t m21, vec_t m22, vec_t m23, vec_t m30, vec_t m31, vec_t m32, vec_t m33)
{
	Init(
		m00, m01, m02, m03,
		m10, m11, m12, m13,
		m20, m21, m22, m23,
		m30, m31, m32, m33
	);
}

VMatrix::VMatrix(const matrix3x4_t& matrix3x4)
{
	Init(matrix3x4);
}

VMatrix::VMatrix(const Vector& xAxis, const Vector& yAxis, const Vector& zAxis)
{
	Init(
		xAxis.x, yAxis.x, zAxis.x, 0.0f,
		xAxis.y, yAxis.y, zAxis.y, 0.0f,
		xAxis.z, yAxis.z, zAxis.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

VMatrix::VMatrix(const Vector& xAxis, const Vector& yAxis, const Vector& zAxis, const Vector& translation)
{
	Init(
		xAxis.x, yAxis.x, zAxis.x, translation.x,
		xAxis.y, yAxis.y, zAxis.y, translation.y,
		xAxis.z, yAxis.z, zAxis.z, translation.z,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

void VMatrix::Init(vec_t m00, vec_t m01, vec_t m02, vec_t m03, vec_t m10, vec_t m11, vec_t m12, vec_t m13, vec_t m20, vec_t m21, vec_t m22, vec_t m23, vec_t m30, vec_t m31, vec_t m32, vec_t m33)
{
	m[0][0] = m00;
	m[0][1] = m01;
	m[0][2] = m02;
	m[0][3] = m03;

	m[1][0] = m10;
	m[1][1] = m11;
	m[1][2] = m12;
	m[1][3] = m13;

	m[2][0] = m20;
	m[2][1] = m21;
	m[2][2] = m22;
	m[2][3] = m23;

	m[3][0] = m30;
	m[3][1] = m31;
	m[3][2] = m32;
	m[3][3] = m33;
}

void VMatrix::Init(const matrix3x4_t& matrix3x4)
{
	memcpy(m, matrix3x4.Base(), sizeof(matrix3x4_t));

	m[3][0] = 0.0f;
	m[3][1] = 0.0f;
	m[3][2] = 0.0f;
	m[3][3] = 1.0f;
}


/*==============================================================================*/
//	Vector3D
/*==============================================================================*/
Vector::Vector()
{
}

Vector::Vector(vec_t X, vec_t Y, vec_t Z)
{
	x = X; y = Y; z = Z;
}

void Vector::Init(vec_t ix, vec_t iy, vec_t iz)
{
	x = ix; y = iy; z = iz;
}

vec_t& Vector::operator[](int i)
{
	return ((vec_t*)this)[i];
}

vec_t Vector::operator[](int i) const
{
	return ((vec_t*)this)[i];
}

bool Vector::operator==(const Vector& src) const
{
	return (src.x == x) && (src.y == y) && (src.z == z);
}

bool Vector::operator!=(const Vector& src) const
{
	return (src.x != x) || (src.y != y) || (src.z != z);
}

void Vector::Negate()
{
	x = -x; y = -y; z = -z;
}

Vector& Vector::operator+=(const Vector& v)
{
	x += v.x;
	y += v.y;
	z += v.z;

	return *this;
}

Vector& Vector::operator-=(const Vector& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;

	return *this;
}

Vector& Vector::operator*=(float fl)
{
	x *= fl;
	y *= fl;
	z *= fl;

	return *this;
}

Vector& Vector::operator*=(const Vector& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;

	return *this;
}

Vector& Vector::operator/=(float fl)
{
	float oofl = 1.0f / fl;
	x *= oofl;
	y *= oofl;
	z *= oofl;

	return *this;
}

Vector& Vector::operator/=(const Vector& v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;

	return *this;
}

vec_t Vector::Dot(const Vector& vOther) const
{
	return DotProduct(*this, vOther);
}

vec_t Vector::Length() const
{
	return VectorLength(*this);
}

vec_t Vector::LengthSqr() const
{
	return (x*x + y * y + z * z);
}

vec_t Vector::DistTo(const Vector &vOther) const
{
	Vector delta;
	VectorSubtract(*this, vOther, delta);

	return delta.Length();
}

vec_t Vector::DistToSqr(const Vector &vOther) const
{
	Vector delta;

	delta.x = x - vOther.x;
	delta.y = y - vOther.y;
	delta.z = z - vOther.z;

	return delta.LengthSqr();
}

Vector Vector::operator+(const Vector& v) const
{
	Vector res;
	VectorAdd(*this, v, res);

	return res;
}

Vector Vector::operator-(const Vector& v) const
{
	Vector res;
	VectorSubtract(*this, v, res);

	return res;
}

Vector Vector::operator*(float fl) const
{
	Vector res;
	VectorMultiply(*this, fl, res);

	return res;
}

Vector Vector::operator*(const Vector& v) const
{
	Vector res;
	VectorMultiply(*this, v, res);

	return res;
}

Vector Vector::operator/(float fl) const
{
	Vector res;
	VectorDivide(*this, fl, res);

	return res;
}

Vector Vector::operator/(const Vector& v) const
{
	Vector res;
	VectorDivide(*this, v, res);

	return res;
}

Vector operator*(float fl, const Vector& v)
{
	return v * fl;
}

Vector Vector::Cross(const Vector& vOther) const
{
	Vector res;
	CrossProduct(*this, vOther, res);

	return res;
}

vec_t Vector::Length2D(void) const
{
	return (vec_t)FastSqrt(x*x + y * y);
}

vec_t Vector::Length2DSqr(void) const
{
	return (x*x + y * y);
}


/*==============================================================================*/
//	QAngle
/*==============================================================================*/
QAngle::QAngle(void)
{
}

QAngle::QAngle(vec_t X, vec_t Y, vec_t Z)
{
	x = X; y = Y; z = Z;
}

void QAngle::Init(vec_t ix, vec_t iy, vec_t iz)
{
	x = ix;
	y = iy;
	z = iz;
}

vec_t& QAngle::operator[](int i)
{
	return ((vec_t*)this)[i];
}

vec_t QAngle::operator[](int i) const
{
	return ((vec_t*)this)[i];
}

bool QAngle::operator==(const QAngle& src) const
{
	return (src.x == x) && (src.y == y) && (src.z == z);
}

bool QAngle::operator!=(const QAngle& src) const
{
	return (src.x != x) || (src.y != y) || (src.z != z);
}

QAngle& QAngle::operator+=(const QAngle& v)
{
	x += v.x; y += v.y; z += v.z;

	return *this;
}

QAngle& QAngle::operator-=(const QAngle& v)
{
	x -= v.x; y -= v.y; z -= v.z;

	return *this;
}

QAngle& QAngle::operator*=(float fl)
{
	x *= fl;
	y *= fl;
	z *= fl;

	return *this;
}

QAngle& QAngle::operator/=(float fl)
{
	float oofl = 1.0f / fl;
	x *= oofl;
	y *= oofl;
	z *= oofl;

	return *this;
}

vec_t QAngle::Length() const
{
	return (vec_t)FastSqrt(LengthSqr());
}

vec_t QAngle::LengthSqr() const
{
	return x * x + y * y + z * z;
}

QAngle QAngle::operator+(const QAngle& v) const
{
	QAngle res;
	res.x = x + v.x;
	res.y = y + v.y;
	res.z = z + v.z;

	return res;
}

QAngle QAngle::operator-(const QAngle& v) const
{
	QAngle res;
	res.x = x - v.x;
	res.y = y - v.y;
	res.z = z - v.z;

	return res;
}

QAngle QAngle::operator*(float fl) const
{
	QAngle res;
	res.x = x * fl;
	res.y = y * fl;
	res.z = z * fl;

	return res;
}

QAngle QAngle::operator/(float fl) const
{
	QAngle res;
	res.x = x / fl;
	res.y = y / fl;
	res.z = z / fl;

	return res;
}

QAngle operator*(float fl, const QAngle& v)
{
	QAngle ret(v * fl);
	return ret;
}