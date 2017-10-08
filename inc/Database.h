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
// DB_PRC_THR (TEST)
//------------------------------------------------------------------------------
typedef struct
{
	int id;
	int val;
} DB_PRCTHR;

#define DB_PRCTHR_SIZE	sizeof(DB_PRCTHR)
//------------------------------------------------------------------------------
// DB_PRC_THR (TEST)
//------------------------------------------------------------------------------
typedef struct
{
	int id; // id
	char address[46]; //
} DB_PRCTHRC;

//------------------------------------------------------------------------------
// DB_PRC_MTHR
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
// YGD_DB
//------------------------------------------------------------------------------
typedef struct
{
	int ISPT_EQUIP_ID;
	char ISPT_EQUIP_NM[60];
	int ISPT_EQUIP_TP;
	char ISPT_OFFICE_ID[8];
} DB_YGD;

#define YGD_DB_SIZE	sizeof(DB_YGD);

typedef struct 
{
	int ISPT_EQUIP_ID;
	int ISPT_EQUIP_TP;
}DB_AAA;
//------------------------------------------------------------------------------
// DB_EQUIP
//------------------------------------------------------------------------------
typedef struct
{
	int ISPT_EQUIP_ID;			// Ispt Equip ID
	char ISPT_EQUIP_NM[60];		// Ispt Eqiup Name
	int ISPT_EQUIP_TP;			// Ispt Equip Type
	char INSTALL_DT[20];	
	int ISPT_LANE;
	char ISPT_OFFICE_ID[8];
	char IP_ADDR[46];
} DB_EQUIP;

#define DB_EQUIP_SIZE	sizeof(DB_EQUIP);
//------------------------------------------------------------------------------
#endif	//DatabaseH
//------------------------------------------------------------------------------
// End of Database.h
//------------------------------------------------------------------------------