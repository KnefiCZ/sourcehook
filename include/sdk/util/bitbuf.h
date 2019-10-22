#ifndef SDK_BITBUF_H
#define SDK_BITBUF_H
#pragma once

#include "../common.h"

class Vector;
class QAngle;

extern unsigned long g_LittleBits[32];
extern unsigned long g_BitWriteMasks[32][33];
extern unsigned long g_ExtraMasks[33];

class bf_write
{
public:
	bf_write();
	bf_write(void *pData, int nBytes, int nMaxBits = -1);

	void StartWriting(void *pData, int nBytes, int iStartBit = 0, int nMaxBits = -1);
	void Reset();

	unsigned char* GetBasePointer() { return (unsigned char*)m_pData; }
	
	void SeekToBit(int bitPos);
	
	void WriteOneBit(int nValue);
	void WriteOneBitNoCheck(int nValue);
	void WriteOneBitAt(int iBit, int nValue);

	void WriteUBitLong(unsigned int data, int numbits, bool bCheckRange = true);
	void WriteSBitLong(int data, int numbits);

	void WriteBitLong(unsigned int data, int numbits, bool bSigned);

	bool WriteBits(const void *pIn, int nBits);

	void WriteUBitVar(unsigned int data);

	void WriteVarInt32(uint32 data);
	//void WriteVarInt64(uint64 data);
	void WriteSignedVarInt32(int32 data);
	//void WriteSignedVarInt64(int64 data);
	int ByteSizeVarInt32(uint32 data);
	//int ByteSizeVarInt64(uint64 data);
	int ByteSizeSignedVarInt32(int32 data);
	//int ByteSizeSignedVarInt64(int64 data);

	bool WriteBitsFromBuffer(class bf_read *pIn, int nBits);
	
	void WriteBitCoord(const float f);
	void WriteBitFloat(float val);
	void WriteBitVec3Coord(const Vector& fa);
	void WriteBitNormal(float f);
	void WriteBitVec3Normal(const Vector& fa);
	void WriteBitAngles(const QAngle& fa);

	void WriteChar(int val);
	void WriteByte(int val);
	void WriteShort(int val);
	void WriteWord(int val);
	void WriteLong(long val);
	//void WriteLongLong(int64 val);
	void WriteFloat(float val);
	bool WriteBytes(const void *pBuf, int nBytes);

	bool WriteString(const char *pStr);

	int GetNumBytesWritten() const;
	int GetNumBitsWritten() const;
	int GetMaxNumBits();
	int GetNumBitsLeft();
	int GetNumBytesLeft();

	unsigned char* GetData();
	const unsigned char* GetData() const;
	
	unsigned long*	m_pData;
	int m_nDataBytes;
	int m_nDataBits;
	int m_iCurBit;
};

class bf_read
{
public:
	bf_read();
	bf_read(const void *pData, int nBytes, int nBits = -1);

	void			StartReading(const void *pData, int nBytes, int iStartBit = 0, int nBits = -1);
	void			Reset();

	void			ExciseBits(int startbit, int bitstoremove);

	int				ReadOneBit();

	unsigned int	CheckReadUBitLong(int numbits);
	int				ReadOneBitNoCheck();
	bool			CheckForOverflow(int nBits);

	const unsigned char*	GetBasePointer() { return m_pData; }

	FORCEINLINE int TotalBytesAvailable(void) const
	{
		return m_nDataBytes;
	}

	void            ReadBits(void *pOut, int nBits);

	int             ReadBitsClamped_ptr(void *pOut, size_t outSizeBytes, size_t nBits);

	template <typename T, size_t N>
	int ReadBitsClamped(T(&pOut)[N], size_t nBits)
	{
		return ReadBitsClamped_ptr(pOut, N * sizeof(T), nBits);
	}

	float			ReadBitAngle(int numbits);

	unsigned int	ReadUBitLong(int numbits);
	unsigned int	ReadUBitLongNoInline(int numbits);
	unsigned int	PeekUBitLong(int numbits);
	int				ReadSBitLong(int numbits);

	unsigned int	ReadUBitVar();
	unsigned int	ReadUBitVarInternal(int encodingType);

	uint32			ReadVarInt32();
	uint64			ReadVarInt64();
	int32			ReadSignedVarInt32();
	int64			ReadSignedVarInt64();

	unsigned int	ReadBitLong(int numbits, bool bSigned);

	float			ReadBitCoord();
	float			ReadBitCoordMP(bool bIntegral, bool bLowPrecision);
	float			ReadBitFloat();
	float			ReadBitNormal();
	void			ReadBitVec3Coord(Vector& fa);
	void			ReadBitVec3Normal(Vector& fa);
	void			ReadBitAngles(QAngle& fa);

	unsigned int	ReadBitCoordBits();
	unsigned int	ReadBitCoordMPBits(bool bIntegral, bool bLowPrecision);

	FORCEINLINE int	ReadChar() { return (char)ReadUBitLong(8); }
	FORCEINLINE int	ReadByte() { return ReadUBitLong(8); }
	FORCEINLINE int	ReadShort() { return (short)ReadUBitLong(16); }
	FORCEINLINE int	ReadWord() { return ReadUBitLong(16); }
	FORCEINLINE long ReadLong() { return ReadUBitLong(32); }
	int64			ReadLongLong();
	float			ReadFloat();
	bool			ReadBytes(void *pOut, int nBytes);

	bool			ReadString(char *pStr, int bufLen, bool bLine = false, int *pOutNumChars = NULL);

	char*			ReadAndAllocateString(bool *pOverflow = 0);

	int				CompareBits(bf_read* other, int bits);
	int				CompareBitsAt(int offset, bf_read* other, int otherOffset, int bits);

	int				GetNumBytesLeft();
	int				GetNumBytesRead();
	int				GetNumBitsLeft();
	int				GetNumBitsRead() const;

	inline bool		Seek(int iBit);
	inline bool		SeekRelative(int iBitDelta);

	const unsigned char*	m_pData;
	int						m_nDataBytes;
	int						m_nDataBits;
	int				m_iCurBit;
};

inline int bf_read::GetNumBytesRead()
{
	return BitByte(m_iCurBit);
}

inline int bf_read::GetNumBitsLeft()
{
	return m_nDataBits - m_iCurBit;
}

inline int bf_read::GetNumBytesLeft()
{
	return GetNumBitsLeft() >> 3;
}

inline int bf_read::GetNumBitsRead() const
{
	return m_iCurBit;
}

inline bool bf_read::Seek(int iBit)
{
	if (iBit < 0 || iBit > m_nDataBits)
	{
		SetOverflowFlag();
		m_iCurBit = m_nDataBits;
		return false;
	}
	else
	{
		m_iCurBit = iBit;
		return true;
	}
}

inline bool	bf_read::SeekRelative(int iBitDelta)
{
	return Seek(m_iCurBit + iBitDelta);
}

inline bool bf_read::CheckForOverflow(int nBits)
{
	if (m_iCurBit + nBits > m_nDataBits)
	{
		SetOverflowFlag();
	}

	return m_bOverflow;
}

inline int bf_read::ReadOneBitNoCheck()
{
	unsigned char value = m_pData[m_iCurBit >> 3] >> (m_iCurBit & 7);

	++m_iCurBit;
	return value & 1;
}

inline int bf_read::ReadOneBit()
{
	if (GetNumBitsLeft() <= 0)
	{
		SetOverflowFlag();
		return 0;
	}
	return ReadOneBitNoCheck();
}

inline float bf_read::ReadBitFloat()
{
	union { uint32 u; float f; } c = { ReadUBitLong(32) };
	return c.f;
}

FORCEINLINE unsigned int bf_read::ReadUBitVar()
{
	unsigned int sixbits = ReadUBitLong(6);
	unsigned int encoding = sixbits & 3;
	if (encoding)
	{
		return ReadUBitVarInternal(encoding);
	}
	return sixbits >> 2;
}

FORCEINLINE unsigned int bf_read::ReadUBitLong(int numbits)
{
	if (GetNumBitsLeft() < numbits)
	{
		m_iCurBit = m_nDataBits;
		SetOverflowFlag();
		return 0;
	}

	unsigned int iStartBit = m_iCurBit & 31u;
	int iLastBit = m_iCurBit + numbits - 1;
	unsigned int iWordOffset1 = m_iCurBit >> 5;
	unsigned int iWordOffset2 = iLastBit >> 5;
	m_iCurBit += numbits;

	extern unsigned long g_ExtraMasks[33];
	unsigned int bitmask = g_ExtraMasks[numbits];

	unsigned int dw1 = LoadLittleDWord((unsigned long*)m_pData, iWordOffset1) >> iStartBit;
	unsigned int dw2 = LoadLittleDWord((unsigned long*)m_pData, iWordOffset2) << (32 - iStartBit);

	return (dw1 | dw2) & bitmask;
}

FORCEINLINE int bf_read::CompareBits(bf_read* other, int numbits)
{
	return (ReadUBitLong(numbits) != other->ReadUBitLong(numbits));
}

#endif