//------------------------------------------------------------------------------
#ifndef CLSstockIFH
#define CLSstockIFH
//------------------------------------------------------------------------------
// Include
//------------------------------------------------------------------------------
#include "SharedMem.h"
#include "General.h"
#include "CLStcp.h"
#include "CLSstockCL.h"

//------------------------------------------------------------------------------
// Constant
//------------------------------------------------------------------------------
#define	TCPBUF_LEN			1024
#define	STOCK_SHORTBUF_LEN	256
//------------------------------------------------------------------------------
// Protocol
//------------------------------------------------------------------------------
#define STOCK_STX1		0
#define STOCK_STX2		STOCK_STX1+1
#define STOCK_SIZE1		STOCK_STX2+1
#define STOCK_SIZE2		STOCK_SIZE1+1
#define STOCK_STATUS	STOCK_SIZE2+1
#define STOCK_OPCODE	STOCK_STATUS+1
#define STOCK_SEQ		STOCK_OPCODE+1
#define STOCK_DATA		STOCK_SEQ+1
//------------------------------------------------------------------------------
#define STOCKDEAL_PORT	12000
//------------------------------------------------------------------------------
// Type definition
//------------------------------------------------------------------------------
typedef enum { STX1, STX2, SIZE1, SIZE2, STATUS, OPCODE, SEQ, DATA, LRC} RX_STATE;
//------------------------------------------------------------------------------
// TOM_INFO (송신 메시지에 대해 수신을 확인할 때)
//------------------------------------------------------------------------------
typedef struct
{
	int count;		// Timeout count
	int sequence;	// 송신 일련번호
	int length;		// 송신 정보수
	int timeout;	// Timeout 대기시간(msec)
	BYTE code;		// 수신대기 코드
	bool waiting;	// 수신 대기 여부
	char message[TCPBUF_LEN];	// 송신 메시지
	struct timeval txTime;	// 송신 시각
} TOM_INFO;

#define TOM_INFO_SIZE		sizeof(TOM_INFO)
//------------------------------------------------------------------------------
// Class
//------------------------------------------------------------------------------
class CLSstockIF : public CLStcp
{
private: 
	int m_id;	// stock client ID
	int m_index;
	int m_length;
	char m_stamp[SHORTBUF_LEN];
	char m_message[TCPBUF_LEN];
	TOM_INFO m_tomInfo;
	CLSstockCL *m_pStockCL;
	time_t m_curClock;
	struct tm *m_curTod;

	void InitComState(bool connected = false);
	CON_RESULT ManageConnection(void);
	bool ManageRX(void);
	void RxHandler(char *buffer, int count);
	void MsgHandler(void);
	bool ManageTimeout(void);
	bool ManageTX(void);	
	void SetRxState(RX_STATE state, int delta = 0);
	bool SendAck(BYTE code, BYTE nackCode = 0);
	bool SendNAck(BYTE code);

	bool SendMessage(void);
	bool SendMessage(BYTE code, int length = 0, char *info = NULL);

	

public:
	CLSstockIF(void);
	CLSstockIF(const char *name, int port, const char *ipAddr, TCP_MODE mode= TCP_SERVER);
	CLSstockIF(const char *name, int port, int id, TCP_MODE mode = TCP_SERVER);
	~CLSstockIF(void);

	bool Manage(void);
	void SetID(int id, CLSstockCL *pStockCL);
};
//------------------------------------------------------------------------------
#endif // !CLSvimsIFH
//------------------------------------------------------------------------------
// End of CLSvimsIF.h
//------------------------------------------------------------------------------