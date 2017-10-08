//------------------------------------------------------------------------------
// Include
//------------------------------------------------------------------------------
#include "CLScommand.h"
//------------------------------------------------------------------------------
// CLScommand
//------------------------------------------------------------------------------
//CLScommand::CLScommand(char *name, FUNCPTR function, char *usage, char *desc)
CLScommand::CLScommand(const char *name, FUNCPTR function, const char *usage, const char *desc)
{
	sprintf(Command, "%s", name);
	m_function = function;
	sprintf(m_usage, "%s", usage);
	sprintf(m_desc, "%s", desc);
}
//------------------------------------------------------------------------------
// Check
//------------------------------------------------------------------------------
bool CLScommand::Check(char *token, bool run)
{
	// 명령어 일치 여부 확인
	if (strcmp(token, Command) != 0)
		return (false);

	printf("token:%s, cmd:%s\n", token, Command);
	// 실행 여부 확인
	if (run)
	{
		printf("run\n");
		m_function();
	}
	return (true);
}
//------------------------------------------------------------------------------
// PrintInfo
//------------------------------------------------------------------------------
void CLScommand::PrintInfo(void)
{
	printf("  설명:\t%s\r\n", m_desc);
	printf("  용법:\t%s\r\n", m_usage);
}
//------------------------------------------------------------------------------
// PrintInfoShort
//------------------------------------------------------------------------------
void CLScommand::PrintInfoShort(void)
{
	printf("  %-16s %s\r\n", Command, m_desc);
}