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
MemoryPool::MemoryPool(USHORT _nUnitSize, USHORT _nInitSize, USHORT _nGrowSize):pBlock(nullptr),nInitSize(_nInitSize),nGrowSize(_nGrowSize) {
	if (_nUnitSize > MEMPOOL_ALIGNMENT) {
		nUnitSize = (_nUnitSize + (MEMPOOL_ALIGNMENT - 1)) & ~(MEMPOOL_ALIGNMENT - 1);
	}
	else {
		nUnitSize = 8;
	}
}

void* MemoryPool::Alloc(USHORT _size) {
	if (!pBlock) {
		if (!nInitSize) return nullptr;
		MemoryBlock* pMyBlock = new(nInitSize, nUnitSize) MemoryBlock(nInitSize, nUnitSize);
		if (!pMyBlock) return nullptr;
		pMyBlock->pNext = pBlock;
		pBlock = pMyBlock;
		return (void*)(pMyBlock->aData);
	}
	MemoryBlock* pMyBlock = pBlock;
	while (pMyBlock && !pMyBlock->nFree) {
		pMyBlock = pMyBlock->pNext;
	}
	if (pMyBlock) {
		char* pFree = pMyBlock->aData + (pMyBlock->nFirst * nUnitSize);
		pMyBlock->nFirst = *((USHORT*)pFree);
		pMyBlock->nFree--;
		return (void*)pFree;
	}
	else {
		if (!nGrowSize) return nullptr;
		pMyBlock = new(nGrowSize, nUnitSize) MemoryBlock(nGrowSize, nUnitSize);
		if (!pMyBlock) return nullptr;
		pMyBlock->pNext = pBlock;
		pBlock = pMyBlock;
		return (void*)(pMyBlock->aData);

	}
	return nullptr;
}
void MemoryPool::Free(void* pFree) {
	MemoryBlock* pMyBlock = pBlock;
	while (((ULONG)pMyBlock->aData > (ULONG)pFree) ||
		((ULONG)pFree >= ((ULONG)pMyBlock->aData + pMyBlock->nSize)))
	{
		 pMyBlock = pMyBlock->pNext;
	}
	pMyBlock->nFree++;
	*((USHORT*)pFree) = pMyBlock->nFirst;
	pMyBlock->nFirst = (USHORT)(((ULONG)pFree - (ULONG)(pBlock->aData)) / nUnitSize);
	if (pMyBlock->nFree * nUnitSize == pMyBlock->nSize)
	{
		MemoryBlock* tBlock = pBlock;
		if (tBlock != pMyBlock) {
			while (tBlock->pNext != pMyBlock) {
				tBlock = tBlock->pNext;
			}
		}
		tBlock->pNext = pMyBlock->pNext;
		if (tBlock != pMyBlock) {
			delete pMyBlock;
			pMyBlock = nullptr;
		}
		else {
			delete pMyBlock;
			pMyBlock = nullptr;
			tBlock = nullptr;
			pBlock = nullptr;
		}
	}
	
}
MemoryPool::~MemoryPool() {
	MemoryBlock* pMyBlock = pBlock;
	MemoryBlock* t = pMyBlock;
	while (pMyBlock) {
		t = pMyBlock;
		pMyBlock = pMyBlock->pNext;
		delete t;
	}
}