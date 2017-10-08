//------------------------------------------------------------------------------
#ifndef CLScommandH
#define CLScommandH
//------------------------------------------------------------------------------
// Include
//------------------------------------------------------------------------------
#include "General.h"
#include <string.h>
//------------------------------------------------------------------------------
// Constant
//------------------------------------------------------------------------------
#define CMDNAME_LEN		16		// �ִ� ��ɾ� ���� ��
#define CMDUSAGE_LEN	256		// �ִ� ��ɾ� ��� ���� ��
#define CMDDESC_LEN		256		// �ִ� ��ɾ� ���� ���� ��
//------------------------------------------------------------------------------
// Type definition
//------------------------------------------------------------------------------
typedef void	(*FUNCPTR)();
//------------------------------------------------------------------------------
// Class
//------------------------------------------------------------------------------
class CLScommand
{
private:
	FUNCPTR m_function;
	char m_usage[CMDUSAGE_LEN];
	char m_desc[CMDDESC_LEN];
public:
	char Command[CMDNAME_LEN];

	//CLScommand(char *name, FUNCPTR function, char *usage, char *desc);
	CLScommand(const char *name, FUNCPTR function, const char *usage, const char *desc);
	bool Check(char *token, bool run = true);	// ��ɾ� Ȯ��
	void PrintInfo(void);
	void PrintInfoShort(void);
};
//------------------------------------------------------------------------------
#endif	// CLScommandH
//------------------------------------------------------------------------------
// End of CLScommand.h
//------------------------------------------------------------------------------


