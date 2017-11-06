//------------------------------------------------------------------------------
// Include
//------------------------------------------------------------------------------
#include "CPSTOCK.h"
//------------------------------------------------------------------------------
// External Variable
//------------------------------------------------------------------------------
extern void *THRserver(void *);
extern void TSVsigHandler(int);
extern void TCLsigHandler(CLSstockCL *, int);
//------------------------------------------------------------------------------
// Prototype
//------------------------------------------------------------------------------
void ClearEnv(void);
//------------------------------------------------------------------------------
// Global Variable
//------------------------------------------------------------------------------
CLSlog Log("CPSTOCK", DIR_LOG);
CLSmap Map("CLMAP"); 
//------------------------------------------------------------------------------
// Local Variable
//------------------------------------------------------------------------------
bool Terminate = false;
SHARED_MEM *ShmPtr = NULL;
CLSsystem *ShmSys = NULL;
CLSprocess *ShmPrc = NULL;
struct timeval TMtimer;
pthread_t MainThrId;

CLSmemory ShmMemory(YGD_SHM_KEY, SHARED_MEM_SIZE, "SHM");
CLSthreadC ThrServer("CPSTOCKSV", THRserver);
//------------------------------------------------------------------------------
// SigHandler
//------------------------------------------------------------------------------
void SigHandler(int sig)
{
	Log.Write(1, "Signal %d accepted", sig);
	switch (sig)
	{
	case SIGTERM:	// 15
	case SIGKILL:	// 9
		ClearEnv();
		break;
	default:	break;
	}
}
//------------------------------------------------------------------------------
// THRsigHandler (external function)
//------------------------------------------------------------------------------
void THRsigHandler(int sig)
{
	pthread_t id = pthread_self();
	CLSstockCL *ptr;
	
	//Log.Write("Thread id [%d] sig [%d]", id, sig);
	// Process server thread signal
	if (id == ThrServer.ID)
	{
		TSVsigHandler(sig);
		return;
	}
	// Process client thread signal
	if ((ptr = Map.Get(id)) != NULL)
		TCLsigHandler(ptr, sig);
}
//------------------------------------------------------------------------------
// NeedTerminate
//------------------------------------------------------------------------------
bool NeedTerminate(void)
{
	pthread_t thrid = pthread_self();
	/*
	if (!ShmPrc->IsActiveProcess(getpid()))
	{
		Log.Write("IsActiveProcess [%d]", thrid);
		return (true);
	}
	*/
	if (Terminate)
	{
		Log.Write("Terminate [%d]", thrid);
		return (true);
	}
	if (ShmSys->Terminate)
	{
		//if (thrid != MainThrId)	
		//	return (false);
		Log.Write("ShmSys->Terminate [%d]", thrid);
		return (true);
	}		

	return (false);
}
//------------------------------------------------------------------------------
// DeleteClient
//------------------------------------------------------------------------------
void DeleteClient(CLSstockCL *pStockCL, char *address)
{
	// Client 종료 요구
	if (pStockCL->Thread != NULL)
		pStockCL->Thread->Terminate = true;
	// 종료 대기
	for (int idx = 0; idx < 5 && pStockCL->Thread->Active; idx++)
		usleep(5000);	// 5ms
	pStockCL->Delete();
	Map.Erase(address);
} 
//------------------------------------------------------------------------------
// DeleteClientX
//------------------------------------------------------------------------------
void DeleteClientX(CLSstockCL *pStockCL)
{
	pStockCL->Delete();
}
//------------------------------------------------------------------------------
// ManageState
//------------------------------------------------------------------------------
void ManageState(void)
{

}
//------------------------------------------------------------------------------
// ManageThread
//------------------------------------------------------------------------------
void ManageThread(void)
{
	CLSstockCL *ptr;
	MPCL_IT it;
	RUN_STATE state;
	CLSthreadC *pThread;
		
	// Manage server thread
	ThrServer.Manage();

	// Manage client thread
	for (it = Map.Client.begin(); it != Map.Client.end();)
	{
		// Thread 실행 상태 확인
		if ((ptr = it->second) == NULL)
		{
			Log.Write(1, "Map State abnomal [1]");
			Map.Client.erase(it++);
			continue;
		}
		pThread = ptr->Thread;
		if(pThread == NULL)
			Log.Write(1, "Thread is NUMM");
		if (!(pThread->IsRunning(&state)))
			Log.Write(1, "Thread is not running");

		if ((pThread != NULL) && (pThread->IsRunning(&state)))
			++it;
		else
		{
			Log.Write(1, "Map state abnomal [2]");
			DeleteClientX(ptr);
			Map.Client.erase(it++);
		}
	}
	
}
//------------------------------------------------------------------------------
// ManageTest
//------------------------------------------------------------------------------
void ManageTest(void)
{

}
//------------------------------------------------------------------------------
// ManageDebug
//------------------------------------------------------------------------------
void ManageDebug(void)
{
	MPCL_IT it;
	CLSstockCL *ptr;

	if (!ShmPrc->ChangeDebug)
		return;

	ShmPrc->SetDebug(ShmPrc->ReqLevel, ShmPrc->ReqTarget);
	Log.SetDebug(ShmPrc->ReqLevel, ShmPrc->ReqTarget);
	ThrServer.SetDebug(ShmPrc->ReqLevel, ShmPrc->ReqTarget);
	for (it = Map.Client.begin(); it != Map.Client.end(); it++)
	{
		if ((ptr = it->second) == NULL)
			continue;
		//if (ptr->TcpIF == NULL)
		//	continue;

		//ptr->TcpIF->SetDebug(ShmPrc->ReqLevel, ShmPrc->ReqTarget);
	}
	ShmPrc->ChangeDebug = false;
}
//------------------------------------------------------------------------------
// InitDebug
//------------------------------------------------------------------------------
void InitDebug(void)
{
	ShmPrc->RequestLevel(DEFAULT_CPSTOCK_LEVEL);
	ManageDebug();
}
//------------------------------------------------------------------------------
// InitThread
//------------------------------------------------------------------------------
bool InitThread(void)
{
	// Start server thread
	if (!ThrServer.Start())
		return (false);
		
	Log.Write("Server thread start");
	return (true);
}
//------------------------------------------------------------------------------
// TerminateThread
//------------------------------------------------------------------------------
void TerminateThread(void)
{
	CLSstockCL *ptr;
	MPCL_IT it;	
	CLSthreadC *pThread;

	pthread_t thrid = pthread_self();
	

	Log.Write("TerminateThread [0][%d]", thrid);
	// Terminate client thread
	for (it = Map.Client.begin(); it != Map.Client.end();)
	{
		if ((ptr = it->second) == NULL)
		{
			Log.Write("TerminateThread [1] [%d]", ptr->ID);
			++it;
		}
		else if ((pThread = ptr->Thread) == NULL)
		{
			Log.Write("TerminateThread [2] [%d]", ptr->ID);
			++it;
		}
		else if (pThread->IsExist())
		{
			Log.Write("TerminateThread [3] [%d]", ptr->ID);
			pThread->Kill();
		}
		else
		{
			Log.Write("TerminateThread [4] [%d]", ptr->ID);
			delete pThread;
			ptr->Thread = NULL;
			++it;
		}
	}
	// Terminate server thread
	Log.Write("TerminateThread [5]");
	ThrServer.Kill();
	Log.Write("TerminateThread [6]");
}
//------------------------------------------------------------------------------
// InitStockCL
//------------------------------------------------------------------------------
bool InitStockCL(void)
{
	CLSstockCL *pStockCL = ShmPtr->stockCL;
	CLSstockCL *ptr;
	MPDB_IT it;

	// DB Map
	for (int idx = 0; idx < ShmSys->stockCL; idx++, pStockCL++)
	{
		Log.Write("init Map [%d]", pStockCL->Mng.id);
		Map.AddDB(pStockCL->Mng.address, pStockCL);
	}
	int i = 0;
	for (it = Map.m_dbase.begin(); it != Map.m_dbase.end(); it++)
	{		
		//if ((ptr = it->second) == NULL)
		//	continue;
		ptr = it->second;

		Log.Write("MAP StockCL ipaddr: %s[%d]", ptr->Mng.address, ptr->ID);
	}
}
//------------------------------------------------------------------------------
// InitOption
//------------------------------------------------------------------------------
void InitOption(int argc, char ** argv)
{
	
}
//------------------------------------------------------------------------------
// InitMemory
//------------------------------------------------------------------------------
bool InitMemory(void)
{
	if ((ShmPtr = (SHARED_MEM *)ShmMemory.Attach()) == (void *)-1)
		return (false);

	ShmSys = &ShmPtr->system;
	ShmPrc = &ShmPtr->process[PRC_CPSTOCK];
	return (true);
}
//------------------------------------------------------------------------------
// InitSignal
//------------------------------------------------------------------------------
void InitSignal(void)
{
	signal(SIGQUIT, SigHandler);
	signal(SIGINT, SigHandler);
	signal(SIGTERM, SigHandler);
	signal(SIGKILL, SigHandler);
	signal(SIGABRT, SigHandler);
	signal(SIGCLD, SigHandler);
	signal(SIGPIPE, SigHandler);
	signal(SIGCHLD, SigHandler);
	// Daemon 작업 처리
	setpgrp();
	signal(SIGHUP, SIG_IGN);
	signal(SIGTTOU, SIG_IGN);
}
//------------------------------------------------------------------------------
// InitEnv
//------------------------------------------------------------------------------
bool InitEnv(int argc, char **argv)
{
	Log.Write("Process start [%d]", getpid());

	InitSignal();	// 시그널 처리기 초기화
	Log.Write("Init Signal");
	InitOption(argc, argv);		// 옵션 처리

	// 공유메모리 초기화
	if (!InitMemory())
	{
		Log.Write("Shared memory initialization fail");
		return (false);
	}
	Log.Write("Init Memory");
	// Thread 초기화
	if (!InitThread())
	{
		Log.Write("Thread initialization fail");
		return (false);
	}
	Log.Write("Init Thread");
	
	InitStockCL();		// Stock client 정보 초기화
	InitDebug();		// Initialize debugging information	
	ShmPrc->Register(getpid());	// Register process
	gettimeofday(&TMtimer, NULL);
	Log.Write("Init Env");
	return (true);
}
//------------------------------------------------------------------------------
// ClearEnv
//------------------------------------------------------------------------------
void ClearEnv(void)
{
	TerminateThread();		// Terminate thread
	ShmPrc->Deregister(getpid());		// Deregister process
	ShmMemory.Detach();		// Detach shared memory
	Log.Write("Process terminate [%d]", getpid());
	exit(0);
}
//------------------------------------------------------------------------------
// main
//------------------------------------------------------------------------------
int main(int argc, char **argv)
{
	bool initOK;

	// 작업 환경 초기화
	initOK = InitEnv(argc, argv);
	Log.Write("Main start");
	MainThrId = pthread_self();
	Log.Write("Main Thread ID[%d]", MainThrId);
	Log.Write("CPSTOCK main log address %d ", Log);

	//Main Loop
	while (initOK && !NeedTerminate())
	{
		ShmPrc->MarkTime();		// 시작 시작 기록
		ManageDebug();			// 디버깅 관리
		ManageThread();			// Thread 관리
		
		ShmPrc->UpdateRunInfo();	// 실행 정보 갱신
		ShmPrc->Pause(100);			// 1000 msec
	}
	Log.Write("Main end");
	ClearEnv();
}