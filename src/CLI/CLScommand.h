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
#define CMDNAME_LEN		16		// 최대 명령어 문자 수
#define CMDUSAGE_LEN	256		// 최대 명령어 용법 문자 수
#define CMDDESC_LEN		256		// 최대 명령어 설명 문자 수
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
	bool Check(char *token, bool run = true);	// 명령어 확인
	void PrintInfo(void);
	void PrintInfoShort(void);
};
//------------------------------------------------------------------------------
#endif	// CLScommandH
//------------------------------------------------------------------------------
// End of CLScommand.h
//------------------------------------------------------------------------------


