//------------------------------------------------------------------------------
//
// Project: General
// Target: -
// Version: 0.9
// Date: 2017.04.11
// Author: YGD
// Filename : CLStcp.h
// Content: TCP/IP
//
//------------------------------------------------------------------------------
#ifndef	CLStcpH
#define	CLStcpH
//------------------------------------------------------------------------------
// Include 
//------------------------------------------------------------------------------
#include "CLScomm.h"
//------------------------------------------------------------------------------
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
//------------------------------------------------------------------------------
// Type definition
//------------------------------------------------------------------------------
// COM_STATE
//------------------------------------------------------------------------------
typedef enum {COM_START, COM_STOP} COM_STATE;
//------------------------------------------------------------------------------
// CON_RESULT
//------------------------------------------------------------------------------
typedef enum {CON_ERROR, CON_INPROGRESS, CON_OK} CON_RESULT;
//------------------------------------------------------------------------------
// TCP_MODE
//------------------------------------------------------------------------------
typedef enum {TCP_SERVER, TCP_CLIENT, TCP_CHILD} TCP_MODE;
//------------------------------------------------------------------------------
// TCP_DESC
//------------------------------------------------------------------------------
typedef struct
{
	char name[OBJNAME_LEN];
	int port;
	char ipAddr[IPADDR_LEN];
	TCP_MODE mode;
} TCP_DESC;
//------------------------------------------------------------------------------
// Class
//------------------------------------------------------------------------------
class CLStcp : public CLScomm
{
private:
	int  m_port;
	char m_ipAddr[IPADDR_LEN];
	TCP_MODE m_mode;

	void Init(int port, int id, const char *ipAddr, TCP_MODE mode);
	int IsDataReady(void);
	void SetOption(void);
public:
	int NewSocket;

	CLStcp(void);
	CLStcp(const char *name, int port, int id, TCP_MODE mode = TCP_CHILD);
	CLStcp(const char *name, int port, const char *ipAddr, TCP_MODE mode = TCP_SERVER);
	virtual ~CLStcp(void);

	bool Accept(bool *newConnection, char *remoteIP = NULL);
	void Close(void);
	CON_RESULT Connect(void);
	bool Open(void);
	int Read(char *ptr, int length);
	bool Write(char *ptr, int length);

	virtual bool Manage(void);
	virtual void SetComState(COM_STATE state);
};
//------------------------------------------------------------------------------
#endif	// CLStcpH
//------------------------------------------------------------------------------
// End of CLStcp.h
//------------------------------------------------------------------------------