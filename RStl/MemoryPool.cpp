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

void* MemoryPool::Alloc(USHORT _size) {
	if (_size >= 128) return HeapAlloc(_size);
	USHORT index = _size / 8;
	USHORT UnitSize = nUnitSize * (index + 1)+2;
	USHORT GrowSize = nGrowSize / UnitSize;
 	if (!pBlock[index]) {
		if (!GrowSize) return nullptr;
		MemoryBlock* pMyBlock = new(GrowSize, UnitSize) MemoryBlock(GrowSize, UnitSize);
		if (!pMyBlock) return nullptr;
		pMyBlock->pNext = pBlock[index];
		pBlock[index] = pMyBlock;
		return (void*)(pMyBlock->aData+2);
	}
	MemoryBlock* pMyBlock = pBlock[index];
	while (pMyBlock && !pMyBlock->nFree) {
		pMyBlock = pMyBlock->pNext;
	}
	if (pMyBlock) {
		char* pFree = pMyBlock->aData + (pMyBlock->nFirst * UnitSize);
		pMyBlock->nFirst = *((USHORT*)pFree);
		pMyBlock->nFree--;
		return (void*)(pFree+2);
	}
	else {
		if (!GrowSize) return nullptr;
		pMyBlock = new(GrowSize, UnitSize) MemoryBlock(GrowSize, UnitSize);
		if (!pMyBlock) return nullptr;
		pMyBlock->pNext = pBlock[index];
		pBlock[index] = pMyBlock;
		return (void*)(pMyBlock->aData+2);

	}
	return nullptr;
}
void* MemoryPool::HeapAlloc(USHORT _size) {

}
ErrorType MemoryPool::Free(void* pFree) {
	if (pFree == nullptr) return ErrorType::NullPointer;
	pFree = (char*)pFree - 2;
	for (int i = 0; i < 16; i++) {
		MemoryBlock* pMyBlock = pBlock[i];
		if (pMyBlock == nullptr) continue;
		while (((ULONG)pMyBlock->aData > (ULONG)pFree) ||
			((ULONG)pFree >= ((ULONG)pMyBlock->aData + pMyBlock->nSize)))
		{
			pMyBlock = pMyBlock->pNext;
		}
		if (pMyBlock != nullptr) {
			pMyBlock->nFree++;
			*((USHORT*)pFree) = pMyBlock->nFirst;
			pMyBlock->nFirst = (USHORT)(((ULONG)pFree - (ULONG)(pBlock[i]->aData)) / nUnitSize);
			return ErrorType::Success;
		}
		else {
			free(pFree);
			return ErrorType::Success;
		}
	}
}
ErrorType MemoryPool::Free(void* pFree,USHORT _size) {
	if (pFree == nullptr) return ErrorType::NullPointer;
	if (_size >= 128) {
		free(pFree);
		return ErrorType::Success;
	}
	USHORT index = _size / 8;
	MemoryBlock* pMyBlock = pBlock[index];
	while (((ULONG)pMyBlock->aData > (ULONG)pFree) ||
		((ULONG)pFree >= ((ULONG)pMyBlock->aData + pMyBlock->nSize)))
	{
		pMyBlock = pMyBlock->pNext;
	}
	if (pMyBlock != nullptr) {
		pMyBlock->nFree++;
		*((USHORT*)pFree) = pMyBlock->nFirst;
		pMyBlock->nFirst = (USHORT)(((ULONG)pFree - (ULONG)(pBlock[index]->aData)) / nUnitSize);
		return ErrorType::Success;
	}
	else {
		free(pFree);
		return ErrorType::Success;
	}
	
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