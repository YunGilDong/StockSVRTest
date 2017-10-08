//------------------------------------------------------------------------------
#ifndef	SharedMemH
#define	SharedMemH
//------------------------------------------------------------------------------
// Include File
//------------------------------------------------------------------------------
#include "Define.h"
#include "CLSsystem.h"
#include "CLSprocess.h"
#include "CLSstockCL.h"
//------------------------------------------------------------------------------
// Constant
//------------------------------------------------------------------------------
// Shared Memory Key
//------------------------------------------------------------------------------
#define	YGD_SHM_KEY		0x200
//------------------------------------------------------------------------------
// Process Index
//------------------------------------------------------------------------------
#define PRC_STOCKMAIN		0
#define PRC_CPSTOCK			PRC_STOCKMAIN+1
#define MAX_PROCESS			PRC_CPSTOCK+1
//------------------------------------------------------------------------------
// SHARED_MEM
//------------------------------------------------------------------------------
typedef struct
{
	CLSsystem	system;
	CLSprocess	process[MAX_PROCESS];
	CLSstockCL  stockCL[MAX_STOCKCL];

	int shrArr[10];
	bool Terminate;
} SHARED_MEM;

#define SHARED_MEM_SIZE		sizeof(SHARED_MEM)

//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------
// End of SharedMem.h
//------------------------------------------------------------------------------