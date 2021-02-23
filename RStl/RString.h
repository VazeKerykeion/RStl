#pragma once
#include<string>
#include<iostream>
#include"MemoryPool.h"

template<typename _Sp>
int str_len(const _Sp* s);

template<>
int str_len(const char32_t* s);
class RString
{
public:
	RString();
	~RString();
	/*----------copy----------*/
	RString(const RString& s);
	RString(const char* s);
	RString(const wchar_t* s);
	RString(const char16_t* s);
	RString(const char32_t* s);
	RString& operator=(const RString& s);
	
	/*-----Element access-----*/
	wchar_t& at(int index);
	wchar_t& operator[](int index);
	wchar_t& front();
	wchar_t& back();
	char16_t* data();
	const char* tochars();


	/*----------size----------*/
	int size();
	bool empty();
	
	friend std::wostream& operator<<(std::wostream& os, const RString& s);
	friend std::ostream& operator<<(std::ostream& os, const RString& s);
private:
	template<typename _Sp>
	void assign_str(const _Sp* s);
	template<>
	void assign_str(const char32_t* s);

	char16_t* _buf_;
	int _len_;

};

