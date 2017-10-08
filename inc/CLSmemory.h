//------------------------------------------------------------------------------
//
// Project: General
// Target: -
// Version: 0.9
// Date: 2017.04.11
// Author: YGD
// Filename : CLSmemory.h
// Content: Shared memory
//
//------------------------------------------------------------------------------
#ifndef	CLSmemoryH
#define	CLSmemoryH
//------------------------------------------------------------------------------
// Include
//------------------------------------------------------------------------------
#include "CLSobject.h"
//------------------------------------------------------------------------------
#include <sys/ipc.h>
#include <sys/shm.h>
//------------------------------------------------------------------------------
// Type Definition
//------------------------------------------------------------------------------
typedef struct
{
	key_t key;
	size_t size;
	char name[OBJNAME_LEN];
}SHM_DESC;

#define SHM_DESC_SIZE	sizeof(SHM_DESC)
//------------------------------------------------------------------------------
// Class
//------------------------------------------------------------------------------
class CLSmemory : public CLSobject
{
private:
	int m_id;
	key_t m_key;
	size_t m_size;
	void *m_address;

	void Init(key_t key = 0, size_t size = 0);
public:
	CLSmemory(void);
	CLSmemory(const char *name);
	CLSmemory(key_t key, size_t size, const char *name);
	~CLSmemory(void);

	void *Attach(void);
	void *Attach(SHM_DESC *pDesc);
	void *Create(void);
	void *Create(SHM_DESC *pDesc);
	bool Delete(void);
	bool Detach(void);
};
//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------
// End of CLSmemory.h
//------------------------------------------------------------------------------