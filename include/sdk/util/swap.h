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

#endif