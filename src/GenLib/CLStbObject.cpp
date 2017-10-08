//------------------------------------------------------------------------------
//
// Project: General
// Target: -
// Version: 0.9
// Date: 2017.04.11
// Author: YGD
// Filename : CLStbObject.cpp
// Content: time
//
//------------------------------------------------------------------------------
// Include
//------------------------------------------------------------------------------
#pragma	hdrstop
#include "CLStbObject.h"
//------------------------------------------------------------------------------
// CLStbObject
//------------------------------------------------------------------------------
CLStbObject::CLStbObject(void)
{

}
//------------------------------------------------------------------------------
CLStbObject::CLStbObject(const char *name) : CLSobject(name)
{

}
//------------------------------------------------------------------------------
// ~CLStbObject
//------------------------------------------------------------------------------
CLStbObject::~CLStbObject(void)
{

}
//------------------------------------------------------------------------------
// CheckElapsedTime
//------------------------------------------------------------------------------
bool CLStbObject::CheckElapsedTime(struct timeval *timer, int duration, int offset)
{
	struct timeval current, elapsed;
	
	gettimeofday(&current, NULL);
	if (!timerisset(timer))
	{
		memcpy(timer, &current, sizeof(struct timeval));
		return (false);
	}
	timersub(&current, timer, &elapsed);
	if (TimerToMsec(&elapsed) < duration)
		return (false);
	memcpy(timer, &current, sizeof(struct timeval));
	return (true);	
}
//------------------------------------------------------------------------------
// TimerToMsec
//------------------------------------------------------------------------------
int CLStbObject::TimerToMsec(struct timeval *timer)
{
	return (timer->tv_sec * 1000 + timer->tv_usec / 1000);
}
//------------------------------------------------------------------------------
// End of CLStbObject.cpp
//------------------------------------------------------------------------------