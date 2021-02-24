#pragma once
#include<memory>
#include<iostream>
#define MEMPOOL_ALIGNMENT 8
#define USHORT unsigned short
#define ULONG unsigned long
struct zone {
	void* pointer;
	int cap;
	zone() :pointer(nullptr), cap(0) {};
	zone(void* p, int l) :pointer(p), cap(l) {};
	~zone() {};
};
enum class ErrorType
{
	Success,
	NullPointer,
	NotFound
};
struct MemoryBlock
{
	USHORT nSize;
	USHORT nFree;
	USHORT nFirst;
	MemoryBlock* pNext;
	char aData[1];
	
	void* operator new(size_t, USHORT nTypes, USHORT nUnitSize)
	{
		return ::operator new(sizeof(MemoryBlock) + nTypes * nUnitSize);
	}
	
	void  operator delete(void* p, size_t)
	{
		::operator delete (p);
	}
	MemoryBlock(USHORT _nTypes = 1, USHORT _nUnitSize = 0);
	~MemoryBlock();
};
	class MemoryPool
	{
	private:
		MemoryBlock** pBlock;
		MemoryPool();
		MemoryPool(USHORT _nUintSize, USHORT _nGrowSize);
		USHORT nUnitSize;
		USHORT nGrowSize;
	public:
		static MemoryPool& GetPool() {
			static MemoryPool pool(8, 1024);
			return pool;
		}
		~MemoryPool();
		static zone Alloc(USHORT _size);
		static zone Ralloc(USHORT origin, USHORT nSize);
		static zone HeapAlloc(USHORT _size);
		static ErrorType Free(void* p);
		static ErrorType Free(void* p, USHORT _size);
	};
