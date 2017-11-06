//------------------------------------------------------------------------------
// Include File
//------------------------------------------------------------------------------
#include "CLI.h"
//------------------------------------------------------------------------------
// External
//------------------------------------------------------------------------------
extern bool	Terminate;
extern CLI_INFO CliInfo;
extern SHARED_MEM	*ShmPtr;
extern CLSsystem *ShmSys;
extern CLScommand	CmdTable[];

//------------------------------------------------------------------------------
extern bool CheckEOT(int);
extern bool CheckNumber(int, int, int *, const char *);
extern bool GetNumber(int, int, int *, const char *);
extern void PrnLine(const char *, char, bool);
extern void PrnError(const char *, ...);
extern CLSprocess *GetProcess(void);
//------------------------------------------------------------------------------
// CmdHelp
//------------------------------------------------------------------------------
void CmdHelp(void)
{
	int idx;
	CLScommand *ptr = CmdTable;

	// 전체 명령어 표시 여부 확인
	if (CheckEOT(EOT_DONTCARE))
	{
		PrnLine("[도움말]", '=', true);
		printf("  %-16s 설명\n", "명령");
		PrnLine("", '-', true);
		for (idx = 0; idx < MAX_CLICMD; idx++, ptr++)
			ptr->PrintInfoShort();
		PrnLine("[도움말]", '=', false);
		return;
	}
	// Display request command usage
	for (idx = 0; idx < MAX_CLICMD; idx++, ptr++)
	{
		if (!ptr->Check(CliInfo.token, false))
			continue;
		if (!CheckEOT(EOT_VALID))
			return;

		PrnLine("[도움말]", '=', true);
		printf("  명령:\t%s\r\n", ptr->Command);
		PrnLine("", '-', false);
		ptr->PrintInfo();
		PrnLine("[도움말]", '=', false);
		return;
	}
	PrnError("%%%%ERR_'%s' 은(는) 미정의된 명령어 입니다.\n", CliInfo.token);
}
//------------------------------------------------------------------------------
// CmdProcess
//------------------------------------------------------------------------------
void CmdProcess(void)
{
	RUN_INFO *rPtr;
	CLSprocess *ptr;

	if (!CheckEOT(EOT_VALID))
		return;		

	ptr = ShmPtr->process;		
	PrnLine("[Process]", '-', true);
	printf(" [Process]    %-6s ACT DBG %-8s %-8s WTC  %-13s THREAD\n","PID", "TARGET", "WDC", "ELAPSED TIME");
	PrnLine("", '-', true);

	for (int idx = 0; idx < MAX_PROCESS; idx++, ptr++)
	{
		ptr->SetElapsedTime();
		//printf(" [%d]", idx);
		rPtr = &ptr->RunInfo;
		printf("%-11s   %-6d %3s %3d %-8s %d/%-3d %3d  %6d/%-6d %7s\n", ptr->Name, ptr->ID, (ptr->Active? "YES" : "NO")
				,ptr->Level, ptr->Target,rPtr->wdc , rPtr->prevWdc, rPtr->wtc, rPtr->elapsed, rPtr->maxET,".");
	}
	PrnLine("[Process]", '=', false);
}
//------------------------------------------------------------------------------
// CmdDebug
//------------------------------------------------------------------------------
void CmdDebug(void)
{
	int level;
	CLSprocess *ptr = ShmPtr->process; 

	if ((ptr = GetProcess()) == NULL)	// 입력 프로세스 포인터 리턴
		return;
	if (!GetNumber(0, 9, &level, "디버깅 수준"))
		return;
	if (!CheckEOT(EOT_VALID))
		return;

	ptr->RequestLevel(level);
}
//------------------------------------------------------------------------------
// CmdKill
//------------------------------------------------------------------------------
void CmdKill(void)
{
	CLSprocess *ptr = ShmPtr->process;

	if ((ptr = GetProcess()) == NULL)
		return;
	if (!CheckEOT(EOT_VALID))
		return;

	ptr->Kill();
}
//------------------------------------------------------------------------------
// CmdExit
//------------------------------------------------------------------------------
void CmdExit(void)
{
	if (!CheckEOT(EOT_VALID))
		return;

	Terminate = true;
}
//------------------------------------------------------------------------------
// CmdTerminate
//------------------------------------------------------------------------------
void CmdTerminate(void)
{
	if (!CheckEOT(EOT_VALID))
		return;

	Terminate = true;
	ShmPtr->system.Terminate = true;
}