#ifndef SDK_CRC_H
#define SDK_CRC_H
#pragma once

typedef unsigned int CRC32_t;

void CRC32_Init(CRC32_t *pulCRC);
void CRC32_ProcessBuffer(CRC32_t *pulCRC, const void *p, int len);
void CRC32_Final(CRC32_t *pulCRC);
CRC32_t	CRC32_GetTableEntry(unsigned int slot);

CRC32_t CRC32_ProcessSingleBuffer(const void *p, int len);

#endif