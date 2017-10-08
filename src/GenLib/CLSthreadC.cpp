//------------------------------------------------------------------------------
//
// Project: General
// Target: -
// Version: 0.9
// Date: 2017.04.11
// Author: YGD
// Filename : CLSthreadC.cpp
// Content: pthread
//
//------------------------------------------------------------------------------
// Include 
//------------------------------------------------------------------------------
#pragma hdrstop
#include "CLSthreadC.h"
//------------------------------------------------------------------------------
// CLSthread
//------------------------------------------------------------------------------
CLSthreadC::CLSthreadC(void)
{
	//Init();
}
//------------------------------------------------------------------------------
CLSthreadC::CLSthreadC(const char *name) :CLSrunObject(name)
{
	Init();
}
////------------------------------------------------------------------------------
//CLSthreadC::CLSthreadC(const char *name, int interval) : CLSrunObject(name)
//{
//	Init(NULL,NULL,interval);
//}
//------------------------------------------------------------------------------
CLSthreadC::CLSthreadC(const char *name, THR_PTRC startRoutine, void *arg, int interval) :CLSrunObject(name)
{
	Init(startRoutine, arg, interval);
}
//------------------------------------------------------------------------------
// ~CLSthread
//------------------------------------------------------------------------------
CLSthreadC::~CLSthreadC(void)
{
}
//------------------------------------------------------------------------------
// Init
//------------------------------------------------------------------------------
void CLSthreadC::Init(THR_PTRC startRoutine, void *arg, int interval)
{
	Active = true;
	m_interval = interval;
	m_pArg = arg;
	m_startRoutine = startRoutine;
}
//------------------------------------------------------------------------------
// IsExist
//------------------------------------------------------------------------------
bool CLSthreadC::IsExist(void)
{
	// ID 확인
	if (ID <= 0)
	{
		Debug("[IsExist] Thread ID:[%d]", ID);
		return (false);
	}

	// Object 존재 상태 확인
	if (pthread_kill(ID, 0) != 0) 
	{
		ID = 0;
		return (false);	
	}

	return (true);
}
//------------------------------------------------------------------------------
// IsRunning
//------------------------------------------------------------------------------
bool CLSthreadC::IsRunning(RUN_STATE *pState)
{
	// Thread 존재 여부 확인
	if (!IsExist())
	{
		Debug("Thread RST_UNEXIST");
		*pState = RST_UNEXIST;
		return (false);
	}
	// Thread 실행 상태 확인
	if (CheckElapsedTime(&m_timer, m_interval))
	{
		if (!CheckRunInfo(pState))
		{
			Debug("Thread RST_ABNOMAL");
			*pState = RST_ABNOMAL;
			return (false);
		}
	}
	*pState = RST_OK;
	return (true);
}
//------------------------------------------------------------------------------
// Kill
//------------------------------------------------------------------------------
void CLSthreadC::Kill(void)
{
	for (int idx = 0; idx < 5; idx++)
	{
		// Thread 존재 확인
		if (!IsExist())
			break;
		// Thread를 종료시킨다.
		if (pthread_kill(ID, SIGUSR1) != 0)
			break;
		usleep(10000); //10 msec
	}
}
//------------------------------------------------------------------------------
// Manage
//------------------------------------------------------------------------------
void CLSthreadC::Manage(void)
{
	RUN_STATE state;

	// Thread 상태 확인
	if (IsRunning(&state))
	{
		return;
	}
	//Debug("THR status [%d]", state);
	// Thread 상태 복원
	switch (state)
	{
	case RST_ABNOMAL: 
		Debug("THR RST_ABNOMAL [%d]", state); 
		Kill();
	case RST_UNEXIST: 
		Debug("THR RST_UNEXIST [%d]", state); 
		Start();
		break;
	default: break;
	}
}
//------------------------------------------------------------------------------
// Start
//------------------------------------------------------------------------------
bool CLSthreadC::Start(void *pArg)
{	
	pthread_attr_t	attr;

	// Start Routine 지정 여부 확인
	if (m_startRoutine == NULL)
		return(false);

	// 쓰레드를 시작한다
	if (pthread_attr_init(&attr) != 0)
		return(false);
	if (pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED) != 0)
		return(false);

	ResetRunInfo();
	if (pthread_create(&ID, &attr, m_startRoutine, (pArg != NULL) ? pArg : m_pArg) != 0)
		return(false);

	pthread_attr_destroy(&attr);
	gettimeofday(&m_timer, NULL);
	return(true);
}
//------------------------------------------------------------------------------
// Stop
//------------------------------------------------------------------------------
void CLSthreadC::Stop(bool byForce)
{
	Active = false;
	if (byForce)
		pthread_kill(ID, SIGUSR1);
	else pthread_exit(NULL);
}
//------------------------------------------------------------------------------
// End of CLSthread.cpp
//--------------------------------------------------------------------------------