/*==============================================================================*/
#include "sdk/util/bitbuf.h"
#include "sdk/util/bitbytes.h"
#include "sdk/math/vector.h"
#include "sdk/math/qangle.h"

#include <string.h>
/*==============================================================================*/

static int BitByte(int bits)
{
	return (bits + 7) >> 3;
}

static uint32 ZigZagEncode32(int32 n)
{
	// Note:  the right-shift must be arithmetic
	return(n << 1) ^ (n >> 31);
}

static int32 ZigZagDecode32(uint32 n)
{
	return(n >> 1) ^ -static_cast<int32>(n & 1);
}

static uint64 ZigZagEncode64(int64 n)
{
	// Note:  the right-shift must be arithmetic
	return(n << 1) ^ (n >> 63);
}

static int64 ZigZagDecode64(uint64 n)
{
	return(n >> 1) ^ -static_cast<int64>(n & 1);
}

static unsigned long g_LittleBits[32];
static unsigned long g_BitWriteMasks[32][33];
static unsigned long g_ExtraMasks[33];

class CBitWriteMasksInit
{
public:
	CBitWriteMasksInit()
	{
		for (unsigned int startbit = 0; startbit < 32; startbit++)
		{
			for (unsigned int nBitsLeft = 0; nBitsLeft < 33; nBitsLeft++)
			{
				unsigned int endbit = startbit + nBitsLeft;
				g_BitWriteMasks[startbit][nBitsLeft] = GetBitForBitnum(startbit) - 1;
				if (endbit < 32)
					g_BitWriteMasks[startbit][nBitsLeft] |= ~(GetBitForBitnum(endbit) - 1);
			}
		}

		for (unsigned int maskBit = 0; maskBit < 32; maskBit++)
			g_ExtraMasks[maskBit] = GetBitForBitnum(maskBit) - 1;
		g_ExtraMasks[32] = ~0ul;

		for (unsigned int littleBit = 0; littleBit < 32; littleBit++)
			StoreLittleDWord(&g_LittleBits[littleBit], 0, 1u << littleBit);
	}
};

static CBitWriteMasksInit g_BitWriteMasksInit;

static const int kMaxVarintBytes = 10;
static const int kMaxVarint32Bytes = 5;

/*==============================================================================*/
//	bf_write
/*==============================================================================*/
bf_write::bf_write()
{
	m_pData = NULL;
	m_nDataBytes = 0;
	m_nDataBits = -1;
	m_iCurBit = 0;
}

bf_write::bf_write(void *pData, int nBytes, int nBits)
{
	StartWriting(pData, nBytes, 0, nBits);
}

void bf_write::StartWriting(void *pData, int nBytes, int iStartBit, int nBits)
{
	nBytes &= ~3;

	m_pData = (unsigned long*)pData;
	m_nDataBytes = nBytes;

	if (nBits == -1)
	{
		m_nDataBits = nBytes << 3;
	}
	else
	{
		m_nDataBits = nBits;
	}

	m_iCurBit = iStartBit;
}

void bf_write::Reset()
{
	m_iCurBit = 0;
}

void bf_write::SeekToBit(int bitPos)
{
	m_iCurBit = bitPos;
}

void bf_write::WriteOneBitNoCheck(int nValue)
{
	if (nValue)
		m_pData[m_iCurBit >> 5] |= g_LittleBits[m_iCurBit & 31];
	else
		m_pData[m_iCurBit >> 5] &= ~g_LittleBits[m_iCurBit & 31];

	++m_iCurBit;
}

void bf_write::WriteOneBit(int nValue)
{
	if (m_iCurBit >= m_nDataBits)
	{
		return;
	}

	WriteOneBitNoCheck(nValue);
}

void bf_write::WriteOneBitAt(int iBit, int nValue)
{
	if (nValue)
		m_pData[iBit >> 5] |= g_LittleBits[iBit & 31];
	else
		m_pData[iBit >> 5] &= ~g_LittleBits[iBit & 31];
}

void bf_write::WriteUBitLong(unsigned int curData, int numbits, bool bCheckRange)
{
	if (GetNumBitsLeft() < numbits)
	{
		m_iCurBit = m_nDataBits;
		return;
	}

	int iCurBitMasked = m_iCurBit & 31;
	int iDWord = m_iCurBit >> 5;
	m_iCurBit += numbits;

	unsigned long* pOut = &m_pData[iDWord];

	curData = (curData << iCurBitMasked) | (curData >> (32 - iCurBitMasked));

	unsigned int temp = 1 << (numbits - 1);
	unsigned int mask1 = (temp * 2 - 1) << iCurBitMasked;
	unsigned int mask2 = (temp - 1) >> (31 - iCurBitMasked);

	int i = mask2 & 1;
	unsigned long dword1 = LoadLittleDWord(pOut, 0);
	unsigned long dword2 = LoadLittleDWord(pOut, i);

	dword1 ^= (mask1 & (curData ^ dword1));
	dword2 ^= (mask2 & (curData ^ dword2));

	StoreLittleDWord(pOut, i, dword2);
	StoreLittleDWord(pOut, 0, dword1);
}

void bf_write::WriteSBitLong(int data, int numbits)
{
	int nValue = data;
	int nPreserveBits = (0x7FFFFFFF >> (32 - numbits));
	int nSignExtension = (nValue >> 31) & ~nPreserveBits;
	nValue &= nPreserveBits;
	nValue |= nSignExtension;

	WriteUBitLong(nValue, numbits, false);
}

void bf_write::WriteBitLong(unsigned int data, int numbits, bool bSigned)
{
	if (bSigned)
		WriteSBitLong((int)data, numbits);
	else
		WriteUBitLong(data, numbits);
}

bool bf_write::WriteBits(const void *pInData, int nBits)
{
	unsigned char *pOut = (unsigned char*)pInData;
	int nBitsLeft = nBits;

	if ((m_iCurBit + nBits) > m_nDataBits)
	{
		return false;
	}

	while (((unsigned long)pOut & 3) != 0 && nBitsLeft >= 8)
	{

		WriteUBitLong(*pOut, 8, false);
		++pOut;
		nBitsLeft -= 8;
	}

	if ((nBitsLeft >= 32) && (m_iCurBit & 7) == 0)
	{
		int numbytes = nBitsLeft >> 3;
		int numbits = numbytes << 3;

		memcpy((char*)m_pData + (m_iCurBit >> 3), pOut, numbytes);
		pOut += numbytes;
		nBitsLeft -= numbits;
		m_iCurBit += numbits;
	}

	if (nBitsLeft >= 32)
	{
		unsigned long iBitsRight = (m_iCurBit & 31);
		unsigned long iBitsLeft = 32 - iBitsRight;
		unsigned long bitMaskLeft = g_BitWriteMasks[iBitsRight][32];
		unsigned long bitMaskRight = g_BitWriteMasks[0][iBitsRight];

		unsigned long *pData = &m_pData[m_iCurBit >> 5];

		while (nBitsLeft >= 32)
		{
			unsigned long curData = *(unsigned long*)pOut;
			pOut += sizeof(unsigned long);

			*pData &= bitMaskLeft;
			*pData |= curData << iBitsRight;

			pData++;

			if (iBitsLeft < 32)
			{
				curData >>= iBitsLeft;
				*pData &= bitMaskRight;
				*pData |= curData;
			}

			nBitsLeft -= 32;
			m_iCurBit += 32;
		}
	}

	while (nBitsLeft >= 8)
	{
		WriteUBitLong(*pOut, 8, false);
		++pOut;
		nBitsLeft -= 8;
	}

	if (nBitsLeft)
	{
		WriteUBitLong(*pOut, nBitsLeft, false);
	}

	return true;
}

void bf_write::WriteUBitVar(unsigned int data)
{
	int n = (data < 0x10u ? -1 : 0) + (data < 0x100u ? -1 : 0) + (data < 0x1000u ? -1 : 0);
	WriteUBitLong(data * 4 + n + 3, 6 + n * 4 + 12);

	if (data >= 0x1000u)
	{
		WriteUBitLong(data >> 16, 16);
	}
}

void bf_write::WriteVarInt32(uint32 data)
{
	if ((m_iCurBit & 7) == 0 && (m_iCurBit + kMaxVarintBytes * 8) <= m_nDataBits)
	{
		uint8 *target = ((uint8*)m_pData) + (m_iCurBit >> 3);

		target[0] = static_cast<uint8>(data | 0x80);
		if (data >= (1 << 7))
		{
			target[1] = static_cast<uint8>((data >> 7) | 0x80);
			if (data >= (1 << 14))
			{
				target[2] = static_cast<uint8>((data >> 14) | 0x80);
				if (data >= (1 << 21))
				{
					target[3] = static_cast<uint8>((data >> 21) | 0x80);
					if (data >= (1 << 28))
					{
						target[4] = static_cast<uint8>(data >> 28);
						m_iCurBit += 5 * 8;
						return;
					}
					else
					{
						target[3] &= 0x7F;
						m_iCurBit += 4 * 8;
						return;
					}
				}
				else
				{
					target[2] &= 0x7F;
					m_iCurBit += 3 * 8;
					return;
				}
			}
			else
			{
				target[1] &= 0x7F;
				m_iCurBit += 2 * 8;
				return;
			}
		}
		else
		{
			target[0] &= 0x7F;
			m_iCurBit += 1 * 8;
			return;
		}
	}
	else
	{
		while (data > 0x7F)
		{
			WriteUBitLong((data & 0x7F) | 0x80, 8);
			data >>= 7;
		}
		WriteUBitLong(data & 0x7F, 8);
	}
}

void bf_write::WriteSignedVarInt32(int32 data)
{
	WriteVarInt32(ZigZagEncode32(data));
}

int	bf_write::ByteSizeVarInt32(uint32 data)
{
	int size = 1;

	while (data > 0x7F)
	{
		size++;
		data >>= 7;
	}

	return size;
}

int bf_write::ByteSizeSignedVarInt32(int32 data)
{
	return ByteSizeVarInt32(ZigZagEncode32(data));
}

bool bf_write::WriteBitsFromBuffer(bf_read *pIn, int nBits)
{
	while (nBits > 32)
	{
		WriteUBitLong(pIn->ReadUBitLong(32), 32);
		nBits -= 32;
	}

	WriteUBitLong(pIn->ReadUBitLong(nBits), nBits);
	return true;
}

void bf_write::WriteBitCoord(const float f)
{
	int signbit = (f <= -COORD_RESOLUTION);
	int intval = (int)abs(f);
	int fractval = abs((int)(f*COORD_DENOMINATOR)) & (COORD_DENOMINATOR - 1);

	WriteOneBit(intval);
	WriteOneBit(fractval);

	if (intval || fractval)
	{
		WriteOneBit(signbit);

		if (intval)
		{
			intval--;
			WriteUBitLong((unsigned int)intval, COORD_INTEGER_BITS);
		}

		if (fractval)
		{
			WriteUBitLong((unsigned int)fractval, COORD_FRACTIONAL_BITS);
		}
	}
}

void bf_write::WriteBitFloat(float val)
{
	long intVal;

	intVal = *((long*)&val);
	WriteUBitLong(intVal, 32);
}

void bf_write::WriteBitVec3Coord(const Vector& fa)
{
	int		xflag, yflag, zflag;

	xflag = (fa[0] >= COORD_RESOLUTION) || (fa[0] <= -COORD_RESOLUTION);
	yflag = (fa[1] >= COORD_RESOLUTION) || (fa[1] <= -COORD_RESOLUTION);
	zflag = (fa[2] >= COORD_RESOLUTION) || (fa[2] <= -COORD_RESOLUTION);

	WriteOneBit(xflag);
	WriteOneBit(yflag);
	WriteOneBit(zflag);

	if (xflag)
		WriteBitCoord(fa[0]);
	if (yflag)
		WriteBitCoord(fa[1]);
	if (zflag)
		WriteBitCoord(fa[2]);
}

void bf_write::WriteBitNormal(float f)
{
	int	signbit = (f <= -NORMAL_RESOLUTION);
	unsigned int fractval = abs((int)(f*NORMAL_DENOMINATOR));

	if (fractval > NORMAL_DENOMINATOR)
		fractval = NORMAL_DENOMINATOR;

	WriteOneBit(signbit);

	WriteUBitLong(fractval, NORMAL_FRACTIONAL_BITS);
}

void bf_write::WriteBitVec3Normal(const Vector& fa)
{
	int		xflag, yflag;

	xflag = (fa[0] >= NORMAL_RESOLUTION) || (fa[0] <= -NORMAL_RESOLUTION);
	yflag = (fa[1] >= NORMAL_RESOLUTION) || (fa[1] <= -NORMAL_RESOLUTION);

	WriteOneBit(xflag);
	WriteOneBit(yflag);

	if (xflag)
		WriteBitNormal(fa[0]);
	if (yflag)
		WriteBitNormal(fa[1]);

	int	signbit = (fa[2] <= -NORMAL_RESOLUTION);
	WriteOneBit(signbit);
}

void bf_write::WriteBitAngles(const QAngle& fa)
{
	Vector tmp(fa.x, fa.y, fa.z);
	WriteBitVec3Coord(tmp);
}

void bf_write::WriteChar(int val)
{
	WriteSBitLong(val, sizeof(char) << 3);
}

void bf_write::WriteByte(int val)
{
	WriteUBitLong(val, sizeof(unsigned char) << 3);
}

void bf_write::WriteShort(int val)
{
	WriteSBitLong(val, sizeof(short) << 3);
}

void bf_write::WriteWord(int val)
{
	WriteUBitLong(val, sizeof(unsigned short) << 3);
}

void bf_write::WriteLong(long val)
{
	WriteSBitLong(val, sizeof(long) << 3);
}

void bf_write::WriteFloat(float val)
{
	// Pre-swap the float, since WriteBits writes raw data
	LittleFloat(&val, &val);

	WriteBits(&val, sizeof(val) << 3);
}

bool bf_write::WriteBytes(const void *pBuf, int nBytes)
{
	return WriteBits(pBuf, nBytes << 3);
}

bool bf_write::WriteString(const char *pStr)
{
	if (pStr)
	{
		do
		{
			WriteChar(*pStr);
			++pStr;
		} while (*(pStr - 1) != 0);
	}
	else
	{
		WriteChar(0);
	}

	return true;
}

int bf_write::GetNumBytesWritten() const
{
	return BitByte(m_iCurBit);
}

int bf_write::GetNumBitsWritten() const
{
	return m_iCurBit;
}

int bf_write::GetMaxNumBits()
{
	return m_nDataBits;
}

int bf_write::GetNumBitsLeft()
{
	return m_nDataBits - m_iCurBit;
}

int bf_write::GetNumBytesLeft()
{
	return GetNumBitsLeft() >> 3;
}

unsigned char* bf_write::GetData()
{
	return (unsigned char*)m_pData;
}

const unsigned char* bf_write::GetData() const
{
	return (unsigned char*)m_pData;
}


/*==============================================================================*/
//	bf_read
/*==============================================================================*/
bf_read::bf_read()
{
	m_pData = NULL;
	m_nDataBytes = 0;
	m_nDataBits = -1;
	m_iCurBit = 0;
}

bf_read::bf_read(const void *pData, int nBytes, int nBits)
{
	StartReading(pData, nBytes, 0, nBits);
}

void bf_read::StartReading(const void *pData, int nBytes, int iStartBit, int nBits)
{
	m_pData = (unsigned char*)pData;
	m_nDataBytes = nBytes;

	if (nBits == -1)
	{
		m_nDataBits = m_nDataBytes << 3;
	}
	else
	{
		m_nDataBits = nBits;
	}

	m_iCurBit = iStartBit;
}

void bf_read::Reset()
{
	m_iCurBit = 0;
}

int bf_read::ReadOneBitNoCheck()
{
	unsigned char value = m_pData[m_iCurBit >> 3] >> (m_iCurBit & 7);

	++m_iCurBit;
	return value & 1;
}

int bf_read::ReadOneBit()
{
	if (GetNumBitsLeft() <= 0)
	{
		return 0;
	}

	return ReadOneBitNoCheck();
}

void bf_read::ReadBits(void *pOutData, int nBits)
{
	unsigned char *pOut = (unsigned char*)pOutData;
	int nBitsLeft = nBits;

	while (((size_t)pOut & 3) != 0 && nBitsLeft >= 8)
	{
		*pOut = (unsigned char)ReadUBitLong(8);
		++pOut;
		nBitsLeft -= 8;
	}

	while (nBitsLeft >= 32)
	{
		*((unsigned long*)pOut) = ReadUBitLong(32);
		pOut += sizeof(unsigned long);
		nBitsLeft -= 32;
	}

	while (nBitsLeft >= 8)
	{
		*pOut = ReadUBitLong(8);
		++pOut;
		nBitsLeft -= 8;
	}

	if (nBitsLeft)
	{
		*pOut = ReadUBitLong(nBitsLeft);
	}
}

unsigned int bf_read::ReadUBitLong(int numbits)
{
	if (GetNumBitsLeft() < numbits)
	{
		m_iCurBit = m_nDataBits;
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

unsigned int bf_read::PeekUBitLong(int numbits)
{
	unsigned int r;
	int i, nBitValue;

	bf_read savebf;
	savebf = *this;

	r = 0;
	for (i = 0; i < numbits; i++)
	{
		nBitValue = ReadOneBit();

		if (nBitValue)
		{
			r |= GetBitForBitnum(i);
		}
	}

	*this = savebf;
	return r;
}

int bf_read::ReadSBitLong(int numbits)
{
	unsigned int r = ReadUBitLong(numbits);
	unsigned int s = 1 << (numbits - 1);

	if (r >= s)
	{
		r = r - s - s;
	}

	return r;
}

uint32 bf_read::ReadVarInt32()
{
	uint32 result = 0;
	int count = 0;
	uint32 b;

	do
	{
		if (count == kMaxVarint32Bytes)
		{
			return result;
		}
		b = ReadUBitLong(8);
		result |= (b & 0x7F) << (7 * count);
		++count;
	} while (b & 0x80);

	return result;
}

int32 bf_read::ReadSignedVarInt32()
{
	uint32 value = ReadVarInt32();
	return ZigZagDecode32(value);
}

unsigned int bf_read::ReadBitLong(int numbits, bool bSigned)
{
	if (bSigned)
		return (unsigned int)ReadSBitLong(numbits);
	else
		return ReadUBitLong(numbits);
}

float bf_read::ReadBitCoord(void)
{
	int intval = 0;
	int fractval = 0;
	int signbit = 0;
	float value = 0.0;

	intval = ReadOneBit();
	fractval = ReadOneBit();

	if (intval || fractval)
	{
		signbit = ReadOneBit();

		if (intval)
		{
			intval = ReadUBitLong(COORD_INTEGER_BITS) + 1;
		}

		if (fractval)
		{
			fractval = ReadUBitLong(COORD_FRACTIONAL_BITS);
		}

		value = intval + ((float)fractval * COORD_RESOLUTION);

		if (signbit)
			value = -value;
	}

	return value;
}

float bf_read::ReadBitFloat()
{
	union { uint32 u; float f; } c = { ReadUBitLong(32) };
	return c.f;
}

float bf_read::ReadBitNormal(void)
{
	int	signbit = ReadOneBit();

	unsigned int fractval = ReadUBitLong(NORMAL_FRACTIONAL_BITS);

	float value = (float)fractval * NORMAL_RESOLUTION;

	if (signbit)
		value = -value;

	return value;
}

void bf_read::ReadBitVec3Coord(Vector& fa)
{
	int xflag;
	int yflag;
	int zflag;

	fa.Init(0, 0, 0);

	xflag = ReadOneBit();
	yflag = ReadOneBit();
	zflag = ReadOneBit();

	if (xflag)
		fa[0] = ReadBitCoord();
	if (yflag)
		fa[1] = ReadBitCoord();
	if (zflag)
		fa[2] = ReadBitCoord();
}

void bf_read::ReadBitVec3Normal(Vector& fa)
{
	int xflag = ReadOneBit();
	int yflag = ReadOneBit();

	if (xflag)
		fa[0] = ReadBitNormal();
	else
		fa[0] = 0.0f;

	if (yflag)
		fa[1] = ReadBitNormal();
	else
		fa[1] = 0.0f;

	int znegative = ReadOneBit();

	float fafafbfb = fa[0] * fa[0] + fa[1] * fa[1];
	if (fafafbfb < 1.0f)
		fa[2] = sqrt(1.0f - fafafbfb);
	else
		fa[2] = 0.0f;

	if (znegative)
		fa[2] = -fa[2];
}

void bf_read::ReadBitAngles(QAngle& fa)
{
	Vector tmp;
	ReadBitVec3Coord(tmp);
	fa.Init(tmp.x, tmp.y, tmp.z);
}

int	bf_read::ReadChar()
{
	return (char)ReadUBitLong(8);
}

int	bf_read::ReadByte()
{
	return ReadUBitLong(8);
}

int	bf_read::ReadShort()
{
	return (short)ReadUBitLong(16);
}

int	bf_read::ReadWord()
{
	return ReadUBitLong(16);
}

long bf_read::ReadLong()
{
	return ReadUBitLong(32);
}

float bf_read::ReadFloat()
{
	float ret;
	ReadBits(&ret, 32);

	LittleFloat(&ret, &ret);
	return ret;
}

bool bf_read::ReadBytes(void *pOut, int nBytes)
{
	ReadBits(pOut, nBytes << 3);
	return true;
}

bool bf_read::ReadString(char *pStr, int maxLen, bool bLine, int *pOutNumChars)
{
	bool bTooSmall = false;
	int iChar = 0;

	while (1)
	{
		char val = ReadChar();
		if (val == 0)
			break;
		else if (bLine && val == '\n')
			break;

		if (iChar < (maxLen - 1))
		{
			pStr[iChar] = val;
			++iChar;
		}
		else
		{
			bTooSmall = true;
		}
	}

	pStr[iChar] = 0;

	if (pOutNumChars)
		*pOutNumChars = iChar;

	return true;
}

char* bf_read::ReadAndAllocateString(bool *pOverflow)
{
	char str[2048];

	int nChars;
	bool bOverflow = !ReadString(str, sizeof(str), false, &nChars);
	if (pOverflow)
		*pOverflow = bOverflow;

	char *pRet = new char[nChars + 1];
	for (int i = 0; i <= nChars; i++)
		pRet[i] = str[i];

	return pRet;
}

int bf_read::GetNumBytesRead()
{
	return BitByte(m_iCurBit);
}

int bf_read::GetNumBitsLeft()
{
	return m_nDataBits - m_iCurBit;
}

int bf_read::GetNumBytesLeft()
{
	return GetNumBitsLeft() >> 3;
}

int bf_read::GetNumBitsRead() const
{
	return m_iCurBit;
}

bool bf_read::Seek(int iBit)
{
	if (iBit < 0 || iBit > m_nDataBits)
	{
		m_iCurBit = m_nDataBits;
		return false;
	}
	else
	{
		m_iCurBit = iBit;
		return true;
	}
}

bool bf_read::SeekRelative(int iBitDelta)
{
	return Seek(m_iCurBit + iBitDelta);
}