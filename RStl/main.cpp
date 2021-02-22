#include<string>
#include<vector>
#include<iostream>
#include"MemoryPool.h"

#include <time.h>

using namespace std;

#define MAX_CYCLE 10240000
class CTest
{
private:
	//int ID;
	//int ID2;
	//double ID3;
	char a[3];
};

inline void use_new()
{
	for (int i = 0; i < MAX_CYCLE; i++)
		delete new CTest();
}

inline void use_pool()
{
	MemoryPool pool(8);
	vector<void*> pVec(1024, nullptr);
	for (int i = 0; i < MAX_CYCLE/1024; i++) {

		for (int j = 0; j < 1024; j++) {
			pVec[j] = pool.Alloc(1);
		}for (int j = 0; j < 1024; j++) {
			pool.Free(pVec[j]);
		}

	}
	
}

inline void use_vector()
{
	vector<CTest> testVec(MAX_CYCLE);

}
int main() {
	MemoryPool pool(8,10,10);
	vector<void*> pVec(10, nullptr);

	for (int i = 0; i < 10; i++) {
		pVec[i] = pool.Alloc(1);
	}

	for (int i = 0; i < 9; i++) {
		pool.Free(pVec[i]);
	}

	for (int i = 0; i < 9; i++) {
		pVec[i] = pool.Alloc(1);
	}
	for (int i = 0; i < 9; i++) {
		pool.Free(pVec[i]);
	}

	return 0;

}