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
	// ��ɾ� ��ġ ���� Ȯ��
	if (strcmp(token, Command) != 0)
		return (false);

	printf("token:%s, cmd:%s\n", token, Command);
	// ���� ���� Ȯ��
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
	printf("  ����:\t%s\r\n", m_desc);
	printf("  ���:\t%s\r\n", m_usage);
}
//------------------------------------------------------------------------------
// PrintInfoShort
//------------------------------------------------------------------------------
void CLScommand::PrintInfoShort(void)
{
	printf("  %-16s %s\r\n", Command, m_desc);
}