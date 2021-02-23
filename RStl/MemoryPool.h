#pragma once
#include<memory>
#define MEMPOOL_ALIGNMENT 8
#define USHORT unsigned short
#define ULONG unsigned long
enum class ErrorType
{
	Success,
	NullPointer,
	NotExist
};
struct MemoryBlock
{
	USHORT nSize;
	USHORT nFree;
	USHORT nFirst;
	MemoryBlock* pNext;
	char aData[1];
	static void* operator new(size_t, USHORT nTypes, USHORT nUnitSize)
	{
		return ::operator new(sizeof(MemoryBlock) + nTypes * nUnitSize);
	}
	static void  operator delete(void* p, size_t)
	{
		::operator delete (p);
	}
	MemoryBlock(USHORT _nTypes=1,USHORT _nUnitSize=0);
	~MemoryBlock();
};


class MemoryPool
{
private:
	MemoryBlock** pBlock;
	USHORT nUnitSize;
	USHORT nGrowSize;
public:
	MemoryPool(USHORT _nUintSize=8, USHORT _nGrowSize = 1024);
	~MemoryPool();
	void* Alloc(USHORT _size);
	void* HeapAlloc(USHORT _size);
	ErrorType Free(void* p);
	ErrorType Free(void* p, USHORT _size);
};

