#pragma once

#include <iostream>
#include <string>

using namespace std;

class Input
{
public:
	Input() = default;

	bool isKey(wchar_t) const;
	wchar_t getKey() const;
	wstring getLine() const;
	size_t getNum() const;

	bool isValidRange(size_t, size_t, size_t) const;
	bool isValidName(wstring) const;
};
