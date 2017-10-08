//------------------------------------------------------------------------------
// Include
//------------------------------------------------------------------------------
#include "STOCKMAIN.h"
//------------------------------------------------------------------------------
// External
//------------------------------------------------------------------------------
extern bool	LoadDatabase(void);
//------------------------------------------------------------------------------
// Global
//------------------------------------------------------------------------------
CLSlog Log("CLSMAIN", DIR_LOG);
SHARED_MEM *ShmPtr = NULL;
CLSsystem *ShmSys = NULL;
//------------------------------------------------------------------------------
// Local
//------------------------------------------------------------------------------
bool Terminate = false;
CLSprocess *ShmPrc = NULL;
CLSmemory   ShmMemory(YGD_SHM_KEY, SHARED_MEM_SIZE, "SHM");
PRC_DESC PrcDesc[MAX_PROCESS] = { 
				{"STOCKMAIN",""},
				{"CPSTOCK",CPSTOCK_PROCESS }
			};

//------------------------------------------------------------------------------
// SigHandler
//------------------------------------------------------------------------------
void SigHandler(int sig)
{
	switch (sig)
	{
	case SIGTERM:	//15
	case SIGKILL:	//9
		Log.Write("Signal %d accepted", sig);
		Terminate = true;
		break;
	case SIGCHLD:	//17
	case SIGUSR1:   //10
	default:
		Log.Write(1,"Signal %d accepted", sig);
		break;
	}
}
//------------------------------------------------------------------------------
// NeedTerminate
//------------------------------------------------------------------------------
bool NeedTerminate(void)
{
	if (Terminate)
		return (true);
	if (ShmSys->Terminate)
		return (true);

	return (false);
}
//------------------------------------------------------------------------------
// ManageProcess
//------------------------------------------------------------------------------
void ManageProcess(void)
{
	RUN_STATE state;
	CLSprocess *ptr = ShmPtr->process;

	// ���μ��� � ���� Ȯ��
	for (int idx = PRC_STOCKMAIN; idx < MAX_PROCESS; idx++, ptr++)
	{
		// ���μ��� Ȱ�� ���� �� ���� ���� Ȯ��
		if (!ptr->Active)		// Init����
			continue;
		if (ptr->IsRunning(&state))
			continue;

		//Log.Write(2,"%s Run State : %d", ptr->Name, state);
		switch (state)
		{
		case RST_OK:
			Log.Write(2,"%s Run State : RST_OK", ptr->Name);
			break;
		case RST_UNEXIST:
			Log.Write(2,"%s Run State : RST_UNEXIST", ptr->Name);
			break;
		case RST_ABNOMAL:
			Log.Write(2,"%s Run State : RST_ABNOMA", ptr->Name);
			break;
		}

		switch (state)
		{
		case RST_ABNOMAL: ptr->Kill();
		case RST_UNEXIST: ptr->Start();	
			break;
		default: break;
		}
	}
}
//------------------------------------------------------------------------------
// ManageDebug
//------------------------------------------------------------------------------
void ManageDebug(void)
{
	if (!ShmPrc->ChangeDebug)
		return;

	ShmPrc->SetDebug(ShmPrc->ReqLevel, ShmPrc->ReqTarget);
	Log.SetDebug(ShmPrc->ReqLevel, ShmPrc->ReqTarget);
	ShmPrc->ChangeDebug = false;
}
//------------------------------------------------------------------------------
// InitProcess
//------------------------------------------------------------------------------
bool InitProcess(void)
{
	PRC_DESC *dPtr = PrcDesc;
	CLSprocess *ptr = ShmPtr->process;

	for (int idx = PRC_STOCKMAIN; idx < MAX_PROCESS; idx++, ptr++, dPtr++)
	{
		if (!strlen(dPtr->name))
			break;

		if (idx == PRC_STOCKMAIN)
			ptr->Init(dPtr->name);
		else if (!ptr->Start(dPtr))
			return (false);
		else
			Log.Write("Start Process [%s]", ptr->Name);
	}
	return (true);
}
//------------------------------------------------------------------------------
// TerminateProcess
//------------------------------------------------------------------------------
void TerminateProcess(void)
{
	CLSprocess *ptr;

	// �������� �����޸� ������ �ƴ� ��� �����Ѵ�.
	if (ShmPtr == NULL)
		return;

	for (int idx = PRC_STOCKMAIN; idx < MAX_PROCESS;)
	{
		if (idx == PRC_STOCKMAIN)
		{
			idx++;
			continue;
		}
		// ���μ����� Ȱ�� ���� �� ���� ���� Ȯ��
		ptr = &ShmPtr->process[idx];
		if (!ptr->Active || !ptr->IsExist())
		{
			idx++;
			continue;
		}
		// ���μ����� �����Ų��.
		ptr->Kill();
		usleep(50000); // 50 msec
	}
}

//------------------------------------------------------------------------------
// InitDebug
//------------------------------------------------------------------------------
bool InitDatabase(void)
{
	bool loadOK;

	loadOK = LoadDatabase();	// Load database
	return (loadOK);
}
void InitDebug(void)
{
	ShmPrc->RequestLevel(DEF_CLSMAIN_LEVEL);
	ManageDebug();
}
//------------------------------------------------------------------------------
// InitMemory
//------------------------------------------------------------------------------
bool InitMemory(void)
{
	if ((ShmPtr = (SHARED_MEM *)ShmMemory.Create()) == (void *)-1)
		return (false);

	ShmSys = &ShmPtr->system;
	ShmPrc = &ShmPtr->process[PRC_STOCKMAIN];
	return (true);
}
//------------------------------------------------------------------------------
// InitOption
//------------------------------------------------------------------------------
void InitOption(int argc, char **argv)
{
}
//------------------------------------------------------------------------------
// InitSignal
//------------------------------------------------------------------------------
void InitSignal(void)
{
	signal(SIGABRT, SigHandler);	// ������ ���� ���μ��� ���� �ñ׳�
	signal(SIGCHLD, SIG_IGN);		// Child process stop and terminate
	signal(SIGCLD, SigHandler);		// SIGCHID�� ���Ǿ�
	signal(SIGINT, SigHandler);		// Keyboard Interupt
	signal(SIGKILL, SigHandler);	// kill
	signal(SIGQUIT, SigHandler);	// Ctrl-\, QUIT��ȣ�� ����, ���μ����� �����Ų �� �ھ �����ϴ� ����
	signal(SIGTERM, SigHandler);	// �����ȣ
	// Deamon �۾� ó��
	setpgrp();	// Set the process group ID
	signal(SIGHUP,  SIG_IGN);		// Hang up
	signal(SIGTTOU, SIG_IGN);		// ��׶��� ���μ��� ����õ�
}
//------------------------------------------------------------------------------
// InitEnv
//------------------------------------------------------------------------------
bool InitEnv(int argc, char **argv)
{
	Log.Write("Process start");

	InitSignal();	// �ñ׳� ó���� �ʱ�ȭ
	InitOption(argc, argv);

	// �����޸� �ʱ�ȭ
	if (!InitMemory())
	{
		Log.Write("Shared memory initionalization fail");
		return (false);
	}

	InitDebug();	// ����� ���� �ʱ�ȭ

	// Database �ʱ�ȭ
	if (!InitDatabase())
	{
		Log.Write("Database initialization fail");
		return (false);
	}
	// Process �ʱ�ȭ
	if (!InitProcess())
	{
		Log.Write("Process initialization fail");
		return (false);
	}

	ShmPrc->Register(getpid());		// Register process
	ShmPrc->Pause(100);				// 100 msec
	Log.Write("Initialization OK");
	return (true);
}
//------------------------------------------------------------------------------
// ClearEnv
//------------------------------------------------------------------------------
void ClearEnv(void)
{
	usleep(500);	// 500 msec
	Log.Write("Process terminating ....");
	TerminateProcess();
	ShmPrc->Deregister(getpid());	// Deregister process
	ShmMemory.Delete();				// Delete shared memory
	Log.Write("Process terminate");
}
//------------------------------------------------------------------------------
// main
//------------------------------------------------------------------------------
int main(int argc, char **argv)
{
	bool initOK;
	printf("main start\n");
	// �۾� ȯ�� �ʱ�ȭ
	initOK = InitEnv(argc, argv);

	// Main Loop
	while (initOK && !NeedTerminate())
	{
		ShmPrc->MarkTime();
		ManageDebug();		// Debugging ����
		ManageProcess();	// Process ����
		ShmPrc->UpdateRunInfo();	// ���� ���� ����
		ShmPrc->Pause(100);			// 100 msec
	}

	ClearEnv();
	printf("main end\n");
	return (0);
}