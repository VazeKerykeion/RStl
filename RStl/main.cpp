#include<string>
#include<vector>
#include<iostream>
#include"MemoryPool.h"
#include"RString.h"

int main() {
	

	RString test(u"hello");
	test[1] = u'x';
	std::wcout << test << std::endl;

	auto s = test.tochars();
	
	std::cout << s << std::endl;


	
}