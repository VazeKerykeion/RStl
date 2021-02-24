#include "MemoryPool.h"

MemoryBlock::MemoryBlock(USHORT _nTypes, USHORT _nUnitSize)
	:nSize(_nTypes* _nUnitSize),
	nFree(_nTypes - 1),
	nFirst(1),
	pNext(0)
{
	char* pData = aData;
	for (USHORT i = 1; i < _nTypes; i++) {
		*reinterpret_cast<USHORT*>(pData) = i;
		pData += _nUnitSize;
	}
}

MemoryBlock::~MemoryBlock()
{

}

MemoryPool::MemoryPool(USHORT _nUnitSize, USHORT _nGrowSize): pBlock(nullptr),nGrowSize(_nGrowSize) {
	pBlock = (MemoryBlock**)malloc(sizeof(MemoryBlock*) * 16);
	for (int i = 0; i < 16; i++) {
		pBlock[i] = nullptr;
	}

	if (_nUnitSize > MEMPOOL_ALIGNMENT) {
		nUnitSize = (_nUnitSize + (MEMPOOL_ALIGNMENT - 1)) & ~(MEMPOOL_ALIGNMENT - 1);
	}
	else {
		nUnitSize = 8;
	}
}

zone MemoryPool::Alloc(USHORT _size) {
	MemoryPool& pool=GetPool();
	if (_size >= 128) return HeapAlloc(_size);
	zone r;
	USHORT index = _size / pool.nUnitSize;
	USHORT UnitSize = pool.nUnitSize * (index + 1)+2;
	
	USHORT GrowSize = pool.nGrowSize / UnitSize;
 	if (!pool.pBlock[index]) {
		if (!GrowSize) return r;
		MemoryBlock* pMyBlock = new(GrowSize, UnitSize) MemoryBlock(GrowSize, UnitSize);
		if (!pMyBlock) return r;
		pMyBlock->pNext = pool.pBlock[index];
		pool.pBlock[index] = pMyBlock;
		r.pointer = (void*)(pMyBlock->aData + 2);
		r.cap = UnitSize-2;
		return r;
	}
	MemoryBlock* pMyBlock = pool.pBlock[index];
	while (pMyBlock && !pMyBlock->nFree) {
		pMyBlock = pMyBlock->pNext;
	}
	if (pMyBlock) {
		char* pFree = pMyBlock->aData + (pMyBlock->nFirst * UnitSize);
		pMyBlock->nFirst = *((USHORT*)pFree);
		pMyBlock->nFree--;
		r.pointer = (void*)(pFree + 2);
		r.cap = UnitSize-2;
		return r;
	}
	else {
		if (!GrowSize) return r;
		pMyBlock = new(GrowSize, UnitSize) MemoryBlock(GrowSize, UnitSize);
		if (!pMyBlock) return r;
		pMyBlock->pNext = pool.pBlock[index];
		pool.pBlock[index] = pMyBlock;
		r.pointer = (void*)(pMyBlock->aData + 2);
		r.cap = UnitSize-2;
		return r;

	}
	return r;
}
zone MemoryPool::Ralloc(USHORT origin, USHORT nSize) {
	int size = nSize - origin;
	if (nSize < 64) {
		return Alloc((nSize) * 2);
	}
	else {
		if (size < origin / 2) {
			return Alloc((origin + origin / 2 + 1) * 2);
		}
		else if (size >= origin / 2 && size < origin) {
			return Alloc(4 * origin);
		}
		else {
			return Alloc(4 * size);
		}
	}
}
zone MemoryPool::HeapAlloc(USHORT _size) {

	return zone(malloc(_size),_size);
}
ErrorType MemoryPool::Free(void* pFree) {
	MemoryPool& pool = GetPool();
	if (pFree == nullptr) return ErrorType::NullPointer;
	pFree = (char*)pFree - 2;
	for (int i = 0; i < 16; i++) {
		MemoryBlock* pMyBlock = pool.pBlock[i];
		//if (pMyBlock == nullptr) continue;
		while (pMyBlock!=nullptr && (((ULONG)pMyBlock->aData > (ULONG)pFree) ||
			((ULONG)pFree >= ((ULONG)pMyBlock->aData + pMyBlock->nSize))))
		{
			pMyBlock = pMyBlock->pNext;
		}
		if (pMyBlock != nullptr) {
			pMyBlock->nFree++;
			*((USHORT*)pFree) = pMyBlock->nFirst;
			pMyBlock->nFirst = (USHORT)(((ULONG)pFree - (ULONG)(pool.pBlock[i]->aData)) / (pool.nUnitSize*(i+1)));
			return ErrorType::Success;
		}
	}
	std::cout << "free from heap" << std::endl;
	pFree = (char*)pFree + 2;
	free(pFree);
	return ErrorType::Success;
}
ErrorType MemoryPool::Free(void* pFree,USHORT _size) {
	MemoryPool& pool = GetPool();
	if (pFree == nullptr) return ErrorType::NullPointer;
	if (_size >= 128) {
		free(pFree);
		return ErrorType::Success;
	}
	pFree = (char*)pFree - 2;
	USHORT index = (_size-1) / pool.nUnitSize;
	MemoryBlock* pMyBlock = pool.pBlock[index];
	while (pMyBlock != nullptr && (((ULONG)pMyBlock->aData > (ULONG)pFree) ||
		((ULONG)pFree >= ((ULONG)pMyBlock->aData + pMyBlock->nSize))))
	{
		pMyBlock = pMyBlock->pNext;
	}
	if (pMyBlock != nullptr) {
		pMyBlock->nFree++;
		*((USHORT*)pFree) = pMyBlock->nFirst;
		pMyBlock->nFirst = (USHORT)(((ULONG)pFree - (ULONG)(pool.pBlock[index]->aData)) / (pool.nUnitSize*(index+1)));
		return ErrorType::Success;
	}

	free(pFree);
	return ErrorType::Success;
	
}
MemoryPool::~MemoryPool() {
	for (int i = 0; i < 16; i++) {
		MemoryBlock* pMyBlock = pBlock[i];
		MemoryBlock* t = pMyBlock;
		while (pMyBlock) {
			t = pMyBlock;
			pMyBlock = pMyBlock->pNext;
			delete t;
		}
	}
}