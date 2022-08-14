#pragma once

#include <iostream>
#include <string>

using namespace std;

class Input
{
public:
	Input() = default;

	bool isKey(wchar_t k) const { return k == this->getKey(); }
	wchar_t getKey() const { return towlower(_getwch_nolock()); }
	wstring getLine() const;
	size_t getNum() const;

	bool isValidRange(size_t, size_t, size_t) const;
	bool isValidName(wstring) const;
};
