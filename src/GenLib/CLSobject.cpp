//------------------------------------------------------------------------------
//
// Project: General
// Target: -
// Version: 0.9
// Date: 2017.04.11
// Author: YGD
// Filename : CLSobject.cpp
// Content: ObjectClass
// History:		Date		By		Content
//				----------	------	--------------------------------------------
//				2017.04.11	YGD		First edition
//				2017.09.14	YGD		Add printf thread safe
//------------------------------------------------------------------------------
// Include
//------------------------------------------------------------------------------
#pragma	hdrstop
#include "CLSobject.h"
#include <pthread.h>
//------------------------------------------------------------------------------
// Static
//------------------------------------------------------------------------------
static pthread_mutex_t printf_mutex;
//------------------------------------------------------------------------------
// CLSobject
//------------------------------------------------------------------------------
CLSobject::CLSobject(void)
{
	Level = 1;
	Target = 0;
	memset(Name, 0, OBJNAME_LEN);
	pthread_mutex_init(&printf_mutex, NULL);
}
//------------------------------------------------------------------------------
CLSobject::CLSobject(const char *name)
{
	Level = 1;
	Target = 0;
	SetName(name);
	pthread_mutex_init(&printf_mutex, NULL);
}
//------------------------------------------------------------------------------
// ~CLSobject
//------------------------------------------------------------------------------
CLSobject::~CLSobject(void)
{
}
//------------------------------------------------------------------------------
// CheckDebug
//------------------------------------------------------------------------------
bool CLSobject::CheckDebug(int level, int target)
{
	if (!Level)			//Level is 0, no debuging
		return (false);
	if (level > Level)	//기존 설정된 레벨보다 높으면 no debuging (설정된 레벨 이하만 디버깅)
		return (false);

	// Check target
	if (!Target || !target)		// 설정된 target이 없으면
		return (true);
	if (Target != target)
		return (false);

	return (true);
}
//------------------------------------------------------------------------------
// Debug
//------------------------------------------------------------------------------
bool CLSobject::Debug(const char *format, ...)
{
	va_list argList;

	va_start(argList, format);
	pthread_mutex_lock(&printf_mutex);
	vsprintf(m_buffer, format, argList);
	printf("%s %s\n", GetTimeString(), m_buffer);
	pthread_mutex_unlock(&printf_mutex);
	va_end(argList);
}
//------------------------------------------------------------------------------
void CLSobject::Debug(int level, const char *format, ...)
{
	va_list argList;

	va_start(argList, format);
	if (CheckDebug(level))
	{
		pthread_mutex_lock(&printf_mutex);
		vsprintf(m_buffer, format, argList);
		printf("%s  %s\n", GetTimeString(), m_buffer);
		pthread_mutex_unlock(&printf_mutex);
	}
	va_end(argList);
}
//------------------------------------------------------------------------------
void CLSobject::Debug(int target, int level, const char *format, ...)
{
	va_list argList;

	va_start(argList, format);
	if (CheckDebug(level, target))
	{
		pthread_mutex_lock(&printf_mutex);
		vsprintf(m_buffer, format, argList);
		printf("%s  %s\n", GetTimeString(), m_buffer);
		pthread_mutex_unlock(&printf_mutex);
	}
	va_end(argList);
}
//------------------------------------------------------------------------------
// GetTimeString
//------------------------------------------------------------------------------
char *CLSobject::GetTimeString(void)
{
	struct tm *tod;
	struct timeval timeVal;

	gettimeofday(&timeVal, NULL);	//현재 시간
	tod = localtime(&timeVal.tv_sec);	//현재 시간(초)을 날짜 및 시간으로
	sprintf(m_timeStamp, "%04d/%02d/%02d  %02d:%02d:%02d.%03d [%s]",
		tod->tm_year+1900, tod->tm_mon+1, tod->tm_mday, tod->tm_hour,
		tod->tm_min, tod->tm_sec, timeVal.tv_usec / 1000, Name);

	return (m_timeStamp);
}
//------------------------------------------------------------------------------
char* CLSobject::GetTimeString(struct tm *pTod)
{
	struct timeval timeVal;

	gettimeofday(&timeVal, NULL);
	localtime_r(&timeVal.tv_sec, pTod);
	sprintf(m_timeStamp, "%04d/%02d/%02d  %02d:%02d:%02d.%03d [%s]",
		pTod->tm_year + 1900, pTod->tm_mon + 1, pTod->tm_mday, pTod->tm_hour,
		pTod->tm_min, pTod->tm_sec, timeVal.tv_usec / 1000, Name);
	return (m_timeStamp);
}
//------------------------------------------------------------------------------
// SetDebug
//------------------------------------------------------------------------------
void CLSobject::SetDebug(BYTE level, UINT target)
{
	Level = level;
	Target = target;
}
//------------------------------------------------------------------------------
// SetName
//------------------------------------------------------------------------------
void CLSobject::SetName(const char *format, ...)
{
	va_list argList;

	va_start(argList, format);
	vsprintf(Name, format, argList);
	va_end(argList);
}
//------------------------------------------------------------------------------
// End of CLSobject.cpp
//------------------------------------------------------------------------------