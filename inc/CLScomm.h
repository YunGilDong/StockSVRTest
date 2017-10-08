//------------------------------------------------------------------------------
//
// Project: General
// Target: -
// Version: 0.9
// Date: 2017.04.11
// Author: YGD
// Filename : CLScomm.h
// Content: comm
//
//------------------------------------------------------------------------------
#ifndef	CLScommH
#define	CLScommH
//------------------------------------------------------------------------------
// Include
//------------------------------------------------------------------------------
#include "CLStbObject.h"
//------------------------------------------------------------------------------
// Constant
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Type definition
//------------------------------------------------------------------------------
// COM_ENDIAN
//------------------------------------------------------------------------------
typedef enum { ENDIAN_BIG, ENDIAN_LITTLE }	COM_ENDIAN;
//------------------------------------------------------------------------------
// Class
//------------------------------------------------------------------------------
class CLScomm : public CLStbObject
{
private:

	void Init(void);
public:
	int ID;
	bool Connected;
	bool Initialized;

	CLScomm(void);
	CLScomm(const char *name);
	virtual ~CLScomm(void);

	BYTE GenLRC(BYTE *message, int length);
	UINT GetNumber(char *ptr, int length = 2, COM_ENDIAN endian = ENDIAN_BIG);
	void SetNumber(char *ptr, UINT value, int length = 2, COM_ENDIAN = ENDIAN_BIG);
	int String2Int(char *pData, int length = 2);

	virtual bool CheckState(int mode);
};
//------------------------------------------------------------------------------
#endif	//CLScommH
//------------------------------------------------------------------------------
// End of CLScomm.h
//------------------------------------------------------------------------------
