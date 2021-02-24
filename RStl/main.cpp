#include<string>
#include<vector>
#include<iostream>
#include"MemoryPool.h"
#include"RString.h"

class a {};

int main()
{
	RStl::RString test("hello");
	test.insert(5,",world");

	std::cout << test;

	return 0;
}