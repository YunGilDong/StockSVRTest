//------------------------------------------------------------------------------
// Include File
//------------------------------------------------------------------------------
#include "THRserver.h"
//------------------------------------------------------------------------------
// External Variable
//------------------------------------------------------------------------------
extern CLSlog Log;
extern CLSmap Map;
extern SHARED_MEM *ShmPtr;
extern CLSthreadC ThrServer;
extern queue<int> PoolIndexQ;

extern void DeleteClient(CLSstockCL *ptr, char *address);
extern void *THRclient(void *);
extern void THRsigHandler(int);
extern bool NeedTerminate(void);
//------------------------------------------------------------------------------
// Prototype
//------------------------------------------------------------------------------
void TSVclearEnv(void);
//------------------------------------------------------------------------------
// Global Variable
//------------------------------------------------------------------------------
// Local Variable
CLStcp TcpServer("StockCL", TCP_SERVER_PORT, "");
//------------------------------------------------------------------------------
// CLStcp TcpServer("EQUIPSVTCP","TCP_SERVER_PORT, "");
//------------------------------------------------------------------------------
// TSVsigHandler
//------------------------------------------------------------------------------
void TSVsigHandler(int sig)
{
	Log.Write(1, "Server signal %d accepted", sig);
	switch (sig)
	{
	case SIGUSR1:	// 10
	case SIGSEGV:	// 11
	case SIGPIPE:	// 13
		TSVclearEnv();
		break;
	default:	break;
	}
}
//------------------------------------------------------------------------------
// TSVcreateClient
//------------------------------------------------------------------------------
bool TSVcreateClient(char *remoteIP)
{
	char name[SHORTBUF_LEN];
	CLSstockCL *cPtr, *dPtr;

	// Client 접속시점에 remoteIP Map 등록
	// Map에 remoteIP등록 및 thread생성 및 실행

	Log.Debug("createClient IP:[%s]", remoteIP);
	// 등록된 Equip 인지 확인
	if ((dPtr = Map.GetDB(remoteIP)) == NULL)
	{
		// Ip Init();
		// AddDB(ip, pStockLC)
		return (false);
	}
	// 존재하는 Client확인
	sprintf(name, "CLTHR%d", dPtr->ID);
	if ((cPtr = Map.Get(remoteIP)) != NULL)
	{
		Log.Write(1, "Client already exist [%s]", name);
		close(TcpServer.NewSocket);
		DeleteClient(cPtr, remoteIP);
		return (false);
	}
	// Client thread create
	dPtr->Socket = TcpServer.NewSocket;
	if ((dPtr->Thread = new CLSthreadC(name, THRclient, dPtr)) == NULL)
	{
		Log.Write(1, "Client create fail [%s]", name);
		close(TcpServer.NewSocket);
		delete dPtr->Thread;
		return (false);
	}
	// Thread 관리를 위해 map 생성
	if (!Map.Add(remoteIP, dPtr))
	{
		Log.Write(1, "Client add fail [%s]", name);
		return (false);
	}
	// Child thread start
	Log.Write(1, "New client added [%s]", name);
	Log.Debug("New client added [%s]", name);
	if (!dPtr->Thread->Start())
	{
		Log.Write(1, "Client start fail [%s]", name);
		return (false);
	}
	Log.Debug("New client start [%s]", name);
	Log.Write("New client start [%s]", name);
	dPtr->Active = true;
	return (true);
}
//------------------------------------------------------------------------------
// GetPoolIndex
//------------------------------------------------------------------------------
int GetPoolIndex()
{
	int poolIdx = 0;
		
	Log.Debug("thr q size  : %d", PoolIndexQ.size());
	Log.Debug("thr q empty : %d", PoolIndexQ.empty());
	Log.Debug("thr q front : %d", PoolIndexQ.front());

	if (!PoolIndexQ.empty())
	{
		Log.Debug("thr Q OK");
		poolIdx = PoolIndexQ.front();
		PoolIndexQ.pop();
	}
	else 
		return -1;

	return poolIdx;
}
//------------------------------------------------------------------------------
// TSVmanage
//------------------------------------------------------------------------------
bool TSVmanage(void)
{
	bool newConnection;
	char remoteIP[SHORTBUF_LEN];

	if (!TcpServer.Accept(&newConnection, remoteIP))
	{
		Log.Debug("TSV accept fail");
		Log.Write("TSV accept fail");
		return (false);
	}
	if (!newConnection)
		return (true);
	if (!TSVcreateClient(remoteIP))
		return (true);

	return (true);
}
//------------------------------------------------------------------------------
// TSVmanageTest
//------------------------------------------------------------------------------
bool TSVmanageTest(void)
{
	static int CallCnt = 0;
	char * id1 = "192.168.6.1", *id2 = "192.168.10.200", *id3 = "192.168.42.1";
	char * id4 = "192.168.10.205", *id5 = "192.168.10.201";
	//id1 = 1000;	id2 = 1001; id3 = 1002; id4 = 1003; id5 = 1234;

	++CallCnt;
	if (CallCnt > 2) return (true);
	if (CallCnt == 1)
	{
		if (TSVcreateClient(id1))
		{
			
			return (true);
		}
	}/*
	else if (CallCnt == 2)
	{
		if (TSVcreateClient(id2))
		{
			
			return (true);
		}
	}
	else if (CallCnt == 3)
	{
		if (TSVcreateClient(id3))
		{
			
			return (true);
		}
	}
	else if (CallCnt == 4)
	{
		if (TSVcreateClient(id4))
		{
			
			return (true);
		}
	}
	else if (CallCnt == 5)
	{
		if (TSVcreateClient(id5))
		{
			
			return (true);
		}
	}
	*/
	return (true);
}
//------------------------------------------------------------------------------
// TSVinitNetwork
//------------------------------------------------------------------------------
bool TSVinitNetwork(void)
{
	// open server socket
	if (!TcpServer.Open())
	{
		Log.Write("Socket open fail");
		return (false);
	}
	Log.Write("Socket open Success");

	return (true);
}
//------------------------------------------------------------------------------
// TSVinitSignal
//------------------------------------------------------------------------------
void TSVinitSignal(void)
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
	pthread_sigmask(SIG_UNBLOCK, &newMask, NULL);	// thread signal 초기화, 위에서 초기화한 시그널만 프로세스에서 받아들임
	act.sa_handler = THRsigHandler;
	sigaction(SIGSEGV, &act, NULL);
	sigaction(SIGHUP, &act, NULL);
	sigaction(SIGCLD, &act, NULL);
	sigaction(SIGCHLD, &act, NULL);
	sigaction(SIGPIPE, &act, NULL);
	sigaction(SIGUSR1, &act, NULL);
}
//------------------------------------------------------------------------------
// TSVinitEnv
//------------------------------------------------------------------------------
bool TSVinitEnv(void)
{
	// Signal 처리기 초기화
	pthread_detach(pthread_self());
	TSVinitSignal();

	// Network초기화
	if (!TSVinitNetwork())
	{
		Log.Debug("Server network initialization fail");
		Log.Write("Server networks initialization fail");
		return (false);
	}

	return true;
}
//------------------------------------------------------------------------------
// TSVclearEnv
//------------------------------------------------------------------------------
void TSVclearEnv(void)
{
	Log.Write("Server thread clear");
	Log.Debug("Server thread clear");
	TcpServer.Close();
	ThrServer.Stop();	// Terminate thread
}
//------------------------------------------------------------------------------
// THRserver
//------------------------------------------------------------------------------
void *THRserver(void *data)
{
	bool initOK;
	int cycle = 0;
	initOK = TSVinitEnv();	// 작업 환경 초기화

	pthread_t id = pthread_self();
	Log.Debug("Server Thread ID[%d] init:[%d]", id, initOK);

	Log.Debug("THRserver log address %d ", Log);
	//Log.Write("THR Pool IDX : %d", GetPoolIndex());
	Log.Debug("THR Pool IDX : %d", GetPoolIndex());

	// Main loop
	while (initOK && !ThrServer.Terminate && !NeedTerminate())
	{
		//Log.Debug("VIMS server run [%d]##", cycle++); 
		ThrServer.MarkTime();

		if (!TSVmanageTest())
			break;
		//if (!TSVmanage())
		//		break;

		ThrServer.UpdateRunInfo();	// 실행 정보 갱신
		ThrServer.Pause(5);		// 500 msec
	}
	Log.Write("THRserver close");
	Log.Debug("THRserver close");
	//TCLclearEnv();		// 작업 환경 정리
}