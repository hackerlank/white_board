#include <stdlib.h>
#include <memory.h>
#include <wchar.h>
#include "CircularBuffer.h"

CircularBuffer::CircularBuffer()
{
	m_buffer = m_bufferEnd = m_regionAPointer = m_regionBPointer = NULL;
	m_regionASize = m_regionBSize = 0;
}

CircularBuffer::~CircularBuffer()
{
	free(m_buffer);
}

bool CircularBuffer::Read(void * destination, size_t bytes)
{
	if (m_buffer == NULL)
		return false;

	size_t cnt = bytes;
	size_t aRead = 0, bRead = 0;
	if( (m_regionASize + m_regionBSize) < bytes )
		return false;

	if( m_regionASize > 0 )
	{
		aRead = (cnt > m_regionASize) ? m_regionASize : cnt;
		memcpy(destination, m_regionAPointer, aRead);
		m_regionASize -= aRead;
		m_regionAPointer += aRead;
		cnt -= aRead;
	}

	if( cnt > 0 && m_regionBSize > 0 )
	{
		bRead = (cnt > m_regionBSize) ? m_regionBSize : cnt;
		memcpy((char*)destination + aRead, m_regionBPointer, bRead);
		m_regionBSize -= bRead;
		m_regionBPointer += bRead;
		cnt -= bRead;
	}

	if( m_regionASize == 0 )
	{
		if( m_regionBSize > 0 )
		{
			if( m_regionBPointer != m_buffer )
				memmove(m_buffer, m_regionBPointer, m_regionBSize);

			m_regionAPointer = m_buffer;
			m_regionASize = m_regionBSize;
			m_regionBPointer = NULL;
			m_regionBSize = 0;
		}
		else
		{
			m_regionBPointer = NULL;
			m_regionBSize = 0;
			m_regionAPointer = m_buffer;
			m_regionASize = 0;
		}
	}

	return true;
}

void CircularBuffer::AllocateB()
{
	m_regionBPointer = m_buffer;
}

bool CircularBuffer::Write(const void * data, size_t bytes)
{
	if (m_buffer == NULL)
		return false;

	if( m_regionBPointer != NULL )
	{
		if( GetBFreeSpace() < bytes )
			return false;

		memcpy(&m_regionBPointer[m_regionBSize], data, bytes);
		m_regionBSize += bytes;
		return true;
	}

	if( GetAFreeSpace() < GetSpaceBeforeA() )
	{
		AllocateB();
		if( GetBFreeSpace() < bytes )
			return false;

		memcpy(&m_regionBPointer[m_regionBSize], data, bytes);
		m_regionBSize += bytes;
		return true;
	}
	else
	{
		if( GetAFreeSpace() < bytes )
			return false;

		memcpy(&m_regionAPointer[m_regionASize], data, bytes);
		m_regionASize += bytes;
		return true;
	}
}

size_t CircularBuffer::GetSpace()
{
	if( m_regionBPointer != NULL )
		return GetBFreeSpace();
	else
	{
		if( GetAFreeSpace() < GetSpaceBeforeA() )
		{
			AllocateB();
			return GetBFreeSpace();
		}

		return GetAFreeSpace();
	}
}

size_t CircularBuffer::GetSize()
{
	return m_regionASize + m_regionBSize;
}


size_t CircularBuffer::GetContiguiousBytes()
{
	if( m_regionASize )
		return m_regionASize;
	else
		return m_regionBSize;
}

void CircularBuffer::Remove(size_t len)
{
	size_t cnt = len;
	size_t aRem, bRem;

	if( m_regionASize > 0 )
	{
		aRem = (cnt > m_regionASize) ? m_regionASize : cnt;
		m_regionASize -= aRem;
		m_regionAPointer += aRem;
		cnt -= aRem;
	}

	if( cnt > 0 && m_regionBSize > 0 )
	{
		bRem = (cnt > m_regionBSize) ? m_regionBSize : cnt;
		m_regionBSize -= bRem;
		m_regionBPointer += bRem;
		cnt -= bRem;
	}

	if( m_regionASize == 0 )
	{
		if( m_regionBSize > 0 )
		{
			if( m_regionBPointer != m_buffer )
				memmove(m_buffer, m_regionBPointer, m_regionBSize);
	
			m_regionAPointer = m_buffer;
			m_regionASize = m_regionBSize;
			m_regionBPointer = NULL;
			m_regionBSize = 0;
		}
		else
		{
			m_regionBPointer = NULL;
			m_regionBSize = 0;
			m_regionAPointer = m_buffer;
			m_regionASize = 0;
		}
	}
}


void CircularBuffer::Reset()
{
	m_regionBPointer = NULL;
	m_regionASize = m_regionBSize = 0;
}

void * CircularBuffer::GetBuffer()
{
	if( m_regionBPointer != NULL )
		return m_regionBPointer + m_regionBSize;
	else
		return m_regionAPointer + m_regionASize;
}

void CircularBuffer::Allocate(size_t size)
{
	m_buffer = (BYTE*)malloc(size);
	m_bufferEnd = m_buffer + size;
	m_regionAPointer = m_buffer;
}

void CircularBuffer::IncrementWritten(size_t len)
{
	if( m_regionBPointer != NULL )
		m_regionBSize += len;
	else
		m_regionASize += len;
}

void * CircularBuffer::GetBufferStart()
{
	if( m_regionASize > 0 )
		return m_regionAPointer;
	else
		return m_regionBPointer;
		
}
