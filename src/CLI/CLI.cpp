//------------------------------------------------------------------------------
// Include
//------------------------------------------------------------------------------
#include "CLI.h"
//------------------------------------------------------------------------------
// External
//------------------------------------------------------------------------------
extern void CmdHelp(void), CmdProcess(void), CmdDebug(void);
extern void CmdPrcthrc(void);
extern void CmdKill(void), CmdExit(void), CmdTerminate(void);
//------------------------------------------------------------------------------
// Prototype
//------------------------------------------------------------------------------
void ClearEnv(void);
//------------------------------------------------------------------------------
// Global
//------------------------------------------------------------------------------
bool	Terminate = false;
SHARED_MEM	*ShmPtr = NULL;
CLSsystem *ShmSys;
CLI_INFO CliInfo;
CLScommand CmdTable[MAX_CLICMD] = {
	CLScommand("help", (FUNCPTR)CmdHelp, "help [<���>]", "��� ���� ���"),
	CLScommand("process", (FUNCPTR)CmdProcess, "process", "���μ��� ���Ȳ ���"),
	CLScommand("debug", (FUNCPTR)CmdDebug, "debug <���μ���> <����:0-9>", "���μ��� ����� ������ ����"),
	CLScommand("prcthrc", (FUNCPTR)CmdPrcthrc, "PRCTHRC", "PRC_THRC����"),
	CLScommand("kill", (FUNCPTR)CmdKill, "kill <���μ���>", "���μ��� ����"),
	CLScommand("exit", (FUNCPTR)CmdExit, "exit", "CLI ����"),
	CLScommand("terminate", (FUNCPTR)CmdTerminate, "terminate", "CLS system ����")
};
//------------------------------------------------------------------------------
// Local
//------------------------------------------------------------------------------
CLSmemory	ShmMemory(YGD_SHM_KEY, SHARED_MEM_SIZE, "SHM");
CLSlog Log("CLI", DIR_LOG);
//------------------------------------------------------------------------------
// SigHandler
//------------------------------------------------------------------------------
void SigHandler(int sig)
{
	switch (sig)
	{
	case SIGTERM:
	case SIGKILL:
		ClearEnv();
		break;
	default: break;
	}
}
//------------------------------------------------------------------------------
// NeedTerminate
//------------------------------------------------------------------------------
bool NeedTerminate(void)
{
	if (Terminate)
		return (true);

	return (false);
}
//------------------------------------------------------------------------------
// PrintLogo
//------------------------------------------------------------------------------
void PrintLogo(void)
{
	if (Terminate)
		return;

	printf("************************************************************\n");
	printf("*                                                          *\n");
	printf("*                Command Line Interface                    *\n");
	printf("*                                                          *\n");
	printf("************************************************************\n");
	//
}
//------------------------------------------------------------------------------
// PrnLine
//------------------------------------------------------------------------------
void PrnLine(const char *info, char lineChar, bool front)
{
	int infoLen;
	char buffer[DBG_LINE_LEN + 1];

	infoLen = strlen(info);
	memset(buffer, lineChar, DBG_LINE_LEN);
	if (infoLen)
		memcpy(front ? buffer : &buffer[DBG_LINE_LEN - infoLen], info, infoLen);
	buffer[DBG_LINE_LEN] = 0;
	printf("%s\n", buffer);
}
//------------------------------------------------------------------------------
// PrnError
//------------------------------------------------------------------------------
void PrnError(const char *format, ...)
{
	char message[BUFFER_LEN];
	va_list argList;

	va_start(argList, format);
	vsprintf(message, format, argList);
	printf("%%%%ERR_%s\r\n", message);
	va_end(argList);
}
//------------------------------------------------------------------------------
// GetToken
//------------------------------------------------------------------------------
int GetToken(void)
{
	memset(CliInfo.token, 0, TOKEN_LEN);
	for (int idx = 0, count = 0;; idx++, CliInfo.cmdPtr++)
	{
		switch (*CliInfo.cmdPtr)
		{
		case KEY_ENTER:
		case KEY_NEWLINE:
		case KEY_SPACE:
			if (!count)
				break;
			CliInfo.token[count] = 0;
			return (count);
		case NULL:
			if (!count)
				return (0);
			CliInfo.token[count] = 0;
			return (count);
		default:
			if (count <= TOKEN_LEN - 1)
			{
				CliInfo.token[count] = *CliInfo.cmdPtr;
				count++;
			}
		}
	}
}
//------------------------------------------------------------------------------
// GetInput
//------------------------------------------------------------------------------
bool GetInput(const char *info, bool optional)
{
	if (GetToken() <= 0)
	{
		if (!optional)
			PrnError("%s �Է� �ʵ�", info);
		return (false);
	}
	return (true);
}
//------------------------------------------------------------------------------
// CheckNumber
//------------------------------------------------------------------------------
bool CheckNumber(int low, int high, int *result, const char *info)
{
	bool valid;

	*result = atoi(CliInfo.token);
	if (high < low)
		valid = (*result >= low) ? true : false;
	else valid = (*result < low || *result > high) ? false : true;
	if (!valid)
	{
		if (high < low)
			PrnError("%s��(��) %d �̻��̾�� �մϴ�.", info, low);
		else PrnError("%s��(��) %d �� %d ������ ���̾�� �մϴ�.",info, low, high);
		return (false);
	}
	return (true);
}
//------------------------------------------------------------------------------
// GetNumber
//------------------------------------------------------------------------------
bool GetNumber(int low, int high, int *result, const char *info)
{
	bool valid;

	if (GetToken() <= 0)
	{
		PrnError("%s �Է� �ʵ�", info);
		return (false);
	}
	return (CheckNumber(low, high, result, info));
#if 0
	* result = atoi(CliInfo.token);
	if (high < low)
		valid = (*result < low || *result > high) ? false : true;
	else valid = (*result < low || *result > high) ? false : true;
	if (!valid)
	{
		if (high < low)
			PrnError("%s��(��) %d �̻���̾� �մϴ�.", info, low);
		else PrnError("%s��(��) %d �� %d ������ ���̾�� �մϴ�.", info, low, high);
		return (false);
	}
	return (true);
#endif
}
//------------------------------------------------------------------------------
// CheckEOT
//------------------------------------------------------------------------------
bool CheckEOT(int mode)
{
	if (GetToken() > 0)
	{
		printf("what?\n");
		if (mode == EOT_VALID)
			PrnError("'%s' �Է� ����(CheckEOT)", CliInfo.token);
		return (false);
	}
	return (true);
}
//------------------------------------------------------------------------------
// GetProcess
//------------------------------------------------------------------------------
CLSprocess *GetProcess(void)
{
	CLSprocess *ptr = ShmPtr->process;

	if (!GetInput("���μ���", false))
		return (NULL);

	// ���μ��� Ȯ��
	for (int idx = 0; idx < MAX_PROCESS; idx++, ptr++)
	{
		if (strcmp(ptr->Name, CliInfo.token) == 0)
			return (ptr);
	}
	PrnError("�����ǵ� ���μ��� '%s' ����", CliInfo.token);
	return (NULL);
}
//------------------------------------------------------------------------------
// ProcessCmd
//------------------------------------------------------------------------------
void ProcessCmd(void)
{
	CLScommand *ptr = CmdTable;

	// �Է� ���ڸ� �о���δ�.
	if (fgets(CliInfo.command, BUFFER_LEN, stdin) == NULL)
		return;

	// �Է� ���� ������ ����
	CliInfo.cmdPtr = CliInfo.command;
	if (GetToken() <= 0)
		return;

	for (int idx = 0; idx < MAX_CLICMD; idx++, ptr++)
	{
		if (ptr->Check(CliInfo.token, true))
		{
			Log.Write("CLI>>%s", CliInfo.command);
			return;
		}
	}
	PrnError("�����ǵ� ��ɾ�(ProcessCmd) '%s' ����", CliInfo.token);
}
//------------------------------------------------------------------------------
// InitDebug
//------------------------------------------------------------------------------
void InitDebug(void)
{
}
//------------------------------------------------------------------------------
// InitMemory
//------------------------------------------------------------------------------
bool InitMemory(void)
{
	// �����޸� ���� Ȯ��
	if ((ShmPtr = (SHARED_MEM *)ShmMemory.Attach()) == (void *)-1)
		return (false);

	// �����޸� ������ �ʱ�ȭ
	ShmSys = &ShmPtr->system;
	return (true);
}
//------------------------------------------------------------------------------
// InitOption
//------------------------------------------------------------------------------
void InitOption(int argc, char **argv)
{
	int opt;
	while ((opt = getopt(argc, argv, "t")) != -1)
	{
		switch (opt)
		{
		case 't':
			Terminate = true;
			ShmSys->Terminate = true;
			break;
		default:	break;
		}
	}
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
	signal(SIGCHLD, SigHandler);
	// Deamon �۾� ó��
	setpgrp();
	signal(SIGHUP, SIG_IGN);
	signal(SIGTTOU, SIG_IGN);
}
//------------------------------------------------------------------------------
// InitEnv
//------------------------------------------------------------------------------
bool InitEnv(int argc, char **argv)
{
	InitSignal();	// �ñ׳� ó���� �ʱ�ȭ
	// �����޸� �ʱ�ȭ
	if (!InitMemory())
	{
		PrnError("Shared memory initialiaztion fail");
		return (false);
	}
	InitOption(argc, argv);		// �Է� �μ� ó��
	InitDebug();				// ����� ���� �ʱ�ȭ
	PrintLogo();				// �ΰ� ���
	return (true);
}
//------------------------------------------------------------------------------
// ClearEnv
//------------------------------------------------------------------------------
void ClearEnv(void)
{
	ShmMemory.Detach();		// �����޸� ���� ����
}
//------------------------------------------------------------------------------
// main
//------------------------------------------------------------------------------
int main(int argc, char **argv)
{
	bool initOK;

	initOK = InitEnv(argc, argv);

	// Main Loop
	while (initOK && !NeedTerminate())
	{
		printf("CLI>> ");
		ProcessCmd();	// ��ɾ� ó��
	}

	ClearEnv();		// �۾� ȯ�� ����
	return (0);
}