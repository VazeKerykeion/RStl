#include "RString.h"
static MemoryPool pool(8,1024);

template<typename _Sp>
int str_len(const _Sp* s) {
	int r = 0;
	while (*s != 0) {
		r++;
		s++;
	}
	return r;
}
template<>
int str_len(const char32_t* s) {
	int r = 0;
	while (*s != 0) {
		r++;
		s++;
	}
	return r;
}
RString::RString() :_buf_(nullptr), _len_(0) {

}
RString::~RString() {
	ErrorType e=pool.Free(_buf_,(_len_+1)*2);
	_buf_ = nullptr;
	_len_ = 0;
}

RString::RString(const RString& s) : _buf_(nullptr), _len_(0) {
	assign_str(s._buf_);
}
RString::RString(const char* s) : _buf_(nullptr), _len_(0) {
	assign_str(s);
}
RString::RString(const wchar_t* s) :_buf_(nullptr), _len_(0) {
	assign_str(s);
}
RString::RString(const char16_t* s) : _buf_(nullptr), _len_(0) {
	assign_str(s);
}
RString::RString(const char32_t* s) : _buf_(nullptr), _len_(0) {
	assign_str(s);
}
RString& RString::operator=(const RString& s) {
	assign_str(s._buf_);
	return *this;
}

/*---------------Element access---------------*/
wchar_t& RString::at(int index) {
	if (index >= _len_ || index<0) {
		throw std::out_of_range("Out of range");
	}
	return (wchar_t&)_buf_[index];
}
wchar_t& RString::operator[](int index) {
	return at(index);
}
wchar_t& RString::front() {
	return at(0);
}
wchar_t& RString::back() {
	return at(_len_ - 1);
}
char16_t* RString::data() {
	return _buf_;
}
const char* RString::tochars() {
	char* r = new char[_len_*2+1];
	int t = _len_ * 2;
	int c = 0;
	char* tBuf = (char*)_buf_;
	for (USHORT i = 0; i < t; i++) {
		if (tBuf[i] == 0) continue;
		r[c] = tBuf[i];
		c++;
	}
	r[c] = '\0';
	return r;
}

/*--------------------size--------------------*/
int RString::size() {
	return _len_;
}
bool RString::empty() {
	return _len_ == 0;
}
int RString::

std::wostream& operator<<(std::wostream& os, const RString& s)
{
	for (USHORT i = 0; i < s._len_; i++) {
		os << (wchar_t)s._buf_[i];
		//os << s._buf_[i];
	}
	return os;
}
std::ostream& operator<<(std::ostream& os, const RString& s)
{
	int t = s._len_ * 2;
	char* tBuf = (char*)s._buf_;
	for (USHORT i = 0; i < t; i++) {
		if (tBuf[i] == 0) continue;
		os << tBuf[i];
	}
	return os;
}


template<typename _Sp>
void RString::assign_str(const _Sp* s) {
	static_assert((std::is_same<_Sp, char>::value)
		|| (std::is_same<_Sp, wchar_t>::value)
		|| (std::is_same<_Sp,char16_t>::value)
		, "not pointer to chars");
	ErrorType e = pool.Free(_buf_);
	_buf_ = nullptr;
	_len_ = 0;
	if (s != nullptr) {
		USHORT len = str_len(s);
		_buf_ = (char16_t*)pool.Alloc((len + 1) * 2);
		if (_buf_ == nullptr) {
			return;
		}
		for (USHORT i = 0; i < len; i++) {
			_buf_[i] = (char16_t)s[i];
		}
		_buf_[len] = 0;
		_len_ = len;
		
	}

}

template<>
void RString::assign_str(const char32_t* s) {
	ErrorType e = pool.Free(_buf_);
	_buf_ = nullptr;
	_len_ = 0;
	if (s != nullptr) {
		USHORT len = str_len(s)*2;
		char16_t* t = (char16_t*)s;
		_buf_ = (char16_t*)pool.Alloc((len + 1) * 2);
		if (_buf_ == nullptr) {
			return;
		}
		for (USHORT i = 0; i < len; i++) {
			if (t[i] != 0) {
				_buf_[i] = (char16_t)t[i];
			}
			else {
				_buf_[i] = u'0';
			}
		}
		_buf_[len] = 0;
		_len_ = len;
	}
}
