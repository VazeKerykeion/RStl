#include<string>
#include<vector>
#include<iostream>
#include"MemoryPool.h"
#include"RString.h"

int main()
{	
	RStl::RString s;

	for (int i = 0; i < 128; i++) {
		std::cout << i << std::endl;
		s.push_back('a');
	}

	return 0;
}