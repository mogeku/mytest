#include <stdlib.h>
#include <wchar.h>
#include <string.h>

#include <string>
#include <codecvt>
#include <locale>

#include "StringConvertion.hpp"

/*
#ifdef WIN32
#include <Windows.h>

char* UnicodeToUtf8(const wchar_t* unicode)
{
	int len;
	len = WideCharToMultiByte(CP_UTF8, 0, unicode, -1, NULL, 0, NULL, NULL);
	char *szUtf8 = new char[len + 1];
	memset(szUtf8, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, unicode, -1, szUtf8, len, NULL, NULL);
	return szUtf8;
}

wchar_t* UTF8ToWsz(const char* szUtf8)
{
	//预转换，得到所需空间的大小;
	int wcsLen = ::MultiByteToWideChar(CP_UTF8, NULL, szUtf8, strlen(szUtf8), NULL, 0);

	//分配空间要给'\0'留个空间，MultiByteToWideChar不会给'\0'空间
	wchar_t* wszString = new wchar_t[wcsLen + 1];

	//转换
	::MultiByteToWideChar(CP_UTF8, NULL, szUtf8, strlen(szUtf8), wszString, wcsLen);

	//最后加上'\0'
	wszString[wcsLen] = '\0';

	return wszString;
}

#endif // WIN32
*/

std::string Utf16ToUtf8(const std::wstring& u16str)
{
	if (u16str.empty())
	{
		return std::string();
	}

	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
	return conv.to_bytes(u16str);
}

std::wstring Utf8ToUtf16(const std::string& u8str)
{
	if (u8str.empty())
	{
		return std::wstring();
	}

	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> conv;
	return conv.from_bytes(u8str);
}

/*
char *ConvertWCharToUTF8(const wchar_t *wstr)
{
	std::wstring utf16_string = wstr;

	if (utf16_string.size() == 0)
		return (char*)wstr;

	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;

	std::string utf8_string = conv.to_bytes(utf16_string);

	char *str_ptr = (char*)wstr;
	memcpy(str_ptr, utf8_string.c_str(), utf8_string.size() + 1);
	str_ptr[utf8_string.size()] = 0;

	return str_ptr;
}*/
