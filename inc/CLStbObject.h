//------------------------------------------------------------------------------
//
// Project: General
// Target: -
// Version: 0.9
// Date: 2017.04.11
// Author: YGD
// Filename : CLStbObject.h
// Content: time
//
//------------------------------------------------------------------------------
#ifndef CLStbObjectH
#define CLStbObjectH
//------------------------------------------------------------------------------
// Include
//------------------------------------------------------------------------------
#include "CLSobject.h"
//------------------------------------------------------------------------------
// Class
//------------------------------------------------------------------------------
class CLStbObject : public CLSobject
{
private:
public:
	CLStbObject(void);
	CLStbObject(const char *name);
	virtual ~CLStbObject(void);

	bool CheckElapsedTime(struct timeval *timer, int duration, int offset = 0);
	int TimerToMsec(struct timeval *timer);
};


#endif // !CLStbObjectH

