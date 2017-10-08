//------------------------------------------------------------------------------
#ifndef	CLSprocessH
#define	CLSprocessH
//------------------------------------------------------------------------------
// Include
//------------------------------------------------------------------------------
#include "CLSrunObject.h"
//------------------------------------------------------------------------------
// Type definition
//------------------------------------------------------------------------------
// PRC_DESC
//------------------------------------------------------------------------------
typedef struct
{
	char name[OBJNAME_LEN];
	char command[FILENAME_LEN];
} PRC_DESC;
//------------------------------------------------------------------------------
// Class
//------------------------------------------------------------------------------
class CLSprocess : public CLSrunObject
{
private:
	int m_interval;
	char m_command[FILENAME_LEN];
	struct timeval m_timer;
public:
	BYTE ReqLevel;
	UINT ReqTarget;
	bool ChangeDebug;

	CLSprocess(void);
	CLSprocess(const char *name);
	~CLSprocess(void);

	void Init(char *name, int interval = DEF_CHECK_INTERVAL);
	bool IsActiveProcess(pid_t id);
	bool IsExist(void);
	bool IsRunning(RUN_STATE *state);
	void Kill(void);
	void RequestLevel(BYTE level);
	void RequestTarget(UINT target);
	bool Start(void);
	bool Start(PRC_DESC *pDesc, int interval = DEF_CHECK_INTERVAL);
};
//------------------------------------------------------------------------------
#endif // CLSprocessH
//------------------------------------------------------------------------------
// End of CLSprocess.h
//------------------------------------------------------------------------------