//------------------------------------------------------------------------------
//
// Project: General
// Target: -
// Version: 0.9
// Date: 2017.04.11
// Author: YGD
// Filename : CLSthread.cpp
// Content: pthread
//
//------------------------------------------------------------------------------
// Include 
//------------------------------------------------------------------------------
#pragma hdrstop
#include "CLSthread.h"
//------------------------------------------------------------------------------
// CLSthread
//------------------------------------------------------------------------------
CLSthread::CLSthread(void)
{
	Init();
}
//------------------------------------------------------------------------------
CLSthread::CLSthread(const char *name) :CLSrunObject(name)
{
	Init();
}
//------------------------------------------------------------------------------
CLSthread::CLSthread(const char *name, int interval) : CLSrunObject(name)
{
	Init(NULL,NULL,interval);
}
//------------------------------------------------------------------------------
CLSthread::CLSthread(const char *name, THR_PTR startRoutine, void *arg, int interval) :CLSrunObject(name)
{
	Init(startRoutine, arg, interval);
}
//------------------------------------------------------------------------------
// ~CLSthread
//------------------------------------------------------------------------------
CLSthread::~CLSthread(void)
{
}
//------------------------------------------------------------------------------
// Run_
//------------------------------------------------------------------------------
void  *CLSthread::Run_(void *aThis)
{
	CLSthread *pObj = (CLSthread *)aThis;
	printf("RUN_\n");
	pObj->Run();
	return 0;
}
//------------------------------------------------------------------------------
// Init
//------------------------------------------------------------------------------
void CLSthread::Init(THR_PTR startRoutine, void *arg, int interval)
{
	Active = true;
	m_interval = interval;
	m_pArg = arg;
	m_startRoutine = startRoutine;
}
//------------------------------------------------------------------------------
// IsExist
//------------------------------------------------------------------------------
bool CLSthread::IsExist(void)
{
	// ID 확인
	if (ID <= 0)
		return (false);

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
bool CLSthread::IsRunning(RUN_STATE *pState)
{
	// Thread 존재 여부 확인
	if (!IsExist())
	{
		*pState = RST_UNEXIST;
		return (false);
	}
	// Thread 실행 상태 확인
	if (CheckElapsedTime(&m_timer, m_interval))
	{
		if (!CheckRunInfo(pState))
		{
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
void CLSthread::Kill(void)
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
void CLSthread::Manage(void)
{
	RUN_STATE state;

	// Thread 상태 확인
	if (IsRunning(&state))
	{
		return;
	}
	// Thread 상태 복원
	switch (state)
	{
	case RST_ABNOMAL: Kill();
	case RST_UNEXIST: Start(); 
		break;
	default: break;
	}
}
//------------------------------------------------------------------------------
// Start
//------------------------------------------------------------------------------
bool CLSthread::Start(void *pArg)
{	
	pthread_attr_t	attr;

	// Start Routine 지정 여부 확인
	//if (m_startRoutine == NULL)
	//	return(false);

	// 쓰레드를 시작한다
	if (pthread_attr_init(&attr) != 0)
		return(false);
	if (pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED) != 0)
		return(false);

	ResetRunInfo();
	//if (pthread_create(&ID, &attr, m_startRoutine, (pArg != NULL) ? pArg : m_pArg) != 0)
	//	return(false);
	if (pthread_create(&ID, &attr, &CLSthread::Run_, (void*)this) != 0)	
		return(false);

	pthread_attr_destroy(&attr);
	gettimeofday(&m_timer, NULL);
	return(true);
}
//------------------------------------------------------------------------------
// Stop
//------------------------------------------------------------------------------
void CLSthread::Stop(bool byForce)
{
	Active = false;
	if (byForce)
		pthread_kill(ID, SIGUSR1);
	else pthread_exit(NULL);
}
//------------------------------------------------------------------------------
// End of CLSthread.cpp
//------------------------------------------------------------------------------