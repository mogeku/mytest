#ifndef STRING_CONVERTION_HPP
#define STRING_CONVERTION_HPP

#ifdef WIN32

//char* UnicodeToUtf8(const wchar_t* unicode);
//wchar_t* UTF8ToWsz(const char* szUtf8);

#endif // WIN32

std::string Utf16ToUtf8(const std::wstring& u16str);
std::wstring Utf8ToUtf16(const std::string& u8str);

//char *ConvertWCharToUTF8(const wchar_t *wstr);

#endif