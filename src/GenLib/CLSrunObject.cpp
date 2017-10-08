//------------------------------------------------------------------------------
//
// Project: General
// Target: -
// Version: 0.9
// Date: 2017.04.11
// Author: YGD
// Filename : CLSrunObject.cpp
// Content: running manage
//
//------------------------------------------------------------------------------
// Include
//------------------------------------------------------------------------------
#pragma	hdrstop
#include "CLSrunObject.h"
//------------------------------------------------------------------------------
// CLSrunObject
//------------------------------------------------------------------------------
CLSrunObject::CLSrunObject(void)
{
	Init();
}
//------------------------------------------------------------------------------
CLSrunObject::CLSrunObject(const char *name) : CLStbObject(name)
{
	Init();
}
//------------------------------------------------------------------------------
// ~CLSrunObject
//------------------------------------------------------------------------------
CLSrunObject::~CLSrunObject(void)
{
}
//------------------------------------------------------------------------------
// CheckRunInfo
//------------------------------------------------------------------------------
bool CLSrunObject::CheckRunInfo(RUN_STATE *state)
{
	RUN_INFO *rPtr = &RunInfo;
	WDT_INFO *wPtr = &m_wdtInfo;

	// 수행 기록 정리
	rPtr->prevWdc = rPtr->wdc;
	rPtr->wdc = 0;
	
	//Debug("CheckRunInfo prevWdc:[%d], wdc:[%d]", rPtr->prevWdc, rPtr->wdc);
	// 정상 동작 여부 확인
	if (rPtr->prevWdc)
	{
		
		//Debug("CheckRunInfo Success");
		rPtr->wtc = 0;	//wait counter
		return (true);
	}	
	//Debug("CheckRunInfo wtc:[%d], wtcMax:[%d]", rPtr->wtc, wPtr->wtcMax);
	// 비정상 동작 시 대기 시간 확인
	if (++rPtr->wtc < wPtr->wtcMax)
		return (true);

	return (false);
}
//------------------------------------------------------------------------------
// Deregister
//------------------------------------------------------------------------------
void CLSrunObject::Deregister(ULONG id)
{
	if (ID == id)
		ID = 0;
}
//------------------------------------------------------------------------------
// Init
//------------------------------------------------------------------------------
void CLSrunObject::Init(void)
{
	ID = 0;
	TestCode = 0;
	Active = false;
	Terminate = false;
	m_wdtInfo.wtcMax = 3;	//temporary
	ResetRunInfo();
}
//------------------------------------------------------------------------------
// MarkTime
//------------------------------------------------------------------------------
void CLSrunObject::MarkTime(void)
{
	gettimeofday(&m_startTime, NULL);
}
//------------------------------------------------------------------------------
// Pause
//------------------------------------------------------------------------------
void CLSrunObject::Pause(int msec)
{
	struct timespec ts;
	ts.tv_sec = msec / 1000;
	ts.tv_nsec = (msec % 1000) * 1000000;
#if 0
	clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &ts, NULL);
#else
	nanosleep(&ts, NULL);
#endif
}
//------------------------------------------------------------------------------
// Register
//------------------------------------------------------------------------------
void CLSrunObject::Register(ULONG id)
{
	ID = id;
	Debug(2,"PRC Register[%d]", ID);
}
//------------------------------------------------------------------------------
// ResetRunInfo
//------------------------------------------------------------------------------
void CLSrunObject::ResetRunInfo(void)
{
	memset(&RunInfo, 0, RUN_INFO_SIZE);
}
//------------------------------------------------------------------------------
// UpdateRunInfo
//------------------------------------------------------------------------------
void CLSrunObject::UpdateRunInfo(void)
{
	RUN_INFO *ptr = &RunInfo;
	struct timeval endTime, elapsed;

	gettimeofday(&endTime, NULL);
	timersub(&endTime, &m_startTime, &elapsed);
	ptr->elapsed = TimerToMsec(&elapsed);
	if (ptr->elapsed > ptr->maxET)
		ptr->maxET = ptr->elapsed;
	ptr->wdc++;
}
//------------------------------------------------------------------------------
// SetElapsedTime
//------------------------------------------------------------------------------
void CLSrunObject::SetElapsedTime(void)
{
	RUN_INFO *ptr = &RunInfo;
	struct timeval endTime, elapsed;

	gettimeofday(&endTime, NULL);
	timersub(&endTime, &m_startTime, &elapsed);
	ptr->elapsed = TimerToMsec(&elapsed);
	if (ptr->elapsed > ptr->maxET)
		ptr->maxET = ptr->elapsed;
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
ULONG CLSrunObject::GetThreadID(void)
{
	return (ID);
}
//------------------------------------------------------------------------------
// End of CLSrunObject.cpp
//------------------------------------------------------------------------------