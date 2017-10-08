//------------------------------------------------------------------------------
//
// Project: General
// Target: -
// Version: 0.9
// Date: 2017.04.11
// Author: YGD
// Filename : CLSrunObject.h
// Content: running manage
//
//------------------------------------------------------------------------------
#ifndef	CLSrunObjectH
#define	CLSrunObjectH
//------------------------------------------------------------------------------
// Include
//------------------------------------------------------------------------------
#include "CLStbObject.h"
//------------------------------------------------------------------------------
#include <signal.h>
//------------------------------------------------------------------------------
// Constant
//------------------------------------------------------------------------------
// Default Check Interval
//------------------------------------------------------------------------------
#define DEF_CHECK_INTERVAL		1000	//1 sec
//------------------------------------------------------------------------------
// Type definition
//------------------------------------------------------------------------------
// RUN_INFO
//------------------------------------------------------------------------------
typedef struct
{
	BYTE wtc;			// wait counter
	WORD prevWdc;		// 이전 Watch dog counter
	WORD wdc;			// watch dog counter
	int elapsed;		// Max Elapsed Time
	int maxET;
} RUN_INFO;

#define RUN_INFO_SIZE	sizeof(RUN_INFO)
//------------------------------------------------------------------------------
// RUN_STATE
//------------------------------------------------------------------------------
typedef enum {RST_OK, RST_UNEXIST, RST_ABNOMAL} RUN_STATE;
//------------------------------------------------------------------------------
// WDT_INFO
//------------------------------------------------------------------------------
typedef struct
{
	BYTE wtcMax;
} WDT_INFO;
//------------------------------------------------------------------------------
// Class
//------------------------------------------------------------------------------
class CLSrunObject : public CLStbObject
{
private:
	WDT_INFO m_wdtInfo;
	struct timeval m_startTime;

	void Init(void);
public:
	ULONG ID;
	bool Active;
	bool Terminate;
	int TestCode;	// 시험 코드
	int TestTarget;	// 시험 대상
	RUN_INFO RunInfo;

	CLSrunObject(void);
	CLSrunObject(const char *name);
	virtual ~CLSrunObject(void);

	bool CheckRunInfo(RUN_STATE *state);
	void Deregister(ULONG id);
	void MarkTime(void);
	void Pause(int msec);
	void Register(ULONG id);
	void ResetRunInfo(void);
	void UpdateRunInfo(void);
	void SetElapsedTime(void);

	//--------------------------------
	// YGD
	//--------------------------------
	ULONG   GetThreadID(void);
};
//------------------------------------------------------------------------------
#endif	// CLSrunObjectH
//------------------------------------------------------------------------------
// End of CLSrunObject.h
//------------------------------------------------------------------------------