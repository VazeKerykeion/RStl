#pragma once
#include<string>
#include<iostream>
#include"MemoryPool.h"

template<typename _Sp>
int str_len(const _Sp* s);

template<>
int str_len(const char32_t* s);

namespace RStl {
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
		wchar_t& at(int index)const;
		wchar_t& operator[](int index)const;
		wchar_t& front();
		wchar_t& back();
		char16_t* data();
		const char* tochars();


		/*----------size----------*/
		int size()const;
		bool empty()const;

		/*--------operation-------*/
		void clear();
		RString& reverse();
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
		RString& push_back(char c, USHORT count = 1);
		RString& push_back(wchar_t c, USHORT count = 1);
		RString& push_back(char16_t c, USHORT count = 1);
		RString& push_back(const RString& s);
		RString& pop_back();
		RString& operator+=(const RString& s);
		RString substr(USHORT start, USHORT end);
		int compare(const RString& str)const;
		template<typename T>
		RString& arg(T t) {
			RString tS = toRString(t);
			int index = -1;
			int n = 100;
			for (int i = 0; i < _len_; i++) {
				if (_buf_[i] == u'%') {
					int a=10, b=10, tN = 99;
					if (i < _len_ - 1) a = _buf_[i + 1] - u'0';
					if (i < _len_ - 2) b = _buf_[i + 2] - u'0';
					if (a >= 0 && a <= 9) {
						if (b >= 0 && b <= 9) {
							tN = a * 10 + b;
						}
						else {
							tN = a;
						}
					}
					if (tN < n) {
						index = i;
						n = tN;
					}
				}
			}
			if (n < 10) {
				erase(index, 2);
			}
			else {
				erase(index, 3);
			}
			insert(index, tS);
			return *this;
		}
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
		/*--------------find---------------*/
		int find(const RString& s);
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
		RString toRString(const RString& t);
		RString toRString(long long t);

	};
	RString operator+(const RString& a, const RString& b);
	std::wostream& operator<<(std::wostream& os, const RString& s);
	std::ostream& operator<<(std::ostream& os, const RString& s);
	bool operator==(const RString& a, const RString& b);
	bool operator!=(const RString& a, const RString& b);
	bool operator<(const RString& a, const RString& b);
	bool operator>(const RString& a, const RString& b);
	bool operator<=(const RString& a, const RString& b);
	bool operator>=(const RString& a, const RString& b);
	
	

	
	
}


	
	