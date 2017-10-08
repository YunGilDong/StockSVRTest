//------------------------------------------------------------------------------
#ifndef CLSstockCLH
#define CLSstockCLH
//------------------------------------------------------------------------------
// Include
//------------------------------------------------------------------------------
#include "CLStcp.h"
#include "CLSthreadC.h"
#include "Database.h"
//------------------------------------------------------------------------------
// Type definition
//------------------------------------------------------------------------------
// EQUIP_MNG
//------------------------------------------------------------------------------
typedef struct
{
	int id;	// Ispt Equip Id;
	BYTE type;	// Ispt equip Type
	char address[IPADDR_LEN];
} STOCKCL_MNG;
//------------------------------------------------------------------------------
// EQUIP_OPR
//------------------------------------------------------------------------------
typedef struct
{
	bool stsCom;
} STOCKCL_OPR;

#define EQUIP_OPR_SIZE		sizeof(EQUIP_OPR);
//------------------------------------------------------------------------------
// Class
//------------------------------------------------------------------------------
class CLSstockCL : public CLSobject
{
private:
	void InitState(DB_STOCKCL *pInfo);
public:
	int ID;
	int Socket;
	bool Active;
	STOCKCL_MNG Mng;
	STOCKCL_OPR Opr;
	CLStcp *TcpIF;
	CLSthreadC *Thread;

	CLSstockCL(void);
	~CLSstockCL(void);

	bool CheckState(void);
	bool CheckState(int mode, bool state);
	void Delete(void);
	void Init(void);
	void Init(DB_STOCKCL *pInfo);
};
//------------------------------------------------------------------------------
#endif // !CLSstockCLH
//------------------------------------------------------------------------------
// End of CLSstockCL.h
//------------------------------------------------------------------------------
