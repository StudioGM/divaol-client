#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS

#ifdef WIN32
#include <windows.h>
#endif

#include <stdlib.h>
#include <iostream>
#include <string>

#include "CSHA1Test.h"

#include "CSHA1/SHA1.h" // CSHA1 class

using std::endl;

#ifdef _UNICODE
typedef std::wstring tstring;
#define tcout std::wcout
#define tcin std::wcin
#else
typedef std::string tstring;
#define tcout std::cout
#define tcin std::cin
#endif

#pragma warning(push)
// Disable compiler warning 'Unreferenced formal parameter'
#pragma warning(disable: 4100)
// Disable compiler warning 'Conditional expression is constant'
#pragma warning(disable: 4127)

int _tmain(int argc, TCHAR* argv[])
{
	tcout << _T("[ Demo application for the CSHA1 class ]") << endl;

	while(true)
	{
		tcout << endl << _T("1) Hash a string") << endl;
		tcout << _T("2) Hash a file") << endl;
		tcout << _T("3) Exit") << endl << endl;
		tcout << _T("Enter [1], [2] or [3]: ");

		tstring strIn;
		std::getline(tcin, strIn);
		if(strIn.size() == 0) continue;

		const TCHAR ch = strIn[0];

		if(ch == _T('3')) break;
		else if(ch == _T('1')) HashString();
		else if(ch == _T('2')) HashFile();
		else tcout << endl << _T("Input error. Enter 1, 2 or 3 and [Enter].") << endl;
	}

	return 0;
}

void HashString()
{
	tcout << endl << _T("Enter the string to hash:") << endl;

	tstring str;
	std::getline(tcin, str);
	tcout << endl;

	CSHA1 sha1;
	tstring strReport;

#ifdef _UNICODE
	const size_t uAnsiLen = wcstombs(NULL, str.c_str(), 0) + 1;
	char* pszAnsi = new char[uAnsiLen + 1];
	wcstombs(pszAnsi, str.c_str(), uAnsiLen);

	sha1.Update((UINT_8*)&pszAnsi[0], strlen(&pszAnsi[0]));
	sha1.Final();
	sha1.ReportHashStl(strReport, CSHA1::REPORT_HEX_SHORT);
	tcout << _T("Hash of the ANSI representation of the string:") << endl;
	tcout << strReport << endl << endl;

	delete[] pszAnsi;
	sha1.Reset();

	sha1.Update((UINT_8*)str.c_str(), str.size() * sizeof(TCHAR));
	sha1.Final();
	sha1.ReportHashStl(strReport, CSHA1::REPORT_HEX_SHORT);
	tcout << _T("Hash of the Unicode representation of the string:") << endl;
	tcout << strReport << endl;
#else
	sha1.Update((UINT_8*)str.c_str(), str.size() * sizeof(TCHAR));
	sha1.Final();
	sha1.ReportHashStl(strReport, CSHA1::REPORT_HEX_SHORT);
	tcout << _T("String hashed to:") << endl;
	tcout << strReport << endl;
#endif
}

void HashFile()
{
	tcout << endl << _T("File path:") << endl;

	tstring strPath;
	std::getline(tcin, strPath);

	CSHA1 sha1;
	const bool bSuccess = sha1.HashFile(strPath.c_str());
	sha1.Final();

	tstring strReport;
	sha1.ReportHashStl(strReport, CSHA1::REPORT_HEX_SHORT);

	if(bSuccess)
	{
		tcout << endl << _T("File contents hashed to:") << endl;
		tcout << strReport << endl;
	}
	else
		tcout << endl << _T("An error occured (does the file really exist?).") << endl;
}

#pragma warning(pop)
