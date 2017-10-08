//------------------------------------------------------------------------------
// Include
//------------------------------------------------------------------------------
#pragma hdrstop
//------------------------------------------------------------------------------
#include "CLSqueue.h"
//------------------------------------------------------------------------------
// CLSqueue
//------------------------------------------------------------------------------
CLSqueue::CLSqueue(void)
{
	Init();
}
//------------------------------------------------------------------------------
CLSqueue::CLSqueue(const char *name)
{
	Init();
}
//------------------------------------------------------------------------------
// ~CLSqueue
//------------------------------------------------------------------------------
CLSqueue::~CLSqueue(void)
{
}
//------------------------------------------------------------------------------
// Init
//------------------------------------------------------------------------------
void CLSqueue::Init(void)
{
	m_id = -1;
	m_key = 0;
}
//------------------------------------------------------------------------------
// Count
//------------------------------------------------------------------------------
int CLSqueue::Count(void)
{
	struct msqid_ds info;

	// Message queue 확인
	if (m_id < 0)
		return (-1);

	// Message queue 정보 확인
	if (msgctl(m_id, IPC_STAT, &info) < 0)	// 현재 상태를 info(buf)에 저장
		return (-1);

	return (info.msg_qnum);
}
//------------------------------------------------------------------------------
// Create
//------------------------------------------------------------------------------
bool CLSqueue::Create(MSG_DESC *pDesc)
{
	// 0666 is 0x1B6, which does not remind me of any magic numbers.S
	// Message queue를 생성한다.
	m_key = pDesc->key;
	if ((m_id = msgget(m_key, 0x1B6 | IPC_CREAT)) < 0)
		return (false);

	printf("Q Create\n");
	return (true);
}
//------------------------------------------------------------------------------
// Delete
//------------------------------------------------------------------------------
bool CLSqueue::Delete(void)
{
	// Message queue 확인
	if (m_id < 0)
		return (true);
	// Message queue를 삭제한다.
	if (msgctl(m_id, IPC_RMID, (struct msqid_ds *)0) < 0)
		return (false);

	return (true);
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
bool CLSqueue::Open(MSG_DESC *desc)
{
	m_key = desc->key;
	if ((m_id = msgget(m_key, 0x1B6)) < 0)
		return (false);
	
	return (true);
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
int CLSqueue::Recv(void *buffer, int length)
{
	return (Recv(0, buffer, length));
}
//------------------------------------------------------------------------------
int CLSqueue::Recv(long type, void *buffer, int length)
{
	int count;
	MSG_BUFFER message;

	printf("Q Recv\n");
	message.type = type;
	if ((count = msgrcv(m_id, &message, length, type, IPC_NOWAIT | MSG_NOERROR)) < 0)
	{
		// EINTR : Received a signal while waiting to write
		// ENOMSG : Flag가 IPC_NOWAIT가 넘겨지고 이용가능한 메시지가 없으면 ENOMSG를 반환
		if (errno != ENOMSG && errno != EINTR)
			return (-1);

		return (0);
	}
	if (count > 0)
		memcpy(buffer, message.text, count);
	return (count);

}
//------------------------------------------------------------------------------
// Send
//------------------------------------------------------------------------------
bool CLSqueue::Send(void *buffer, int length)
{
	return (Send(1, buffer, length));
}
//------------------------------------------------------------------------------
bool CLSqueue::Send(long type, void *buffer, int length)
{
	MSG_BUFFER message;
	printf("Q Send\n");

	if (m_id < 0)
		return (false);
	if (type <= 0 || length <= 0 || length > MSGQUEUE_LEN)
		return (false);

	message.type = type;
	memcpy(message.text, buffer, length);
	if (msgsnd(m_id, &message, length, IPC_NOWAIT) < 0)
		return (false);

	return (true);
}
//------------------------------------------------------------------------------
// End of CLSqueue.cpp
//------------------------------------------------------------------------------