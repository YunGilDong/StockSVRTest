//------------------------------------------------------------------------------
#ifndef	DatabaseH
#define	DatabaseH
//------------------------------------------------------------------------------
// Include
//------------------------------------------------------------------------------
#include "General.h"
//------------------------------------------------------------------------------
#include <time.h>
//------------------------------------------------------------------------------
// Type Definition
//------------------------------------------------------------------------------
typedef struct
{
	int id;	//id
	char address[46];
} DB_STOCKCL;

#define DB_PRCMTHR_SIZE		sizeof(DB_PRCMTHR);
//------------------------------------------------------------------------------
// TEST_DB
//------------------------------------------------------------------------------
typedef struct
{
	int ISPT_EQUIP_ID;
	char ISPT_EQUIP_NM[60];
	int ISPT_EQUIP_TP;
	char ISPT_OFFICE_ID[8];
} TEST_DB;

#define TEST_DB_SIZE	sizeof(TEST_DB);
//------------------------------------------------------------------------------
#endif	//DatabaseH
//------------------------------------------------------------------------------
// End of Database.h
//------------------------------------------------------------------------------