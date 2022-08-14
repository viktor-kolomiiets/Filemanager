#include "input.h"

//bool Input::isKey(wchar_t k) const
//{
//    return k == this->getKey();
//}

//wchar_t Input::getKey() const
//{
//    return towlower(_getwch_nolock());
//}

wstring Input::getLine() const
{
    wstring buffer{ L"" };
    getline(wcin, buffer, L'\n');
    return buffer;
}

size_t Input::getNum() const
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

bool Input::isValidRange(size_t val, size_t min, size_t max) const
{
    if (val < min || val > max)
        return false;

    return true;
}

bool Input::isValidName(wstring str) const
{
    size_t pos = str.find_first_of(L"\\/:*?\'\"<>|");
    if (pos != wstring::npos)
        return true;

    return false;
}
