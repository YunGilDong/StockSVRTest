//------------------------------------------------------------------------------
// Include
//------------------------------------------------------------------------------
#include "MngDatabase.h"
//------------------------------------------------------------------------------
// External Variable
//------------------------------------------------------------------------------
extern CLSlog Log;
extern SHARED_MEM *ShmPtr;
extern CLSsystem *ShmSys;
//------------------------------------------------------------------------------
// LoadStockCL
//------------------------------------------------------------------------------
bool LoadStockCL(void)
{
	DB_STOCKCL info[MAX_STOCKCL], *pInfo = info;
	CLSstockCL *pStockCL = ShmPtr->stockCL;
	ShmSys->stockCL = 1;	// temp ( stock client count)

	// Set DB (temporary)
	int id = 10;

	// Initialize info
	for (int idx = 0; idx < ShmSys->stockCL; idx++, pStockCL++, pInfo++, id++)
	{
		sprintf(pInfo->address, "192.168.6.1");

		Log.Debug(1, "\t[%d][%d]:[%s]", idx, pInfo->id, pInfo->address);
		Log.Write(1, "\t[%d][%d]:[%s]", idx, pInfo->id, pInfo->address);
		pStockCL->Init(pInfo);
	}
	return (true);
}
//------------------------------------------------------------------------------
// LoadDatabase
//------------------------------------------------------------------------------
bool LoadDatabase(void)
{
	Log.Write("LoadDatabase..");
	// Initialize Stock client database
	/*if (!LoadStockCL())
	{
		Log.Write("StockCL database initailization fail");
		return (false);
	}
*/
	return (true);	
}