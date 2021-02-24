#include<string>
#include<vector>
#include<iostream>
#include"MemoryPool.h"
#include"RString.h"

int main()
{	
	RStl::RString s("hello");
	auto t = s.substr(2, 2);
	std::cout << t;

	

	return 0;
}