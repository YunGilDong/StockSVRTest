//------------------------------------------------------------------------------
// Include
//------------------------------------------------------------------------------
#pragma	hdrstop
#include "CLSprocess.h"
//------------------------------------------------------------------------------
// CLSprocess
//------------------------------------------------------------------------------
CLSprocess::CLSprocess(void)
{
}
//------------------------------------------------------------------------------
CLSprocess::CLSprocess(const char *name) : CLSrunObject(name)
{
}
//------------------------------------------------------------------------------
// ~CLSprocess
//------------------------------------------------------------------------------
CLSprocess::~CLSprocess(void)
{
}
//------------------------------------------------------------------------------
// Init
//------------------------------------------------------------------------------
void CLSprocess::Init(char *name, int interval)
{
	SetName(name);
	m_interval = interval;
	Active = true;
}
//------------------------------------------------------------------------------
// IsActiveProcess
//------------------------------------------------------------------------------
bool CLSprocess::IsActiveProcess(pid_t id)
{
	return ((id == ID) ? true : false);
}
//------------------------------------------------------------------------------
// IsExist
//------------------------------------------------------------------------------
bool CLSprocess::IsExist(void)
{
	// ID 확인
	if (ID <= 0)
		return (false);	

	// Object 존재 상태 확인
	if (kill(ID, 0) < 0)	 // 프로세스 그룹에 신호를 보냄 0:success, -1 is fail
	{
		ID = 0;
		return(false);
	}

	return (true);
}
//------------------------------------------------------------------------------
// IsRunning
//------------------------------------------------------------------------------
bool CLSprocess::IsRunning(RUN_STATE *state)
{
	// 상태 검증 필요 확인
	*state = RST_OK;
	if (!CheckElapsedTime(&m_timer, m_interval))
		return (true);

	// 프로세스 존재 여부 확인
	if (!IsExist())
	{
		*state = RST_UNEXIST;
		return (false);
	}
	// 프로세스 실행 상태 확인
	if (!CheckRunInfo(state))
	{
		*state = RST_ABNOMAL;
		return (false);
	}
	return (true);
}
//------------------------------------------------------------------------------
// Kill
//------------------------------------------------------------------------------
void CLSprocess::Kill(void)
{
	Debug(1,"Kill[%d]", ID);
	//kill(ID, SIGKILL);
	kill(ID, SIGTERM);
}
//------------------------------------------------------------------------------
// RequestLevel
//------------------------------------------------------------------------------
void CLSprocess::RequestLevel(BYTE level)
{
	ReqLevel = level;
	ChangeDebug = true;
}
//------------------------------------------------------------------------------
// RequestTarget
//------------------------------------------------------------------------------
void CLSprocess::RequestTarget(UINT target)
{
	ReqTarget = target;
	ChangeDebug = true;
}
//------------------------------------------------------------------------------
// Start
//------------------------------------------------------------------------------
bool CLSprocess::Start(void)
{
	ResetRunInfo();
	system(m_command);
	gettimeofday(&m_timer, NULL);
	return (true);	
}
//------------------------------------------------------------------------------
bool CLSprocess::Start(PRC_DESC *pDesc, int interval)
{
	Active = true;
	m_interval = interval;
	SetName(pDesc->name);
	ResetRunInfo();
	sprintf(m_command, "%s&", pDesc->command);
	system(m_command);
	gettimeofday(&m_timer, NULL);
	return (true);
}