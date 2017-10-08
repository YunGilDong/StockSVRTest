//------------------------------------------------------------------------------
// Include
//------------------------------------------------------------------------------
#include "CLI.h"
//------------------------------------------------------------------------------
// External
//------------------------------------------------------------------------------
extern void CmdHelp(void), CmdKill(void), CmdExit(void);
//------------------------------------------------------------------------------
// Global
//------------------------------------------------------------------------------
bool	Terminate = false;
SHARED_MEM	*ShmPtr = NULL;
CLSsystem *ShmSys;
CLI_INFO CliInfo;
CLScommand CmdTable[MAX_CLICMD] = {
	CLScommand("help", (FUNCPTR)CmdHelp, "help [<���>]", "��� ���� ���"),
	CLScommand("kill", (FUNCPTR)CmdKill, "kill <���μ���>", "���μ��� ����"),
	CLScommand("exit", (FUNCPTR)CmdExit, "exit", "CLI ����")	
	};
//CLScommand CmdTable("exit", (FUNCPTR)CmdExit, "exit", "CLI ����");

//------------------------------------------------------------------------------
// Local
//------------------------------------------------------------------------------
CLSmemory	ShmMemory(YGD_SHM_KEY, SHARED_MEM_SIZE, "SHM");
CLSlog Log("CLI", DIR_LOG);
// Function
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
void PrnLine(char *info, char lineChar, bool front)
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

	printf("*****************************************************\n");
	printf("*                                                   *\n");
	printf("*                       CLI                         *\n");
	printf("*                                                   *\n");
	printf("*****************************************************\n");
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
			return(count);
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
			break;
		}
	}
}
//------------------------------------------------------------------------------
// GetInput
//------------------------------------------------------------------------------
bool GetInput(char *info, bool optional)
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
// ProcessCmd
//------------------------------------------------------------------------------
void ProcessCmd(void)
{
	CLScommand *ptr = CmdTable;

	// �Է� ���ڸ� �о� ���δ�.
	if (fgets(CliInfo.command, BUFFER_LEN, stdin) == NULL)
		return;

	// �Է� ���� ������ ����
	CliInfo.cmdPtr = CliInfo.command;
	if (GetToken() <= 0)
		return;

	for (int idx = 0; idx < MAX_CLICMD; idx++, ptr++)
	{
		if (ptr->Check(CliInfo.token, true))
			return;
	}
	PrnError("�����ǵ� ��ɾ� '%s' ����", CliInfo.token);
}
//------------------------------------------------------------------------------
// CheckEOT
//------------------------------------------------------------------------------
bool CheckEOT(int mode)
{
	if (GetToken() > 0)
	{
		if (mode == EOT_VALID)
			PrnError("'%s' �Է� ����", CliInfo.token);
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
		return(NULL);

	// ���μ��� Ȯ��
	for (int idx = 0; idx < MAX_PROCESS; idx++, ptr++)
	{
		if (strcmp(ptr->Name, CliInfo.token) == 0)
			return (ptr);
	}
	PrnError("�����ǵ� ���μ��� '%s' ����", CliInfo.token);
	return (NULL);
}

bool InitMemory(void)
{
	// �����޸� ���� Ȯ��
	if ((ShmPtr = (SHARED_MEM *)ShmMemory.Attach()) == (void *)-1)
		return (false);

	// ���� �޸� ������ �ʱ�ȭ
	Log.Debug("Shared Mem Initionalization.");

	ShmSys = &ShmPtr->system;	
	return (true);
}
bool InitOption(int argc, char **argv)
{
	int opt;

	//while ((opt = getopt(argc, argv, 't')) != -1)
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
bool InitEnv(int argc, char **argv)
{
	// ���� �޸� �ʱ�ȭ
	if (!InitMemory())
	{
		PrnError("Shared memory initialization fail");
		return (false);
	}
	InitOption(argc, argv);
	PrintLogo();
	return (true);
}
void ClearEnv(void)
{
	ShmMemory.Detach();
}
int main(int argc, char **argv)
{
	bool InitOK;

	InitOK = InitEnv(argc, argv);	// �۾� ȯ�� �ʱ�ȭ
	printf("main ����\n");
	// Main Loop
	while (InitOK && !NeedTerminate())
	{
		printf("CLI>> ");
		ProcessCmd();		//��ɾ� ó��
	}

	printf("main ����\n");
	return 0;
}