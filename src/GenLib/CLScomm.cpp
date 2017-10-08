//------------------------------------------------------------------------------
//
// Project: General
// Target: -
// Version: 0.9
// Date: 2017.04.11
// Author: YGD
// Filename : CLScomm.cpp
// Content: comm
//
//------------------------------------------------------------------------------
// Include
//------------------------------------------------------------------------------
#pragma	hdrstop
#include "CLScomm.h"
//------------------------------------------------------------------------------
// CLScomm
//------------------------------------------------------------------------------
CLScomm::CLScomm(void)
{
	Init();
}
//------------------------------------------------------------------------------
CLScomm::CLScomm(const char *name)
{
	Init();
}
//------------------------------------------------------------------------------
// ~CLScomm
//------------------------------------------------------------------------------
CLScomm::~CLScomm(void)
{
}
//------------------------------------------------------------------------------
// GenLRC
//------------------------------------------------------------------------------
void CLScomm::Init(void)
{
	ID = -1;
	Initialized = false;
	Connected = false;
}
//------------------------------------------------------------------------------
// GenLRC
//------------------------------------------------------------------------------
BYTE CLScomm::GenLRC(BYTE *message, int length)
{
	BYTE lrc = 0;
	
	for (int idx = 0; idx < length; idx++)
		lrc ^= *(message + idx);
	return (lrc);
}
//------------------------------------------------------------------------------
// GetNumber
//------------------------------------------------------------------------------
UINT CLScomm::GetNumber(char *ptr, int length, COM_ENDIAN endian)
{
	UINT value = 0;

	switch (endian)
	{
	case ENDIAN_BIG:
		for (int idx = 0; idx < length; idx++)
			value = (value * 256) + (BYTE)*(ptr + idx);
		break;
	default:
		for (int idx = length - 1; idx >= 0; idx--)
			value = (value * 256) + (BYTE)*(ptr + idx);
		break;
	}
	return (value);
}
//------------------------------------------------------------------------------
// SetNumber
//------------------------------------------------------------------------------
void CLScomm::SetNumber(char *ptr, UINT value, int length, COM_ENDIAN endian)
{
	switch (endian)
	{
	case ENDIAN_BIG:	//하위비트 높은번지
		for (int idx = length - 1; idx >= 0; idx--, value /= 256)
			*(ptr + idx) = value % 256;
		break;
	default:			//하위비트 낮은번지
		for (int idx = 0; idx < length; idx++, value /= 256)
			*(ptr + idx) = value % 256;
		break;
	}
}
//------------------------------------------------------------------------------
// String2Int
//------------------------------------------------------------------------------
int CLScomm::String2Int(char *pData, int length)
{
	char buffer[SHORTBUF_LEN];

	if (length >= SHORTBUF_LEN - 1)
		return (-1);

	memcpy(buffer, pData, length);
	buffer[length] = 0;
	return (atoi(buffer));
}
//------------------------------------------------------------------------------
// CheckState
//------------------------------------------------------------------------------
bool CLScomm::CheckState(int mode)
{
	return (true);
}
//------------------------------------------------------------------------------
// End of CLScomm.cpp
//------------------------------------------------------------------------------