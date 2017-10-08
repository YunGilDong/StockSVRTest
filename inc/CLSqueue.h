//------------------------------------------------------------------------------
#ifndef CLSqueueH
#define CLSqueueH
//------------------------------------------------------------------------------
// Include 
//------------------------------------------------------------------------------
#include "CLScomm.h"
//------------------------------------------------------------------------------
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
//------------------------------------------------------------------------------
// Type Definition
//------------------------------------------------------------------------------
// MSG_BUFFER
//------------------------------------------------------------------------------
#define MSGQUEUE_LEN	1024

typedef struct
{
	long type;
	char text[MSGQUEUE_LEN];
} MSG_BUFFER;
//------------------------------------------------------------------------------
// MSG_DESC
//------------------------------------------------------------------------------
typedef struct
{
	key_t key;
	char name[OBJNAME_LEN];
} MSG_DESC;

#define MSG_DESC_SIZE	sizeof(MSG_DESC);
//------------------------------------------------------------------------------
// Class
//------------------------------------------------------------------------------
class CLSqueue : public CLScomm
{
private:
	int m_id;
	key_t m_key;

	void Init(void);
public:
	CLSqueue(void);
	CLSqueue(const char *name);
	~CLSqueue(void);

	int Count(void);
	bool Create(MSG_DESC *pDesc);
	bool Delete(void);
	bool Open(MSG_DESC *desc);
	int Recv(void *buffer, int length = MSGQUEUE_LEN);
	int Recv(long type, void *buffer, int length = MSGQUEUE_LEN);
	bool Send(void *buffer, int length);
	bool Send(long type, void *buffer, int length);
};
//------------------------------------------------------------------------------
#endif // CLSqueueH
//------------------------------------------------------------------------------
// End of CLSqueue.h
//------------------------------------------------------------------------------

