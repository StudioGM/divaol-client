#include "DivaMapEncryption.h"
#include <ctime>
#include <string>

namespace divamap
{

#define EncryptOffsetMin 80
#define EncryptOffsetMax 120

	std::wstring& decrypt(std::wstring &str)
	{
		if(str.length()<2) return str;

		wchar_t thisRandSeed = str[0];
		srand((unsigned int)thisRandSeed);

		int ModOffset = (int)str[1];

		for (int i=2;i<str.length();i++)
		{
			int toDelta = rand()%ModOffset;
			if(str[i]!=WEOF-toDelta)
				str[i] -= toDelta;
		}

		return (str=str.substr(2,str.length()-2));
	}

	std::wstring& encrypt(std::wstring &str)
	{
		wchar_t thisRandSeed = (wchar_t)time(0)%WEOF;
		srand(thisRandSeed);
		wchar_t ModOffset = (wchar_t)((int)rand()%(EncryptOffsetMax-EncryptOffsetMin)+EncryptOffsetMin);
		srand(thisRandSeed);

		for(int i=0;i<str.length();i++)
		{
			int toDelta = rand()%(int)ModOffset;
			if(str[i]!=WEOF-toDelta)
				str[i] += toDelta;
		}

		wchar_t toAdd[2];
		toAdd[0]=ModOffset;
		toAdd[1]=0;

		str=toAdd + str;

		toAdd[0]=thisRandSeed;
		return (str=toAdd + str);
	}


}
