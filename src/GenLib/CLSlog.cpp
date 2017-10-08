//------------------------------------------------------------------------------
//
// Project: General
// Target: -
// Version: 0.9
// Date: 2017.04.11
// Author: YGD
// Filename : CLSlog.cpp
// Content: Log
// History:		Date		By		Content
//				----------	------	--------------------------------------------
//				2017.04.11	YGD		First edition
//				2017.09.15	YGD		Add Logging thread safe (보류_)
//------------------------------------------------------------------------------
// Include
//------------------------------------------------------------------------------
#pragma	hdrstop
#include "CLSlog.h"
#include <pthread.h>
//------------------------------------------------------------------------------
// Static
//------------------------------------------------------------------------------
//static pthread_mutex_t log_mutex;
//------------------------------------------------------------------------------
// CLSlog
//------------------------------------------------------------------------------
CLSlog::CLSlog(void)
{
}
//------------------------------------------------------------------------------
CLSlog::CLSlog(const char *name, const char *pDir) : CLSobject(name)
{
	Init(pDir);
}
//------------------------------------------------------------------------------
// ~CLSlog
//------------------------------------------------------------------------------
CLSlog::~CLSlog(void)
{
}
//------------------------------------------------------------------------------
// Logging
//------------------------------------------------------------------------------
void CLSlog::Logging(void)
{
	int length;
	char filename[FILENAME_LEN];
	FILE *fp;

	//pthread_mutex_lock(&log_mutex);
	// 메시지 확인
	if ((length = strlen(m_buffer)) <= 0)
		return;		

	// 파일 저장
	sprintf(filename, "%s/%s_%04d%02d%02d.log", m_dir, Name, m_tod.tm_year + 1900, m_tod.tm_mon + 1, m_tod.tm_mday);
	if ((fp = fopen(filename, "a+")) == NULL)
		return;

	fwrite(m_buffer, 1, length, fp);
	//printf("length : %d\n", length);
	fclose(fp);
	//pthread_mutex_unlock(&log_mutex);
}
//------------------------------------------------------------------------------
// FLdump
//------------------------------------------------------------------------------
void CLSlog::FLdump(const char *stamp, char *data, int length, int maxLength)
{
	int maxCount, pos;

	// 메시지 표시
	sprintf(m_buffer, "%s	%s [%d]\n", GetTimeString(&m_tod), stamp, length);
	Logging();
	maxCount = min(length, maxLength);
	pos = 0;
	for (int idx = 0; idx < maxCount; idx++, data++)
	{
		if ((idx % DEFAULT_DUMP_LEN) == 0)
		{
			if (idx != 0)
			{
				sprintf(&m_buffer[pos], "\n");
				Logging();
			}
			pos = sprintf(m_buffer, "    ");
		}
		pos += sprintf(&m_buffer[pos], " %02X", (BYTE)*data);
	}
	if (pos)
	{
		sprintf(&m_buffer[pos], "\n");
		Logging();
	}
}
//------------------------------------------------------------------------------
void CLSlog::FLdump(int level, const char *stamp, char *data, int length, int maxLength)
{
	if (!CheckDebug(level))
		return;

	FLdump(stamp, data, length, maxLength);
}
//------------------------------------------------------------------------------
void CLSlog::FLdump(int target, int level, const char *stamp, char *data, int length, int maxLength)
{
	if (!CheckDebug(level, target))
		return;

	FLdump(stamp, data, length, maxLength);
}
//------------------------------------------------------------------------------
// Init
//------------------------------------------------------------------------------
void CLSlog::Init(const char *pDir)
{
	if (pDir != NULL)
		sprintf(m_dir, "%s", pDir);
}
//------------------------------------------------------------------------------
// Write
//------------------------------------------------------------------------------
void CLSlog::Write(const char *format, ...)
{
	char buffer[BUFFER_LEN];
	va_list argList;

	va_start(argList, format);
	vsprintf(buffer, format, argList);
	sprintf(m_buffer, "%s  %s\n", GetTimeString(&m_tod), buffer);
	va_end(argList);

	Logging();
}
void CLSlog::Write(int level, const char *format, ...)
{
	char buffer[BUFFER_LEN];
	va_list argList;

	va_start(argList, format);
	if (CheckDebug(level))
	{
		vsprintf(buffer, format, argList);
		sprintf(m_buffer, "%s  %s\n", GetTimeString(&m_tod), buffer);
		Logging();
	}
	va_end(argList);
}
void CLSlog::Write(int target, int level, const char *format, ...)
{
	char buffer[BUFFER_LEN];
	va_list argList;

	va_start(argList, format);
	if (CheckDebug(level, target))
	{
		vsprintf(buffer, format, argList);
		sprintf(buffer, "%s  %s\n", GetTimeString(&m_tod), buffer);
		Logging();
	}
	va_end(argList);
}