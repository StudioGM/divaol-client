/****************************************************
 *  String.cpp
 *
 *  Created by tempbuffer on 8/12/12.
 *  Copyright 2012 tempbuffer. All rights reserved.
 *
 *  A String that combine ansi and unicode
 ****************************************************/

#include "String.h"

#include <cmath>

namespace Base
{
	const String::IndexVar String::npos = std::string::npos;

	String String::lower() const
	{
		base_wstring ret;
		for(size_t index = 0; index < mContent.size(); index++)
			if(mContent[index]>=L'A'&&mContent[index]<='z')
				ret += mContent[index]+(L'a'-L'A');
			else
				ret += mContent[index];
		return ret;
	}
	String String::upper() const
	{
		base_wstring ret;
		for(size_t index = 0; index < mContent.size(); index++)
			if(mContent[index]>=L'a'&&mContent[index]<='z')
				ret += mContent[index]+(L'A'-L'a');
			else
				ret += mContent[index];
		return ret;
	}
	String String::reverse() const
	{
		base_wstring ret;
		for(size_t index = 0; index < mContent.size(); index++)
			ret.push_back(mContent[size()-index-1]);
		return ret;
	}
	Strings String::split(Strings tokens) const
	{
		if(tokens.empty())
			tokens.push_back(L" ");

		Strings ans;
		base_wstring nowS;
		IndexVar index = 0;
		while(index<mContent.size())
		{
			bool flag = false;
			for(Strings::iterator ptr = tokens.begin(); ptr != tokens.end(); ptr++)
				if(find(*ptr,index)==index)
				{
					flag = true;
					ans.push_back(nowS);
					nowS = L"";
					break;
				}
			if(!flag)
				nowS += mContent[index];
			index++;
		}
		ans.push_back(nowS);

		return ans;
	}
	String String::strip(Strings tokens) const
	{
		if(tokens.empty())
			tokens.push_back(L" ");

		IndexVar left = 0, right = mContent.size();
		while(left<mContent.size())
		{
			bool flag = false;
			for(Strings::iterator ptr = tokens.begin(); ptr != tokens.end(); ptr++)
				if(find(*ptr,left)==left)
				{
					flag = true;
					break;
				}
			if(!flag)
				break;
			left++;
		}
		while(right>=0)
		{
			bool flag = false;
			for(Strings::iterator ptr = tokens.begin(); ptr != tokens.end(); ptr++)
				if(rfind(*ptr,size()-right)==size()-right)
				{
					flag = true;
					break;
				}
			if(!flag)
				break;
			right--;
		}
		return substr(left,right-left);
	}

	//utility funcs
	bool String::isInt() const
	{
		IndexVar index = (mContent[0]==L'-')?1:0;
		while(index<mContent.size())
		{
			if(mContent[index]<L'0'||mContent[index]>L'9')
				return false;
			index++;
		}
		return true;
	}
	bool String::isReal() const
	{
		IndexVar index = (mContent[0]==L'-')?1:0;
		bool hasDot = false;
		while(index<mContent.size())
		{
			if(mContent[index]==L'.')
			{
				if(!hasDot)
					hasDot = true;
				else
					return false;
			}
			else if(mContent[index]<L'0'||mContent[index]>L'9')
				return false;
			index++;
		}

		return true;
	}
	String String::format(const char* fmt, ...)
	{
		va_list arg;
		char buffer[1024] = {0};

		va_start(arg, fmt);
		vsprintf(buffer, fmt, arg);
		va_end(arg);

		return String(buffer);
	}
}