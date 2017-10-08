//------------------------------------------------------------------------------
//
// Project: General
// Target: -
// Version: 0.9
// Date: 2017.04.11
// Author: YGD
// Filename : CLSobject.cpp
// Content: ObjectClass
//
//------------------------------------------------------------------------------
#ifndef	CLSobjectH
#define	CLSobjectH
//------------------------------------------------------------------------------
// Include
//------------------------------------------------------------------------------
#include "General.h"
//------------------------------------------------------------------------------
#include <stdarg.h>
#include <time.h>
#include <sys/time.h>
//------------------------------------------------------------------------------
// Constant
//------------------------------------------------------------------------------
#define	OBJNAME_LEN			32
#define	DEFAULT_DUMP_LEN	20
//------------------------------------------------------------------------------
// Class
//------------------------------------------------------------------------------
class CLSobject
{
private:
	char m_buffer[BUFFER_LEN];
	char m_timeStamp[BUFFER_LEN];
public:
	BYTE Level;				// µð¹ö±ë ·¹º§
	UINT Target;			// µð¹ö±ë Target
	char Name[OBJNAME_LEN]; // Object ¸íÄª

	CLSobject(void);
	CLSobject(const char *name);
	virtual ~CLSobject(void);

	bool CheckDebug(int level, int target = 0);
	bool Debug(const char *format, ...);
	void Debug(int level, const char *format, ...);
	void Debug(int target, int level, const char *format, ...);
	char *GetTimeString(void);
	char *GetTimeString(struct tm *pTod);
	void SetDebug(BYTE level, UINT target = 0);
	void SetName(const char *format, ...);	
};
#endif	// CLSobjectH
//------------------------------------------------------------------------------
// End of CLSobject.h
//------------------------------------------------------------------------------