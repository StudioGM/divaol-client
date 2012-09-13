#include "ui/Config/DivaUIConfig.h"
#include <fstream>
#include <sstream>

namespace diva
{
	WJson::Value config;

	void initialize_config(const std::wstring& filename)
	{
		//std::ifstream inf(filename);
		WJson::Reader reader;
		reader.parse(ReadJsonFile(filename), config);
	}

	std::wstring ReadJsonFile(const std::wstring& filename)
	{
		std::wstring jsonStrToParse;

		FILE* readFile;
		if(_wfopen_s(&readFile, filename.c_str(),L"rt, ccs=UTF-8")!=0)
			throw "fuck!";
			//return LOCALIZATION->getLocalStr(L"ReadFile_OpenFileError", path.c_str());

		wchar_t buffer[1000];
		while(fgetws(buffer,sizeof(buffer),readFile))
			jsonStrToParse += std::wstring(buffer);

		fclose(readFile);

		return jsonStrToParse;
	}



	
}

namespace gcn
{
	//Utility
	//int to Wstring()
	std::wstring iToWS(int n) 
	{
		std::wostringstream strStream;
		strStream<<n;
		return strStream.str();
	}



	//Float to WString
	std::wstring fTows(float f,int bit)
	{
		std::wstring ret;
		if(f<0)
		{
			ret += L"-";
			f=-f;
		}

		ret += iToWS(int(f));

		if(bit>0)
		{
			ret += L".";

			float tail = f - int(f);
			for(int i=0;i<bit;i++)
			{
				tail*=10;
				int thisBit = tail;
				ret += (wchar_t)(thisBit + '0');
				tail-=thisBit;
			}
		}
		return ret;
	}

	//Double to Wstring
	std::wstring dTows(double d,int bit)
	{
		std::wstring ret;
		if(d<0)
		{
			ret += L"-";
			d=-d;
		}

		ret += iToWS(int(d));

		if(bit>0)
		{
			ret += L".";

			double tail = d - int(d);
			for(int i=0;i<bit;i++)
			{
				tail*=10;
				int thisBit = tail;
				ret += (wchar_t)(thisBit + '0');
				tail-=thisBit;
			}
		}
		return ret;
	}

	//WString to float
	float wsTof(std::wstring &s)
	{
		//Check validate
		int dotPosCount=s.length();
		bool negative=0;
		for(int i=0;i<s.length();i++)
		{
			if(s[i]==(wchar_t)'.')
			{
				if(dotPosCount!=s.length()) //too many dots
					return 0;
				else
					dotPosCount = i;
			}
			else if((s[i]==(wchar_t)'-'&&!negative))
				negative=true;
			else if(s[i]<(wchar_t)'0'||s[i]>(wchar_t)'9')
				return 0;

		}

		float leftPart = 0;
		for(int i=negative?1:0;i<dotPosCount;i++)
		{
			leftPart*=10;
			leftPart+=s[i]-(wchar_t)'0';
		}

		float rightPart=0;
		for(int i=s.length()-1;i>dotPosCount;i--)
		{
			rightPart+=s[i]-(wchar_t)'0';
			rightPart/=10;
		}

		return negative?(-((leftPart+rightPart))):(leftPart+rightPart);
	}

	//WString to double
	double wsTod(std::wstring &s)
	{
		double ret;
		swscanf(s.c_str(),L"%lf",&ret);
		return ret;

		//Check validate
		int dotPosCount=s.length();
		bool negative=0;
		for(int i=0;i<s.length();i++)
		{
			if(s[i]==(wchar_t)'.')
			{
				if(dotPosCount!=s.length()) //too many dots
					return 0;
				else
					dotPosCount = i;
			}
			else if((s[i]==(wchar_t)'-'&&!negative))
				negative=true;
			else if(s[i]<(wchar_t)'0'||s[i]>(wchar_t)'9')
				return 0;

		}

		double leftPart = 0;
		for(int i=negative?1:0;i<dotPosCount;i++)
		{
			leftPart*=10;
			leftPart+=s[i]-(wchar_t)'0';
		}

		int rightPartI=0;int toDevide=10;
		for(int i=s.length()-1;i>dotPosCount;i--)
		{
			rightPartI+=s[i]-(wchar_t)'0';
			rightPartI*=10;
			toDevide*=10;
		}

		double rightPart = (double)rightPartI/(double)toDevide;

		return negative?(-((double)((int)(leftPart*1000)+(int)(rightPart*1000))/1000)):((double)((int)(leftPart*1000)+(int)(rightPart*1000))/1000);
	}

	//round to integer
	double round(double r) {
		return (r > 0.0) ? floor(r + 0.5) : ceil(r - 0.5);
	}

	//Wstring upper to lower
	void tolower(std::wstring &str)
	{
		for(int i=0;i<str.length();i++)
			if(str[i]>=L'A'&&str[i]<=L'Z')
				str[i] = str[i] + (L'a'-L'A');
	}

	std::wstring secondToTimeWstr(float second)
	{
		int secondI = second;
		int hour = secondI / 3600;
		secondI %= 3600;
		int minute = secondI / 60;
		secondI %= 60;

		std::wstring hourStr = iToWS(hour),minuteStr = iToWS(minute),secondStr = iToWS(secondI);
		std::wstring ret = L"";
		if(hour>0)
		{
			if(hourStr.length()==1)
				hourStr = L'0' + hourStr;
			ret = hourStr + L':';
		}
		if(minuteStr.length()==1)
			minuteStr = L'0'+minuteStr;
		ret = ret+minuteStr + L':';
		if(secondStr.length()==1)
			secondStr = L'0'+secondStr;
		ret = ret+secondStr;

		return ret;
	}

	std::string to_utf8(const wchar_t* buffer, int len)
	{
		int nChars = ::WideCharToMultiByte(
			CP_UTF8,
			0,
			buffer,
			len,
			NULL,
			0,
			NULL,
			NULL);
		if (nChars == 0) return "";

		std::string newbuffer;
		newbuffer.resize(nChars) ;
		::WideCharToMultiByte(
			CP_UTF8,
			0,
			buffer,
			len,
			const_cast< char* >(newbuffer.c_str()),
			nChars,
			NULL,
			NULL); 

		return newbuffer;
	}

	std::string to_utf8(const std::wstring& str)
	{
		return to_utf8(str.c_str(), (int)str.size());
	}
}