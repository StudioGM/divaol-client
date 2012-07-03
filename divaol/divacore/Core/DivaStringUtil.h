/*
 *  DivaStringUtil.h
 *
 *  Created by Hyf042 on 1/28/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_STRING_UTIL
#define DIVA_STRING_UTIL

#include <string>

namespace divacore
{
	std::string iToS(int n);
	int sToI(const std::string &number);
	std::string GBKToUTF8(const std::string& strGBK);
	std::string format(const char*,...);
}

#endif