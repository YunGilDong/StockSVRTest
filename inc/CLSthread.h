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
#ifndef CLSthreadH
#define CLSthreadH
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
// THR_PTR
//------------------------------------------------------------------------------
typedef void *(*THR_PTR)(void *);
//------------------------------------------------------------------------------
// Class
//------------------------------------------------------------------------------
class CLSthread : public CLSrunObject
{
private:
	int m_interval;
	void *m_pArg;
	THR_PTR m_startRoutine;
	struct timeval m_timer;

	void Init(THR_PTR startRoutine = NULL, void *arg = NULL, int interval = DEF_CHECK_INTERVAL);
protected:
	virtual void Run(void) = 0;
	//virtual void Run(void);
public:
	CLSthread(void);
	CLSthread(const char *name);
	CLSthread(const char *name, int interval = DEF_CHECK_INTERVAL);
	CLSthread(const char *name, THR_PTR startRoutine, void *arg = NULL, int interval = DEF_CHECK_INTERVAL);
	virtual ~CLSthread(void);

	static void  *Run_(void *);

	bool IsExist(void);
	bool IsRunning(RUN_STATE *state);
	void Kill(void);
	void Manage(void);
	bool Start(void *pArg = NULL);
	void Stop(bool byForce = false);

	//------------------------------
	
};
//------------------------------------------------------------------------------
#endif	// CLSthreadH
//------------------------------------------------------------------------------
// End of CLSthread.h
//------------------------------------------------------------------------------