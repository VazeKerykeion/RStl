#include<string>
#include<vector>
#include<iostream>
#include"MemoryPool.h"
#include"RString.h"

#define COUNT 5000000
void test1() {
	for (int i = 0; i < COUNT; i++) {
		std::string t = std::string("hello") + std::string("world");
	}
}
void test2() {
	for (int i = 0; i < COUNT; i++) {
		//std::cout << i << std::endl;
		RStl::RString t = RStl::RString("hello") + RStl::RString("world");
	}
}

int main()
{	
	RStl::RString s("hello%1");
	s.arg(123);
	std::cout << s;
	return 0;
}