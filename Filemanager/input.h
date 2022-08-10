#pragma once

//------------------------------INPUT--------------------------------------------------------------

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

inline bool Input::isKey(wchar_t k) const
{
	return k == this->getKey();
}

inline wchar_t Input::getKey() const
{
	return towlower(_getwch_nolock());
}

inline wstring Input::getLine() const
{
	wstring buffer{ L"" };
	getline(wcin, buffer, L'\n');
	return buffer;
}

inline size_t Input::getNum() const
{
	size_t buffer{ 0ull };
	wcin >> buffer;
	if (wcin.fail())
	{
		wcin.clear();
	}
	wcin.ignore(32767, L'\n');
	return buffer;
}

inline bool Input::isValidRange(size_t val, size_t min, size_t max) const
{
	if (val < min || val > max)
		return false;

	return true;
}

inline bool Input::isValidName(wstring str) const
{
	size_t pos = str.find_first_of(L"\\/:*?\'\"<>|");
	if (pos != wstring::npos)
		return true;

	return false;
}
