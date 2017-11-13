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
	int id;	// conn seq
	char address[IPADDR_LEN];
} STOCKCL_MNG;
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
