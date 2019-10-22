#ifndef SDK_SWAP_H
#define SDK_SWAP_H
#pragma once

#define WordSwap  WordSwapAsm
#define DWordSwap DWordSwapAsm

#pragma warning(push)
#pragma warning (disable:4035)

template <typename T>
inline T WordSwapAsm(T w)
{
	__asm
	{
		mov ax, w
		xchg al, ah
	}
}

template <typename T>
inline T DWordSwapAsm(T dw)
{
	__asm
	{
		mov eax, dw
		bswap eax
	}
}

#define SafeSwapFloat( pOut, pIn )	(*((uint*)pOut) = DWordSwap( *((uint*)pIn) ))

#define BigShort( val )				WordSwap( val )
#define BigWord( val )				WordSwap( val )
#define BigLong( val )				DWordSwap( val )
#define BigDWord( val )				DWordSwap( val )
#define LittleShort( val )			( val )
#define LittleWord( val )			( val )
#define LittleLong( val )			( val )
#define LittleDWord( val )			( val )
#define SwapShort( val )			BigShort( val )
#define SwapWord( val )				BigWord( val )
#define SwapLong( val )				BigLong( val )
#define SwapDWord( val )			BigDWord( val )

#define BigFloat( pOut, pIn )		SafeSwapFloat( pOut, pIn )
#define LittleFloat( pOut, pIn )	( *pOut = *pIn )
#define SwapFloat( pOut, pIn )		BigFloat( pOut, pIn )

inline unsigned long LoadLittleDWord(unsigned long *base, unsigned int dwordIndex)
{
	return LittleDWord(base[dwordIndex]);
}

inline void StoreLittleDWord(unsigned long *base, unsigned int dwordIndex, unsigned long dword)
{
	base[dwordIndex] = LittleDWord(dword);
}

#define BITS_PER_INT 32

inline int GetBitForBitnum(int bitNum)
{
	static int bitsForBitnum[] =
	{
		(1 << 0),
		(1 << 1),
		(1 << 2),
		(1 << 3),
		(1 << 4),
		(1 << 5),
		(1 << 6),
		(1 << 7),
		(1 << 8),
		(1 << 9),
		(1 << 10),
		(1 << 11),
		(1 << 12),
		(1 << 13),
		(1 << 14),
		(1 << 15),
		(1 << 16),
		(1 << 17),
		(1 << 18),
		(1 << 19),
		(1 << 20),
		(1 << 21),
		(1 << 22),
		(1 << 23),
		(1 << 24),
		(1 << 25),
		(1 << 26),
		(1 << 27),
		(1 << 28),
		(1 << 29),
		(1 << 30),
		(1 << 31),
	};

	return bitsForBitnum[(bitNum) & (BITS_PER_INT - 1)];
}

inline unsigned long& FloatBits(vec_t& f)
{
	return *reinterpret_cast<unsigned long*>(&f);
}

inline unsigned long const& FloatBits(vec_t const& f)
{
	return *reinterpret_cast<unsigned long const*>(&f);
}

inline vec_t BitsToFloat(unsigned long i)
{
	return *reinterpret_cast<vec_t*>(&i);
}

inline bool IsFinite(vec_t f)
{
	return ((FloatBits(f) & 0x7F800000) != 0x7F800000);
}

inline unsigned long FloatAbsBits(vec_t f)
{
	return FloatBits(f) & 0x7FFFFFFF;
}

inline float FloatMakeNegative(vec_t f)
{
	return BitsToFloat(FloatBits(f) | 0x80000000);
}

inline float FloatMakePositive(vec_t f)
{
	return (float)fabs(f);
}

inline float FloatNegate(vec_t f)
{
	return BitsToFloat(FloatBits(f) ^ 0x80000000);
}

inline bool IsPowerOfTwo(int value)
{
	return (value & (value - 1)) == 0;
}

#endif