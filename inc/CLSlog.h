//------------------------------------------------------------------------------
//
// Project: General
// Target: -
// Version: 0.9
// Date: 2017.04.11
// Author: YGD
// Filename : CLSlog.h
// Content: Log
//
//------------------------------------------------------------------------------
#ifndef	CLSlogH
#define	CLSlogH
//------------------------------------------------------------------------------
// Include 
//------------------------------------------------------------------------------
#include "CLSobject.h"
//------------------------------------------------------------------------------
// Class
//------------------------------------------------------------------------------
class CLSlog : public CLSobject
{
private:
	char m_buffer[BUFFER_LEN];
	char m_dir[FILENAME_LEN];
	struct tm m_tod;

	void Logging(void);
public:
	CLSlog(void);
	CLSlog(const char *name, const char *pDir = NULL);
	~CLSlog(void);

	void FLdump(const char *stamp, char *data, int length, int maxLength);
	void FLdump(int level, const char *stamp, char *data, int length, int maxLength);
	void FLdump(int target, int level, const char *stamp, char *data, int length, int maxLength);
	void Init(const char *pDir);
	void Write(const char *format, ...);
	void Write(int level, const char *format, ...);
	void Write(int target, int level, const char *format, ...);
};
//------------------------------------------------------------------------------
#endif // CLSlogH
//------------------------------------------------------------------------------
// End of CLSlog.h
//------------------------------------------------------------------------------
