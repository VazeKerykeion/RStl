#include "RString.h"

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
RStl::RString::RString() :_buf_(nullptr), _len_(0),_cap_(0) {

}
RStl::RString::~RString() {
	ErrorType e=MemoryPool::Free(_buf_,_cap_*2);
	_buf_ = nullptr;
	_len_ = 0;
}

RStl::RString::RString(const RString& s) :_buf_(nullptr), _len_(0), _cap_(0) {
	assign_str(s._buf_);
}
RStl::RString::RString(const char* s) : _buf_(nullptr), _len_(0), _cap_(0) {
	assign_str(s);
}
RStl::RString::RString(const wchar_t* s) : _buf_(nullptr), _len_(0), _cap_(0) {
	assign_str(s);
}
RStl::RString::RString(const char16_t* s) : _buf_(nullptr), _len_(0), _cap_(0) {
	assign_str(s);
}
RStl::RString::RString(const char32_t* s) : _buf_(nullptr), _len_(0), _cap_(0) {
	assign_str(s);
}
RStl::RString& RStl::RString::operator=(const RString& s) {
	assign_str(s._buf_);
	return *this;
}

/*---------------Element access---------------*/
wchar_t& RStl::RString::at(int index)const {
	if (index >= _len_ || index<0) {
		throw std::out_of_range("Out of range");
	}
	return (wchar_t&)_buf_[index];
}
wchar_t& RStl::RString::operator[](int index)const{
	return at(index);
}
wchar_t& RStl::RString::front() {
	return at(0);
}
wchar_t& RStl::RString::back() {
	return at(_len_ - 1);
}
char16_t* RStl::RString::data() {
	return _buf_;
}
const char* RStl::RString::tochars() {
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
int RStl::RString::size()const {
	return _len_;
}
bool RStl::RString::empty() const {
	return _len_ == 0;
}

/*--------operation-------*/
void RStl::RString::clear() {
	MemoryPool::Free(_buf_);
	_buf_ = nullptr;
	_len_ = 0;
	_cap_ = 0;
}
RStl::RString& RStl::RString::reverse() {
	int i = 0, j = _len_ - 1;
	char16_t t=0;
	while (i < j) {
		t = _buf_[i];
		_buf_[i] = _buf_[j];
		_buf_[j] = t;
		i++;
		j--;
	}
	return *this;
}
RStl::RString& RStl::RString::erase(USHORT index , USHORT count) {
	if (index + count > _len_) {
		_buf_[index] = 0;
		_len_ = index;
		return *this;
	}
	for (int i = index; i + count <= _len_; i++) {
		_buf_[i] = _buf_[i + count];
	}
	_len_ -= count;
	return *this;
}

RStl::RString& RStl::RString::insert(USHORT index, char c, USHORT size) {
	return _insert(index, size, c);
}
RStl::RString& RStl::RString::insert(USHORT index, wchar_t c, USHORT size) {
	return _insert(index, size, c);
}
RStl::RString& RStl::RString::insert(USHORT index, char16_t c, USHORT size) {
	return _insert(index, size, c);
}
/*
RString& RStl::RString::insert(USHORT index, const char* s) {
	return _insert(index, s);
}
RString& RStl::RString::insert(USHORT index, const wchar_t* s) {
	return _insert(index, s);
}
RString& RStl::RString::insert(USHORT index, const char16_t* s) {
	return _insert(index, s);
}
RString& RStl::RString::insert(USHORT index, const char32_t* s) {
	return _insert(index, s);
}
*/
RStl::RString& RStl::RString::insert(USHORT index, const RString& s) {
	return _insert(index, s);
}

RStl::RString& RStl::RString::push_back(char c, USHORT count) {
	return _insert(_len_,count, c);
}
RStl::RString& RStl::RString::push_back(wchar_t c, USHORT count) {
	return _insert(_len_, count, c);
}
RStl::RString& RStl::RString::push_back(char16_t c, USHORT count) {
	return _insert(_len_, count, c);
}
RStl::RString& RStl::RString::push_back(const RString& s) {
	return _insert(_len_, s);
}
RStl::RString& RStl::RString::pop_back() {
	return erase(_len_ - 1, 1);
}
RStl::RString& RStl::RString::operator+=(const RString& s) {
	return _insert(_len_, s);
}
RStl::RString RStl::RString::substr(USHORT start, USHORT count) {
	if (start + count > _len_) count = _len_ - start;
	RString r;
	r.insert(0, '0', count);
	for (int i = 0; i < count; i++) {
		r[i] = _buf_[i + start];
	}
	return r;
}
int RStl::RString::compare(const RString& str)const  {
	int tLen = _len_ < str._len_ ? _len_ : str._len_;
	for (int i = 0; i < tLen; i++) {
		if (_buf_[i] < str._buf_[i]) return 1;
		else if (_buf_[i] > str._buf_[i]) return -1;
	}
	if (_len_ == str._len_) return 0;
	else if (_len_ < str._len_) return 1;
	else return -1;
}
bool RStl::operator==(const RString& a, const RString& b) {
	return a.compare(b) == 0;
}
bool RStl::operator!=(const RString& a, const RString& b) {
	return a.compare(b) != 0;
}
bool RStl::operator<(const RString& a, const RString& b) {
	return a.compare(b) == 1;
}
bool RStl::operator>(const RString& a, const RString& b) {
	return a.compare(b) == -1;
}
bool RStl::operator<=(const RString& a, const RString& b) {
	int t = a.compare(b);
	return t == 0 || t == 1;
}
bool RStl::operator>=(const RString& a, const RString& b) {
	int t = a.compare(b);
	return t == 0 || t == -1;
}
RStl::RString RStl::operator+(const RString& a, const RString& b) {
	RString r(a);

	return r.push_back(b);

}

std::wostream& RStl::operator<<(std::wostream& os, const RString& s)
{
	for (USHORT i = 0; i < s._len_; i++) {
		os << (wchar_t)s._buf_[i];
		//os << s._buf_[i];
	}
	return os;
}
std::ostream& RStl::operator<<(std::ostream& os, const RString& s)
{
	int t = s._len_ * 2;
	char* tBuf = (char*)s._buf_;
	for (USHORT i = 0; i < t; i++) {
		if (tBuf[i] == 0) continue;
		os << tBuf[i];
	}
	return os;
}
int RStl::RString::find(const RString& s) {
	if (s.size() > _len_) return -1;
	int* next = new int[_len_];
	next[0] = -1;
	int i = 0;
	int j = -1;
	while (i < _len_) {
		if (j == -1 || _buf_[i] == _buf_[j]) {
			i++;
			j++;
			next[i] = j;
		}
		else {
			j = next[j];
		}
	}
	i = 0;
	j = 0;
	while (i < _len_ && j < s.size()) {
		if (j == -1 || _buf_[i] == s[j]) {
			i++;
			j++;
		}
		else
			j = next[j];
	}
	if (j == s.size())
		return i - j;
	else
		return -1;
	
}


template<typename _Char>
RStl::RString& RStl::RString::_insert(USHORT index, USHORT size, _Char c) {
	static_assert((std::is_same<_Char, char>::value)
		|| (std::is_same<_Char, wchar_t>::value)
		|| (std::is_same<_Char, char16_t>::value)
		, "not chars");
	if (index > _len_) {
		throw std::out_of_range("out of range");
		return *this;
	}
	char16_t tc = (char16_t)c;
	int nLen = _len_ + size +1;
	if (nLen > _cap_) {
		zone t=MemoryPool::Ralloc(_len_,nLen);
		/*
		if (nLen < 64) {
			t = pool.Alloc((nLen) * 2);
		}
		else {
			if (size < _len_ / 2) {
				t = pool.Alloc((_len_ + _len_ / 2 + 1) * 2);
			}
			else if (size >= _len_ / 2 && size < _len_) {
				t = pool.Alloc(4 * _len_);
			}
			else {
				t = pool.Alloc(4 * size);
			}
		}
		*/
		char16_t* tp = (char16_t*)t.pointer;
		int c = 0;
		for (int i = 0; i < index; i++) {
			tp[c] = _buf_[i];
			c++;
		}
		for (int i = 0; i < size; i++) {
			tp[c] = tc;
			c++;
		}
		if (_buf_ != nullptr) {
			for (int i = index; i <= _len_; i++) {
				tp[c] = _buf_[i];
				c++;
			}
		}
		MemoryPool::Free(_buf_);
		_buf_ = tp;
		_len_ = nLen - 1;
		_cap_ = t.cap/2;
		_buf_[_len_] = 0;
	}
	else {
		for (int i = _len_; i >= index; i--) {
			_buf_[i + size] = _buf_[i];
		}
		for (int i = index; i < index+size; i++) {
			_buf_[i] = tc;
		}
		_len_ = nLen - 1;
	}
	return *this;
}

RStl::RString& RStl::RString::_insert(USHORT index,const RString& s) {
	if (index > _len_) {
		throw std::out_of_range("out of range");
		return *this;
	}
	USHORT size = s.size();
	int nLen = _len_ + size + 1;
	if (nLen > _cap_) {
		zone t = MemoryPool::Ralloc(_len_, nLen);
		char16_t* tp = (char16_t*)t.pointer;
		int c = 0;
		for (int i = 0; i < index; i++) {
			tp[c] = _buf_[i];
			c++;
		}
		for (int i = 0; i < size; i++) {
			tp[c] = s._buf_[i];
			c++;
		}
		if (_buf_ != nullptr) {
			for (int i = index; i <= _len_; i++) {
				tp[c] = _buf_[i];
				c++;
			}
		}
		MemoryPool::Free(_buf_);
		_buf_ = tp;
		_len_ = nLen - 1;
		_cap_ = t.cap / 2;
		_buf_[_len_] = 0;
	}
	else {
		for (int i = _len_; i >= index; i--) {
			_buf_[i + size] = _buf_[i];
		}
		for (int i = 0; i < size; i++) {
			_buf_[i+index] = s._buf_[i];
		}
		_len_ = nLen - 1;
	}
	return *this;

}

RStl::RString RStl::RString::toRString(const RStl::RString& t) {
	return RStl::RString(t);
}
RStl::RString RStl::RString::toRString(long long t) {
	RStl::RString r;
	int n = t;
	if (n == 0) {
		r.push_back('0');
	}
	while (n > 0) {
		char16_t i = n % 10 + '0';
		r.push_back(i);
		n = n / 10;
	}
	return r.reverse();
}

template<typename _Sp>
void RStl::RString::assign_str(const _Sp* s) {
	static_assert((std::is_same<_Sp, char>::value)
		|| (std::is_same<_Sp, wchar_t>::value)
		|| (std::is_same<_Sp,char16_t>::value)
		, "not pointer to chars");
	ErrorType e = MemoryPool::Free(_buf_);
	_buf_ = nullptr;
	_len_ = 0;
	if (s != nullptr) {
		USHORT len = str_len(s);
		zone r = MemoryPool::Alloc((len + 1) * 2);
		_buf_ = (char16_t*)r.pointer;
		_cap_ = r.cap/2;
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
void RStl::RString::assign_str(const char32_t* s) {
	ErrorType e = MemoryPool::Free(_buf_);
	_buf_ = nullptr;
	_len_ = 0;
	_cap_ = 0;
	if (s != nullptr) {
		USHORT len = str_len(s)*2;
		char16_t* t = (char16_t*)s;
		zone r = MemoryPool::Alloc((len + 1) * 2);
		_buf_ = (char16_t*)r.pointer;
		_cap_ = r.cap/2;
		if (_buf_ == nullptr) {
			return;
		}
		for (USHORT i = 0; i < len; i++) {
			if (t[i] != 0) {
				_buf_[i] = (char16_t)t[i];
			}
			else {
				_buf_[i] = u'\xFFFF';
			}
		}
		_buf_[len] = 0;
		_len_ = len;
	}
}
