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

	// Test Sendmessage (SendSig)	 interval 5 sec
	if (CheckElapsedTime(&m_sigTimer, TEST_SIG__INTERVAL))
		return(SendSignal(BUY_SIG));

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
	int txLength = length + STOCK_HEADER_LEN + 1;	// +1 : LRC
	char message[TCPBUF_LEN], stamp[SHORTBUF_LEN];	
	BYTE lrc;

	memset(message, 0, STOCK_HEADER_LEN);
	message[STOCK_STX1] = STX1_CHAR;
	message[STOCK_STX2] = STX1_CHAR;
	message[STOCK_STATUS] = 0;
	message[STOCK_OPCODE] = code;

	switch (code)
	{
	case HEARTBEAT:
		break;
	case BUY_SIG:
		message[SEQ] = ++m_txSequence;
		if (length && info != NULL)
			memcpy(&message[DATA], info, length);

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
	SetNumber(&message[SIZE1], length + 1, 2);
	lrc = GenLRC((BYTE*)message, txLength - 1);
	message[txLength - 1] = lrc;


	if (!Write(message, txLength))
		return (false);

	Log.FLdump(1, "SEND[RAW]", message, txLength, txLength);

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
	int idx=0;
	char info[STOCK_SHORTBUF_LEN];

	memset(info, 0x00, 47);

	info[idx] = 'b';	m_sigInfo.type = info[idx];		 idx += 1;
	/*info[idx] = 12;		m_sigInfo.mon = info[idx];		 idx += 1;
	info[idx] = 15;		m_sigInfo.day = info[idx];		 idx += 1;
	info[idx] = 17;		m_sigInfo.hour = info[idx];		 idx += 1;
	info[idx] = 16;		m_sigInfo.minute = info[idx];	 idx == 1;*/

	SetNumber(&info[idx], 12151810, 4);					idx += 4;
	

	//SetStockCode(&info[idx]);	idx += 7;
	//SetStockName(&info[idx]);	idx += 32;
	
	snprintf(&info[idx], 7, "%s", "123456");	
	memcpy(m_sigInfo.stockCode, &info[idx], 7);    idx += 7;	// code		

	snprintf(&info[idx], 32, "%s", "삼성전자");	
	memcpy(m_sigInfo.stockNm, &info[idx], 7);    idx += 32;	// name

	SetNumber(&info[idx], 2500, 4);	 m_sigInfo.price = 2500;		idx += 4;

	Log.Debug("%c %d %d %d %d", m_sigInfo.type, m_sigInfo.mon, m_sigInfo.day, m_sigInfo.hour, m_sigInfo.minute);
	Log.Debug("%s %s %d", m_sigInfo.stockCode, m_sigInfo.stockNm, m_sigInfo.price);
	
	return (SendMessage(code, idx, info));
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
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void CLSstockIF::SetStockCode(char *info)		// test용
{
	// size 7
	// 현재 시간값을 구해서 겹치지 않는 범위에서 random하게 생성	
	char data[7];
	int tmV = 0;
	struct timeval tm;
	gettimeofday(&tm, NULL);
	tmV = tm.tv_sec;
	data[0] = tmV % 10;		tmV /= 10;
	data[1] = tmV % 10;		tmV /= 10;
	data[2] = tmV % 10;		tmV /= 10;
	data[3] = tmV % 10;		tmV /= 10;
	data[4] = tmV % 10;		tmV /= 10;
	data[5] = tmV % 10;		tmV /= 10;
	data[6] = tmV % 10;		

	memcpy(info, data, 7);
	Log.Write("dCODE:[%s]", data);
	Log.Write("iCODE:[%s]", info);
}
//------------------------------------------------------------------------------									
//
//------------------------------------------------------------------------------
void CLSstockIF::SetStockName(char *info)		// test용
{
	// size 32
	// 현재 시간값을 구해서 겹치지 않는 범위에서 random하게 생성

	char data[32];
	int tmV = 0;
	struct timeval tm;
	gettimeofday(&tm, NULL);
	tmV = tm.tv_sec;
	data[0] = 'N';
	data[1] = 'M';
	data[2] = tmV % 10;		tmV /= 10;
	data[3] = tmV % 10;		tmV /= 10;
	data[4] = tmV % 10;		tmV /= 10;
	data[5] = tmV % 10;		tmV /= 10;
	data[6] = tmV % 10;		tmV /= 10;
	data[7] = tmV % 10;

	memcpy(info, data, 32);
	Log.Write("dNAME:[%s]", data);
	Log.Write("iNAME:[%s]", info);
}