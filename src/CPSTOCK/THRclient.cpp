//------------------------------------------------------------------------------
// Include
//------------------------------------------------------------------------------
#include "THRclient.h"
//------------------------------------------------------------------------------
// External Variable
//------------------------------------------------------------------------------
extern CLSlog Log;
extern void THRsigHandler(int);
extern bool NeedTerminate(void);
//------------------------------------------------------------------------------
// Prototype
//------------------------------------------------------------------------------
void TCLclearEnv(CLSstockCL *);
//------------------------------------------------------------------------------
// TCLsigHandler
//------------------------------------------------------------------------------
void TCLsigHandler(CLSstockCL *ptr, int sig)
{
	Log.Write(1, "CL%d Signal %d accepted", ptr->ID, sig);
	switch (sig)
	{
	case SIGUSR1:	// 10
	case SIGSEGV:	// 11
		TCLclearEnv(ptr);
		break;
	case SIGPIPE:	// 13
	default:
		break;
	}
}
//------------------------------------------------------------------------------
// TCLinitNetwork
//------------------------------------------------------------------------------
bool TCLinitNetwork(CLSstockCL *info)
{
	char name[SHORTBUF_LEN];
	CLSthreadC *ptr = info->Thread;

	// Create TCP
	sprintf(name, "VIMSTCP%d", info->ID);
	if ((info->TcpIF = new CLSstockIF(name, 0, info->Socket, TCP_CHILD)) == NULL)
	{
		Log.Write("CLient TCP create fail");
		return (false);
	}
	((CLSstockIF *)info->TcpIF)->SetID(info->ID, info);
	Log.Write("CLient TCP create");
	return (true);
}
//------------------------------------------------------------------------------
// TCLinitSignal
//------------------------------------------------------------------------------
void TCLinitSignal(void)
{
	sigset_t newMask;
	struct sigaction act;

	// Signal 처리기 초기화
	sigemptyset(&newMask);
	sigaddset(&newMask, SIGSEGV);
	sigaddset(&newMask, SIGHUP);
	sigaddset(&newMask, SIGCLD);
	sigaddset(&newMask, SIGCHLD);
	sigaddset(&newMask, SIGPIPE);
	sigaddset(&newMask, SIGUSR1);
	pthread_sigmask(SIG_UNBLOCK, &newMask, NULL);
	act.sa_handler = THRsigHandler;
	sigaction(SIGSEGV, &act, NULL);
	sigaction(SIGHUP, &act, NULL);
	sigaction(SIGCLD, &act, NULL);
	sigaction(SIGCHLD, &act, NULL);
	sigaction(SIGPIPE, &act, NULL);
	sigaction(SIGUSR1, &act, NULL);
}
//------------------------------------------------------------------------------
// TCLinitEnv
//------------------------------------------------------------------------------
bool TCLinitEnv(CLSstockCL *info)
{
	// Signal 처리기 초기화
	pthread_detach(pthread_self());
	TCLinitSignal();

	// Network 초기화
	if (!TCLinitNetwork(info))
		return (false);

	return (true);
}
//------------------------------------------------------------------------------
// TCLclearEnv
//------------------------------------------------------------------------------
void TCLclearEnv(CLSstockCL *info)
{
	Log.Write("Client Thread clear %d [%d]", info->ID, info->Thread->ID);

	// Close client Socket
	if (info->TcpIF != NULL)
	{
		Log.Write("Socket Close");
		info->TcpIF->Close();
	}
	// Terminate thread
	if (info->Thread != NULL)
	{
		Log.Write("thread stop");
		info->Thread->Stop();
	}
}
//------------------------------------------------------------------------------
// THRclient
//------------------------------------------------------------------------------
void *THRclient(void *data)
{
	bool initOK;
	int id=0;
	int cycle = 0;
	CLSstockCL *info;
	CLSthreadC *pThread = NULL;

	// 작업 환경 초기화
	info = (CLSstockCL *)data;
	pThread = info->Thread;
	initOK = TCLinitEnv(info);
	id = info->ID;

	pthread_t thrid = pthread_self();
	Log.Write("Client Thread ID[%d]", thrid);


	Log.Write("THRclient log address %d ", Log);
	// Main loop
	while (initOK && !pThread->Terminate && !NeedTerminate())
	{	
		pThread->MarkTime();
		if ((++cycle) % 500 == 0)
		{
			Log.Write("##CLTHR[%d] stock [%d]", id, cycle);
			Log.Debug("##CLTHR[%d] stock [%d]", id, cycle);
			if (cycle == 100000000) cycle = 0;
		}
		// Client 통신 관리
		/*if (!info->TcpIF->Manage())
		{		
			Log.Write("##CLTHR TCP Mng false");
			break;
		}*/

		pThread->UpdateRunInfo();	// 실행 정보 갱신
		pThread->Pause(5);		// 500 msec
	}
	// 작업 환경 정리
	TCLclearEnv(info);
}
