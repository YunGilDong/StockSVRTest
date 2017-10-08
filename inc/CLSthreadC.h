//------------------------------------------------------------------------------
//
// Project: General
// Target: -
// Version: 0.9
// Date: 2017.04.11
// Author: YGD
// Filename : CLSthread.h
// Content: pthread
//
//------------------------------------------------------------------------------
#ifndef CLSthreadCH
#define CLSthreadCH
//------------------------------------------------------------------------------
// Include
//------------------------------------------------------------------------------
#include "CLSrunObject.h"
//------------------------------------------------------------------------------
#include <pthread.h>
#include <unistd.h>
//------------------------------------------------------------------------------
// Type definition
//------------------------------------------------------------------------------
// THR_PTRC
//------------------------------------------------------------------------------
typedef void *(*THR_PTRC)(void *);
//------------------------------------------------------------------------------
// Class
//------------------------------------------------------------------------------
class CLSthreadC : public CLSrunObject
{
private:
	int m_interval;
	void *m_pArg;
	THR_PTRC m_startRoutine;
	struct timeval m_timer;

	void Init(THR_PTRC startRoutine = NULL, void *arg = NULL, int interval = DEF_CHECK_INTERVAL);
protected:

public:
	CLSthreadC(void);
	CLSthreadC(const char *name);
	//CLSthreadC(const char *name, int interval = DEF_CHECK_INTERVAL);
	CLSthreadC(const char *name, THR_PTRC startRoutine, void *arg = NULL, int interval = DEF_CHECK_INTERVAL);
	virtual ~CLSthreadC(void);
	
	bool IsExist(void);
	bool IsRunning(RUN_STATE *state);
	void Kill(void);
	void Manage(void);
	bool Start(void *pArg = NULL);
	void Stop(bool byForce = false);

	//------------------------------	
};
//------------------------------------------------------------------------------
#endif	// CLSthreadCH
//------------------------------------------------------------------------------
// End of CLSthreadC.h
//------------------------------------------------------------------------------