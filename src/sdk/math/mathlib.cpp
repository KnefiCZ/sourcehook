/*==============================================================================*/
#include <limits>
#include <math.h>
#include <xmmintrin.h>

#include "sdk/math/math_pfns.h"
#include "sdk/math/mathlib.h"
#include "sdk/math/vector.h"
#include "sdk/math/vector2d.h"
#include "sdk/math/qangle.h"
/*==============================================================================*/


/*==============================================================================*/
/*							Math Performance Functions							*/
/*==============================================================================*/
float FastSqrt(float x)
{
	__m128 root = _mm_sqrt_ss(_mm_load_ss(&x));
	return *(reinterpret_cast<float *>(&root));
}

float FastRSqrtFast(float x)
{
	__m128 rroot = _mm_rsqrt_ss(_mm_load_ss(&x));
	return *(reinterpret_cast<float *>(&rroot));
}

float FastRSqrt(float x)
{
	float rroot = FastRSqrtFast(x);
	return (0.5f * rroot) * (3.f - (x * rroot) * rroot);
}

void FastSinCos(float x, float* s, float* c)
{
	float t4, t8, t12;

	__asm
	{
		movss	xmm0, x
		movss	t12, xmm0
		movss	xmm1, _ps_am_inv_sign_mask
		mov		eax, t12
		mulss	xmm0, _ps_am_2_o_pi
		andps	xmm0, xmm1
		and		eax, 0x80000000

		cvttss2si	edx, xmm0
		mov		ecx, edx
		mov		t12, esi
		mov		esi, edx
		add		edx, 0x1
		shl		ecx, (31 - 1)
		shl		edx, (31 - 1)

		movss	xmm4, _ps_am_1
		cvtsi2ss	xmm3, esi
		mov		t8, eax
		and		esi, 0x1

		subss	xmm0, xmm3
		movss	xmm3, _sincos_inv_masks[esi * 4]
		minss	xmm0, xmm4

		subss	xmm4, xmm0

		movss	xmm6, xmm4
		andps	xmm4, xmm3
		and		ecx, 0x80000000
		movss	xmm2, xmm3
		andnps	xmm3, xmm0
		and		edx, 0x80000000
		movss	xmm7, t8
		andps	xmm0, xmm2
		mov		t8, ecx
		mov		t4, edx
		orps	xmm4, xmm3

		mov		eax, s
		mov		edx, c

		andnps	xmm2, xmm6
		orps	xmm0, xmm2

		movss	xmm2, t8
		movss	xmm1, xmm0
		movss	xmm5, xmm4
		xorps	xmm7, xmm2
		movss	xmm3, _ps_sincos_p3
		mulss	xmm0, xmm0
		mulss	xmm4, xmm4
		movss	xmm2, xmm0
		movss	xmm6, xmm4
		orps	xmm1, xmm7
		movss	xmm7, _ps_sincos_p2
		mulss	xmm0, xmm3
		mulss	xmm4, xmm3
		movss	xmm3, _ps_sincos_p1
		addss	xmm0, xmm7
		addss	xmm4, xmm7
		movss	xmm7, _ps_sincos_p0
		mulss	xmm0, xmm2
		mulss	xmm4, xmm6
		addss	xmm0, xmm3
		addss	xmm4, xmm3
		movss	xmm3, t4
		mulss	xmm0, xmm2
		mulss	xmm4, xmm6
		orps	xmm5, xmm3
		mov		esi, t12
		addss	xmm0, xmm7
		addss	xmm4, xmm7
		mulss	xmm0, xmm1
		mulss	xmm4, xmm5
		
		movss[eax], xmm0
		movss[edx], xmm4
	}
}

float FastCos(float x)
{
	float temp;

	__asm
	{
		movss	xmm0, x
		movss	xmm1, _ps_am_inv_sign_mask
		andps	xmm0, xmm1
		addss	xmm0, _ps_am_pi_o_2
		mulss	xmm0, _ps_am_2_o_pi

		cvttss2si	ecx, xmm0
		movss	xmm5, _ps_am_1
		mov		edx, ecx
		shl		edx, (31 - 1)
		cvtsi2ss	xmm1, ecx
		and		edx, 0x80000000
		and ecx, 0x1

		subss	xmm0, xmm1
		movss	xmm6, _sincos_masks[ecx * 4]
		minss	xmm0, xmm5

		movss	xmm1, _ps_sincos_p3
		subss	xmm5, xmm0

		andps	xmm5, xmm6
		movss	xmm7, _ps_sincos_p2
		andnps	xmm6, xmm0
		mov		temp, edx
		orps	xmm5, xmm6
		movss	xmm0, xmm5

		mulss	xmm5, xmm5
		movss	xmm4, _ps_sincos_p1
		movss	xmm2, xmm5
		mulss	xmm5, xmm1
		movss	xmm1, _ps_sincos_p0
		addss	xmm5, xmm7
		mulss	xmm5, xmm2
		movss	xmm3, temp
		addss	xmm5, xmm4
		mulss	xmm5, xmm2
		orps	xmm0, xmm3
		addss	xmm5, xmm1
		mulss	xmm0, xmm5

		movss   x, xmm0

	}
}

/*==============================================================================*/
/*									Mathlib										*/
/*==============================================================================*/




/*==============================================================================*/
/*									Vector2D									*/
/*==============================================================================*/
Vector2D::Vector2D(void)
{
}

Vector2D::Vector2D(vec_t X, vec_t Y)
{
	x = X; y = Y;
}

void Vector2D::Init(vec_t ix, vec_t iy)
{
	x = ix; y = iy;
}

vec_t Vector2D::operator[](int i) const
{
	return ((vec_t*)this)[i];
}

vec_t& Vector2D::operator[](int i)
{
	return ((vec_t*)this)[i];
}

bool Vector2D::operator==(Vector2D& src)
{
	return (src.x == x) && (src.y == y);
}

bool Vector2D::operator!=(Vector2D& src)
{
	return (src.x != x) || (src.y != y);
}

void Vector2D::Negate()
{
	x = -x; y = -y;
}

Vector2D& Vector2D::operator+=(const Vector2D& v)
{
	x += v.x;
	y += v.y;

	return *this;
}

Vector2D& Vector2D::operator-=(const Vector2D& v)
{
	x -= v.x;
	y -= v.y;

	return *this;
}

Vector2D& Vector2D::operator*=(float fl)
{
	x *= fl;
	y *= fl;

	return *this;
}

Vector2D& Vector2D::operator*=(const Vector2D& v)
{
	x *= v.x;
	y *= v.y;

	return *this;
}

Vector2D& Vector2D::operator/=(float fl)
{
	float oofl = 1.0f / fl;
	x *= oofl;
	y *= oofl;

	return *this;
}

Vector2D& Vector2D::operator/=(const Vector2D& v)
{
	x /= v.x;
	y /= v.y;

	return *this;
}

vec_t Vector2D::Dot(Vector2D& vOther) const
{
	return DotProduct2D(*this, vOther);
}

vec_t Vector2D::LengthSqr() const
{
	return (x*x + y * y);
}

vec_t Vector2D::Length() const
{
	return Vector2DLength(*this);
}

vec_t Vector2D::DistTo(const Vector2D &vOther) const
{
	Vector2D delta;
	Vector2DSubtract(*this, vOther, delta);

	return delta.Length();
}

vec_t Vector2D::DistToSqr(const Vector2D &vOther) const
{
	Vector2D delta;
	Vector2DSubtract(*this, vOther, delta);

	return delta.LengthSqr();
}

Vector2D Vector2D::operator+(const Vector2D& v) const
{
	Vector2D res;
	Vector2DAdd(*this, v, res);

	return res;
}

Vector2D Vector2D::operator-(const Vector2D& v) const
{
	Vector2D res;
	Vector2DSubtract(*this, v, res);

	return res;
}

Vector2D Vector2D::operator*(float fl) const
{
	Vector2D res;
	Vector2DMultiply(*this, fl, res);

	return res;
}

Vector2D Vector2D::operator*(const Vector2D& v) const
{
	Vector2D res;
	Vector2DMultiply(*this, v, res);

	return res;
}

Vector2D Vector2D::operator/(float fl) const
{
	Vector2D res;
	Vector2DDivide(*this, fl, res);

	return res;
}

Vector2D Vector2D::operator/(const Vector2D& v) const
{
	Vector2D res;
	Vector2DDivide(*this, v, res);

	return res;
}

Vector2D operator*(float fl, const Vector2D& v)
{
	return v * fl;
}

void Vector2DCopy(Vector2D& src, Vector2D& dst)
{
	dst.x = src.x;
	dst.y = src.y;
}

void Vector2DAdd(Vector2D& a, Vector2D& b, Vector2D& c)
{
	c.x = a.x + b.x;
	c.y = a.y + b.y;
}

void Vector2DSubtract(Vector2D& a, Vector2D& b, Vector2D& c)
{
	c.x = a.x - b.x;
	c.y = a.y - b.y;
}

void Vector2DMultiply(Vector2D& a, vec_t b, Vector2D& c)
{
	c.x = a.x * b;
	c.y = a.y * b;
}

void Vector2DMultiply(Vector2D& a, Vector2D& b, Vector2D& c)
{
	c.x = a.x * b.x;
	c.y = a.y * b.y;
}

void Vector2DDivide(Vector2D& a, vec_t b, Vector2D& c)
{
	vec_t oob = 1.0f / b;
	c.x = a.x * oob;
	c.y = a.y * oob;
}

void Vector2DDivide(Vector2D& a, Vector2D& b, Vector2D& c)
{
	c.x = a.x / b.x;
	c.y = a.y / b.y;
}

void Vector2DMA(Vector2D& start, float s, Vector2D& dir, Vector2D& result)
{
	result.x = start.x + s * dir.x;
	result.y = start.y + s * dir.y;
}

void Vector2DLerp(Vector2D& src1, Vector2D& src2, vec_t t, Vector2D& dest)
{
	dest[0] = src1[0] + (src2[0] - src1[0]) * t;
	dest[1] = src1[1] + (src2[1] - src1[1]) * t;
}

vec_t DotProduct2D(Vector2D& a, Vector2D& b)
{
	return( (a.x * b.x) + (a.y * b.y) );
}

vec_t Vector2DLength(Vector2D& v)
{
	return (vec_t)FastSqrt(v.x*v.x + v.y*v.y);
}


/*==============================================================================*/
/*									Vector3D									*/
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

void VectorCopy(Vector& src, Vector& dst)
{
	dst.x = src.x;
	dst.y = src.y;
	dst.z = src.z;
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

void VectorAdd(Vector& a, Vector& b, Vector& c)
{
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	c.z = a.z + b.z;
}

void VectorSubtract(Vector& a, Vector& b, Vector& c)
{
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	c.z = a.z - b.z;
}

void VectorMultiply(Vector& a, vec_t b, Vector& c)
{
	c.x = a.x * b;
	c.y = a.y * b;
	c.z = a.z * b;
}

void VectorMultiply(Vector& a, Vector& b, Vector& c)
{
	c.x = a.x * b.x;
	c.y = a.y * b.y;
	c.z = a.z * b.z;
}

void VectorScale(Vector& in, vec_t scale, Vector& result)
{
	VectorMultiply(in, scale, result);
}

void VectorDivide(Vector& a, vec_t b, Vector& c)
{
	vec_t oob = 1.0f / b;
	c.x = a.x * oob;
	c.y = a.y * oob;
	c.z = a.z * oob;
}

void VectorDivide(Vector& a, Vector& b, Vector& c)
{
	c.x = a.x / b.x;
	c.y = a.y / b.y;
	c.z = a.z / b.z;
}

void VectorLerp(Vector& src1, Vector& src2, vec_t t, Vector& dest)
{
	dest.x = src1.x + (src2.x - src1.x) * t;
	dest.y = src1.y + (src2.y - src1.y) * t;
	dest.z = src1.z + (src2.z - src1.z) * t;
}

Vector VectorLerp(Vector& src1, Vector& src2, vec_t t)
{
	Vector result;
	VectorLerp(src1, src2, t, result);

	return result;
}

vec_t DotProduct(Vector& a, Vector& b)
{
	return(a.x*b.x + a.y*b.y + a.z*b.z);
}

void CrossProduct(Vector& a, Vector& b, Vector& result)
{
	result.x = a.y*b.z - a.z*b.y;
	result.y = a.z*b.x - a.x*b.z;
	result.z = a.x*b.y - a.y*b.x;
}

vec_t DotProductAbs(Vector &v0, Vector &v1)
{
	return FloatMakePositive(v0.x*v1.x) + FloatMakePositive(v0.y*v1.y) + FloatMakePositive(v0.z*v1.z);
}

vec_t DotProductAbs(Vector &v0, float *v1)
{
	return FloatMakePositive(v0.x * v1[0]) + FloatMakePositive(v0.y * v1[1]) + FloatMakePositive(v0.z * v1[2]);
}

vec_t VectorLength(Vector& v)
{
	return (vec_t)FastSqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

Vector CrossProduct(Vector& a, Vector& b)
{
	return Vector(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
}


/*==============================================================================*/
/*  								  QAngle									*/
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

void AngleCopy(QAngle& src, QAngle& dst)
{
	dst.x = src.x;
	dst.y = src.y;
	dst.z = src.z;
}