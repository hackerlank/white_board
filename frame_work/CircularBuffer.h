#pragma once
#include <stddef.h>
#include "BaseDef.h"


class CircularBuffer
{
	BYTE* m_buffer;
	BYTE* m_bufferEnd;
	BYTE* m_regionAPointer;
	size_t m_regionASize;
	BYTE * m_regionBPointer;
	size_t m_regionBSize;

	inline size_t GetAFreeSpace() { return (m_bufferEnd - m_regionAPointer - m_regionASize); }
	inline size_t GetSpaceBeforeA() { return (m_regionAPointer - m_buffer); }
	inline size_t GetSpaceAfterA() { return (m_bufferEnd - m_regionAPointer - m_regionASize); }
	inline size_t GetBFreeSpace() { if(m_regionBPointer == NULL) { return 0; } return (m_regionAPointer - m_regionBPointer - m_regionBSize); }

public:

	CircularBuffer();
	~CircularBuffer();
	
	void AllocateB();
	void Allocate(size_t size);
	
	void IncrementWritten(size_t len);
	bool Write(const void * data, size_t bytes);
	bool Read(void * destination, size_t bytes);
	void Remove(size_t len);
	void Reset();

	size_t GetSpace();
	size_t GetSize();
	size_t GetContiguiousBytes();
	void * GetBuffer();
	void * GetBufferStart();
};

