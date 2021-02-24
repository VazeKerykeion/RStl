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
	int size()const;
	bool empty()const;
	
	/*--------operation-------*/
	void clear();
	RString& erase(USHORT index = 0, USHORT count = 65535);

	RString& insert(USHORT index, char c, USHORT size = 1);
	RString& insert(USHORT index, wchar_t c, USHORT size = 1);
	RString& insert(USHORT index, char16_t c, USHORT size = 1);
	/*
	RString& insert(USHORT index, const char* s);
	RString& insert(USHORT index, const wchar_t* s);
	RString& insert(USHORT index, const char16_t* s);
	RString& insert(USHORT index, const char32_t* s);
	*/
	RString& insert(USHORT index, const RString& s);
	RString& push_back(char c,USHORT count);
	RString& push_back(wchar_t c, USHORT count);
	RString& push_back(char16_t c, USHORT count);
	RString& push_back(const RString& s);
	RString& pop_back();
	RString& operator+=(const RString& s);
	int compare(const RString& str)const;

	friend RString operator+(const RString& a, const RString& b);
	friend std::wostream& operator<<(std::wostream& os, const RString& s);
	friend std::ostream& operator<<(std::ostream& os, const RString& s);
	/*----------------compare-----------*/
	friend bool operator==(const RString& a, const RString& b);
	friend bool operator!=(const RString& a, const RString& b);
	friend bool operator<(const RString& a, const RString& b);
	friend bool operator>(const RString& a, const RString& b);
	friend bool operator<=(const RString& a, const RString& b);
	friend bool operator>=(const RString& a, const RString& b);
private:
	template<typename _Char>
	RString& _insert(USHORT index, USHORT size, _Char c);

	RString& _insert(USHORT index, const RString& s);
	template<typename _Sp>
	void assign_str(const _Sp* s);
	template<>
	void assign_str(const char32_t* s);

	char16_t* _buf_;
	USHORT _len_;
	USHORT _cap_;

};

