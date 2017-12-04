//------------------------------------------------------------------------------
// Include
//------------------------------------------------------------------------------
#pragma hdrstop
#include "CLSstockIF.h"
#include "CLSprocess.h"
#include "CLSlog.h"
#include "CLSmap.h"
//------------------------------------------------------------------------------
// Extern
//------------------------------------------------------------------------------
extern CLSlog Log;
extern CLSmap Map;
extern CLSprocess *ShmPrc;
extern CLSsystem *ShmSys;
extern SHARED_MEM *ShmPtr;
//------------------------------------------------------------------------------
// CLSvimsIF
//------------------------------------------------------------------------------
CLSstockIF::CLSstockIF(void)
{

}
//------------------------------------------------------------------------------
CLSstockIF::CLSstockIF(const char *name, int port, const char *ipAddr, TCP_MODE mode)
	: CLStcp(name, port, ipAddr, mode)
{

}
//------------------------------------------------------------------------------
CLSstockIF::CLSstockIF(const char *name, int port, int id, TCP_MODE mode)
	: CLStcp(name, port, id, mode)
{

}
//------------------------------------------------------------------------------
// ~CLSvimsIF
//------------------------------------------------------------------------------
CLSstockIF::~CLSstockIF(void)
{

}
//------------------------------------------------------------------------------
// InitComState
//------------------------------------------------------------------------------
void CLSstockIF::InitComState(bool connected)
{
	
}
//------------------------------------------------------------------------------
// ManageConnection
//------------------------------------------------------------------------------
CON_RESULT CLSstockIF::ManageConnection(void)
{
	bool tryConnect = false;
	CON_RESULT status;
	Log.Write("Manage CON");
	// 연결 상태 확인
	if (Connected)
	{
		Log.Write("TCP CON");
		return (CON_OK);
	}
	// 초기화 상태 확인
	if (!Initialized && !Open())
	{
		Log.Write("TCP CON ERR");
		return (CON_ERROR);
	}
	/*
	// 연결 요구
	if ((status = Connect()) == CON_OK)
		InitComState(true);
	*/
	return (status);
}
//------------------------------------------------------------------------------
// ManageRX
//------------------------------------------------------------------------------
bool CLSstockIF::ManageRX(void)
{
	int count;
	char buffer[TCPBUF_LEN];

	// Read
	if ((count = Read(buffer, TCPBUF_LEN)) <= 0)
	{
		if (count < 0)
			Log.Write(1, "%s Abnomal read[%d] [%d] [%d]", m_stamp, count, errno, m_id);
		return ((count < 0) ? false : true);
	}
	// Process packet
	RxHandler(buffer, count);
	return (true);
}
//------------------------------------------------------------------------------
// RxHandler
//------------------------------------------------------------------------------
void CLSstockIF::RxHandler(char *buffer, int length)
{
	
}
//------------------------------------------------------------------------------
// MsgHandler
//------------------------------------------------------------------------------
void CLSstockIF::MsgHandler(void)
{
	int level; 
	bool waitChecked;
	BYTE code = m_message[STOCK_OPCODE];
	char stamp[SHORTBUF_LEN];

	// 수신메시지 확인
	

	// 수신메시지 처리
	
}
//------------------------------------------------------------------------------
// ManageTimeout
//------------------------------------------------------------------------------
bool CLSstockIF::ManageTimeout(void)
{
	return (true);
}
//------------------------------------------------------------------------------
// ManageTX
//------------------------------------------------------------------------------
bool CLSstockIF::ManageTX(void)
{
	TOM_INFO *ptr = &m_tomInfo;

	// 수신대기 상태 확인
	if (ptr->waiting)
		return (true);

	// Test Sendmessage (SendTest)	 interval 3 sec
	if (CheckElapsedTime(&m_testTimer, TEST__INTERVAL))
		return(SendTest(0x99));

	return (true);
}
//------------------------------------------------------------------------------
// SetRxState
//------------------------------------------------------------------------------
void CLSstockIF::SetRxState(RX_STATE state, int delta)
{

}
//------------------------------------------------------------------------------
// SendMessage
//------------------------------------------------------------------------------
bool CLSstockIF::SendMessage(void)
{
	/* 미수신 메시지 재전송 */
	int level;
	char stamp[SHORTBUF_LEN];
	TOM_INFO *ptr = &m_tomInfo;

	if (!Write(ptr->message, ptr->length))
		return (false);
	
	return (true);
}
//------------------------------------------------------------------------------
bool CLSstockIF::SendMessage(BYTE code, int length, char *info)
{
	int txLength = length;
	char message[TCPBUF_LEN], stamp[SHORTBUF_LEN];	

	switch (code)
	{
	case HEARTBEAT:
		break;
	case BUY_SIG:
		break;
	case SELL_SIG:
		break;
	case ACK:
		break;
	case NACK:
		break;
	default:
		memcpy(message, info, length);
		break;
	}

	if (!Write(message, txLength))
		return (false);

	Log.FLdump(1, "SEND[RAW]", message, length, length);

	return (true);
}
//------------------------------------------------------------------------------
// SendAck
//------------------------------------------------------------------------------
bool CLSstockIF::SendAck(BYTE code, BYTE nackCode)
{
	int idx;
	char info[SHORTBUF_LEN];
	return (SendMessage(0, idx, info));
}
//------------------------------------------------------------------------------
// SendNAck
//------------------------------------------------------------------------------
bool CLSstockIF::SendNAck(BYTE code)
{
	int idx;
	char info[SHORTBUF_LEN];
	return (SendMessage(0, idx, info));
}
bool CLSstockIF::SendSignal(BYTE code)
{
	int idx;
	char info[SHORTBUF_LEN];
	
	return (SendMessage(0x99, idx, info));
}
bool CLSstockIF::SendTest(BYTE code)
{
	int idx = 0;
	char info[SHORTBUF_LEN];
	memset(info, 0x00, 5);
	sprintf(info, "hi, this is test message");
	idx = strlen(info);
	//Log.Debug("test:[%d]:[%s]", idx, info);

	/*info[0] = 0x01;		idx += 1;
	info[1] = 0x02;		idx += 1;
	info[2] = 0x03;		idx += 1;
	info[3] = 0x04;		idx += 1;
	info[4] = 0x05;		idx += 1;*/

	return (SendMessage(code, idx, info));
}
//------------------------------------------------------------------------------
// Manage
//------------------------------------------------------------------------------
bool CLSstockIF::Manage(void)
{
	time(&m_curClock);		// 현재 시각 갱신
	m_curTod = localtime(&m_curClock);

	// Manage RX Timeout
	if (!ManageTimeout())
		return (true);

	// Manage RX
	if (!ManageRX())
	{
		Log.Write("%s RX fail[%d]", m_stamp, m_id);
		Log.Debug("%s RX fail[%d]", m_stamp, m_id);
		Close();
		return (false);
	}
	// Manage TX
	if (!ManageTX())
	{
		Log.Write("%s TX fail[%d]", m_stamp, m_id);
		Log.Debug("%s TX fail[%d]", m_stamp, m_id);
		Close();
		return (false);
	}
	return (true);
}
//------------------------------------------------------------------------------
// SetID
//------------------------------------------------------------------------------
void CLSstockIF::SetID(int id, CLSstockCL *pStockCL)
{
	m_id = id;
	m_pStockCL = pStockCL;
	sprintf(m_stamp, "CLTCP%04d", m_id);
}