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

	void StartReading(const void *pData, int nBytes, int iStartBit = 0, int nBits = -1);
	void Reset();
	
	int ReadOneBitNoCheck();
	int ReadOneBit();
	
	const unsigned char* GetBasePointer() { return m_pData; }

	void ReadBits(void *pOut, int nBits);
	
	unsigned int ReadUBitLong(int numbits);
	unsigned int PeekUBitLong(int numbits);
	int ReadSBitLong(int numbits);
	
	uint32 ReadVarInt32();
	//uint64 ReadVarInt64();
	int32 ReadSignedVarInt32();
	//int64 ReadSignedVarInt64();

	unsigned int ReadBitLong(int numbits, bool bSigned);

	float ReadBitCoord();
	//float ReadBitCoordMP(bool bIntegral, bool bLowPrecision);
	float ReadBitFloat();
	float ReadBitNormal();
	void ReadBitVec3Coord(Vector& fa);
	void ReadBitVec3Normal(Vector& fa);
	void ReadBitAngles(QAngle& fa);

	//unsigned int ReadBitCoordBits();
	//unsigned int ReadBitCoordMPBits(bool bIntegral, bool bLowPrecision);

	int	ReadChar();
	int	ReadByte();
	int	ReadShort();
	int	ReadWord();
	long ReadLong();
	//int64 ReadLongLong();
	float ReadFloat();
	bool ReadBytes(void *pOut, int nBytes);

	bool ReadString(char *pStr, int bufLen, bool bLine = false, int *pOutNumChars = NULL);

	char* ReadAndAllocateString(bool *pOverflow = 0);

	int GetNumBytesLeft();
	int GetNumBytesRead();
	int GetNumBitsLeft();
	int GetNumBitsRead() const;

	inline bool Seek(int iBit);
	inline bool SeekRelative(int iBitDelta);

	const unsigned char* m_pData;
	int m_nDataBytes;
	int m_nDataBits;
	int m_iCurBit;
};

#endif