#pragma once
#include<string>
class RString
{
public:
	RString();
	RString(const RString&);
	RString(const char*);
	RString(const wchar_t*);
	~RString();
private:
	char16_t* _buf_;
	int _len_;


	
};

