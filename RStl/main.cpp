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
	/*
	time_t timer1, timer2;
	time(&timer1);
	test1();
	time(&timer2);
	std::cout << difftime(timer2, timer1) << std::endl;
	time(&timer1);
	test2();
	time(&timer2);
	std::cout << difftime(timer2, timer1) << std::endl;
	system("pause");
	*/
	RStl::RString s("he%1,wor%99");
	s.arg("llo").arg("ld");
	std::cout << s;
	return 0;
}