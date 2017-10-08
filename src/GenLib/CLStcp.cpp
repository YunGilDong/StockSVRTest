//------------------------------------------------------------------------------
//
// Project: General
// Target: -
// Version: 0.9
// Date: 2017.04.11
// Author: YGD
// Filename : CLStcp.cpp
// Content: TCP/IP
//
//------------------------------------------------------------------------------
// Include 
//------------------------------------------------------------------------------
#pragma hdrstop
#include "CLStcp.h"
//------------------------------------------------------------------------------
// CLStcp
//------------------------------------------------------------------------------
CLStcp::CLStcp(void)
{
}
//------------------------------------------------------------------------------
CLStcp::CLStcp(const char *name, int port, int id, TCP_MODE mode) : CLScomm(name)
{
	Init(port, id, NULL, mode);
}
//------------------------------------------------------------------------------
CLStcp::CLStcp(const char *name, int port, const char *ipAddr, TCP_MODE mode) : CLScomm(name)
{
	Init(port, 0, ipAddr, mode);
}
//------------------------------------------------------------------------------
// ~CLStcp
//------------------------------------------------------------------------------
CLStcp::~CLStcp(void)
{
}
//------------------------------------------------------------------------------
// Init
//------------------------------------------------------------------------------
void CLStcp::Init(int port, int id, const char *ipAddr, TCP_MODE mode)
{
	m_port = port;
	if (ipAddr != NULL)
		sprintf(m_ipAddr, "%s", ipAddr);
	else memset(m_ipAddr, 0, IPADDR_LEN);
	m_mode = mode;
	if (m_mode != TCP_CHILD)
		return;

	ID = id;
	Initialized = true;
	Connected = true;
}
//------------------------------------------------------------------------------
// IsDataReadyp
//------------------------------------------------------------------------------
int CLStcp::IsDataReady(void)
{
	int retVal;
	fd_set expFds, readFds;
	struct timeval rxTimeout;

	// SOCKET ID 확인
	if (ID <= 0)
		return (-1);

	// 정보 수신 확인
	FD_ZERO(&readFds);
	FD_ZERO(&expFds);
	FD_SET(ID, &readFds);
	FD_SET(ID, &expFds);
	rxTimeout.tv_sec = 0;
	rxTimeout.tv_usec = 10000;	//10 msec
	if ((retVal = select(ID + 1, &readFds, 0, &expFds, &rxTimeout)) <= 0)
		return (retVal);
	if (FD_ISSET(ID, &expFds))
		return (-1);
	if (FD_ISSET(ID, &readFds))
		return (1);

	return (0);
}
//------------------------------------------------------------------------------
// SetOption
//------------------------------------------------------------------------------
void CLStcp::SetOption(void)
{
	int flag = 1;

	setsockopt(ID, SOL_SOCKET, SO_REUSEADDR, (char *)&flag, sizeof(int));
	setsockopt(ID, SOL_SOCKET, SO_KEEPALIVE, (char *)&flag, sizeof(int));	//link check
	setsockopt(ID, IPPROTO_TCP, TCP_NODELAY, (char *)&flag, sizeof(int));
	ioctl(ID, FIONBIO, &flag);
}
//------------------------------------------------------------------------------
// Accept
//------------------------------------------------------------------------------
bool CLStcp::Accept(bool *newConnection, char *remoteIP)
{
	socklen_t length;
	struct sockaddr_in client;

	*newConnection = false;
	if ((NewSocket = accept(ID, (struct sockaddr *)&client, &length)) < 0)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)	//non block socket이라서
			return (true);
		return (false);
	}
	*newConnection = true;
	if (remoteIP != NULL)
	{
		getpeername(NewSocket, (struct sockaddr *)&client, &length);
		sprintf(remoteIP, "%s", inet_ntoa(client.sin_addr));
	}
	Connected = true;
	return (true);
}
//------------------------------------------------------------------------------
// Close
//------------------------------------------------------------------------------
void CLStcp::Close(void)
{
	if (ID < 0)
		return;
	close(ID);
	ID = -1;
	Initialized = false;
	Connected = false;
}
//------------------------------------------------------------------------------
// Connect
//------------------------------------------------------------------------------
CON_RESULT CLStcp::Connect(void)
{
	int error, retVal;
	int length = sizeof(struct sockaddr_in);
	fd_set readFds, writeFds;
	socklen_t optLength;
	struct sockaddr_in server;
	struct timeval timeout = { 0, 50000 };	// 50msec

	// 접속 요구
	memset(&server, 0, length);
	server.sin_family = AF_INET;
	server.sin_port = htons(m_port);
	server.sin_addr.s_addr = inet_addr(m_ipAddr);
	if (connect(ID, (struct sockaddr *)&server, length) > 0)
	{
		Connected = true;
		return (CON_OK);
	}

	//접속 Error 확인
	if (errno != EINPROGRESS && errno != EALREADY)
	{
		usleep(10000);
		return (CON_ERROR);
	}

	// 접속 진행 상태 확인
	FD_ZERO(&readFds);
	FD_SET(ID, &readFds);
	FD_ZERO(&writeFds);
	FD_SET(ID, &writeFds);
	if ((retVal = select(ID + 1, &readFds, &writeFds, NULL, &timeout)) < 0)
	{
		return (CON_ERROR);
	}
	if (FD_ISSET(ID, &readFds) || FD_ISSET(ID, &writeFds))
	{
		optLength = sizeof(int);
		if (getsockopt(ID, SOL_SOCKET, SO_ERROR, &error, &optLength) < 0)
		{
			return (CON_ERROR);
		}
		if (!error)
		{
			Connected = true;
			return (CON_OK);
		}
	}

	return (CON_INPROGRESS);
}
//------------------------------------------------------------------------------
// Open
//------------------------------------------------------------------------------
bool CLStcp::Open(void)
{
	int length = sizeof(struct sockaddr_in);
	struct sockaddr_in server;

	Debug("Socket open [1]");

	// Open socket
	if ((ID = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
	{
		Debug("Socket open fail");
		return (false);
	}

	SetOption();

	if (m_mode != TCP_SERVER)
	{
		Initialized = true;
		return (true);
	}

	// Bind socket
	memset(&server, 0, length);
	server.sin_family = AF_INET;
	server.sin_port = htons(m_port);
	server.sin_addr.s_addr = htons(INADDR_ANY);
	if (bind(ID, (struct sockaddr *)&server, length) < 0)
	{
		Debug("Socket bind fail port[%d]", m_port);
		return (false);
	}
	// Listen
	if (listen(ID, 10) < 0)
	{
		Debug("Socket listen fail");
		return (false);
	}
	Debug("Socket open success[2]");
	Initialized = true;
	return (true);
}
//------------------------------------------------------------------------------
// Read
//------------------------------------------------------------------------------
int CLStcp::Read(char *ptr, int length)
{
	int count, status;

	// 수신할 정보가 있는지 확인
	if ((status = IsDataReady()) <= 0)	// 0 : isDataReady, -1 no
		return (status);
	if ((count = read(ID, ptr, length)) < 0)
	{
		return (-1);
	}
	return (count);
}
//------------------------------------------------------------------------------
// Write
//------------------------------------------------------------------------------
bool CLStcp::Write(char *buffer, int length)
{
	int count, id;

	if ((count = write(ID, buffer, length)) < 0)
		return (false);

	return ((count == length) ? true : false);
}
//------------------------------------------------------------------------------
// Manage
//------------------------------------------------------------------------------
bool CLStcp::Manage(void)
{
	return (true);
}
//------------------------------------------------------------------------------
// SetComState
//------------------------------------------------------------------------------
void CLStcp::SetComState(COM_STATE state)
{
//
}