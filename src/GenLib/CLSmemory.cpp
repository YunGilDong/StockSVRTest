//------------------------------------------------------------------------------
//
// Project: General
// Target: -
// Version: 0.9
// Date: 2017.04.11
// Author: YGD
// Filename : CLSlog.cpp
// Content: Shared memory
//
//------------------------------------------------------------------------------
// Include
//------------------------------------------------------------------------------
#pragma hdrstop
#include "CLSmemory.h"
//------------------------------------------------------------------------------
// CLSmemory
//------------------------------------------------------------------------------
CLSmemory::CLSmemory(void)
{
	Init();
}
//------------------------------------------------------------------------------
CLSmemory::CLSmemory(const char *name)	: CLSobject(name)
{
	Init();
}
//------------------------------------------------------------------------------
CLSmemory::CLSmemory(key_t key, size_t size, const char *name) : CLSobject(name)
{
	Init(key, size);
}
//------------------------------------------------------------------------------
// ~CLSmemory
//------------------------------------------------------------------------------
CLSmemory::~CLSmemory(void)
{
}
//------------------------------------------------------------------------------
// Init
//------------------------------------------------------------------------------
void CLSmemory::Init(key_t key, size_t size)
{
	m_id = -1;
	m_key = key;
	m_size = size;
	m_address = (void *)-1;
}
//------------------------------------------------------------------------------
// Attach
//------------------------------------------------------------------------------
void *CLSmemory::Attach(void)
{
	// 0666 is 0x1B6, which does not remind me of any magic numbers.

	// Shared memory ���� ���� Ȯ��
	if ((m_id = shmget(m_key, m_size, 0x1B6)) < 0)
		return ((void *)-1);

	m_address = shmat(m_id, 0, 0x1B6);	// ���� �Ҵ�
	return (m_address);
}
//------------------------------------------------------------------------------
void *CLSmemory::Attach(SHM_DESC *pDesc)
{
	// Initialize Variable
	m_key = pDesc->key;
	m_size = pDesc->size;

	return (Attach());
}
//------------------------------------------------------------------------------
// Create
//------------------------------------------------------------------------------
void *CLSmemory::Create(void)
{
	// 0666 is 0x1B6, which does not remind me of any magic numbers.
	// 8���� 0666 is 0x1B6

	// Shared Memory�� �����Ѵ�.
	if ((m_id = shmget(m_key, m_size, 0x1B6 | IPC_CREAT)) < 0)
	{
		// ���� ���� ���� Ȯ��
		if ((m_id = shmget(m_key, m_size, 0x1B6)) < 0)
			return ((void *)-1);
	}

	m_address = shmat(m_id, 0, 0x1B6);
	return (m_address);
}
//------------------------------------------------------------------------------
void *CLSmemory::Create(SHM_DESC *pDesc)
{
	// Initialize Variable
	m_key = pDesc->key;
	m_size = pDesc->size;

	return (Create());
}
//------------------------------------------------------------------------------
// Delete
//------------------------------------------------------------------------------
bool CLSmemory::Delete(void)
{
	// Detach shared memory
	if (!Detach())
		return (false);

	// Remove shared memory
	if (shmctl(m_id, IPC_RMID, (struct shmid_ds *)0) < 0)
		return (false);

	return (true);
}
//------------------------------------------------------------------------------
// Detach
//------------------------------------------------------------------------------
bool CLSmemory::Detach(void)
{
	// Shared memory ���� Ȯ��
	if (m_id < 0)
		return (false);

	// ���� �Ҵ� ���� Ȯ��
	if (m_address == (void *)-1)
		return (false);

	// �Ҵ�� ������ Detach�Ѵ�.
	if (shmdt(m_address) < 0)
		return (false);

	return (true);
}
//------------------------------------------------------------------------------
// End of CLSmemory.cpp
//------------------------------------------------------------------------------