/*
 *  DivaArgument.h
 *
 *  Created by Hyf042 on 1/13/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_ARGUMENT
#define DIVA_ARGUMENT

#include <map>
#include "SoraAny.h"

namespace divacore
{
	using namespace sora;

	typedef std::map<std::string, sora::SoraAny> ARGUMENTS;

	/*
	 * some support functions to deal with ARGUMENT
	 */
	class Argument
	{
	public:
		/*
		argument utilities
		*/
		static bool isArg(const std::string &name, ARGUMENTS &arg) {return arg.find(name)!=arg.end();}
		static void makeSure(const std::string &name, ARGUMENTS &arg) {if(!isArg(name,arg)) DIVA_EXCEPTION_MODULE("argument "+name+" does not exist","EventManager");}
		static void addDefaultArg(const std::string &varname, sora::SoraAny any, ARGUMENTS &arg)
		{ 
			if(arg.find(varname)==arg.end())
				arg[varname] = any;
		}
		static void addDefaultArg(const std::string &varname, int any, ARGUMENTS &arg) { addDefaultArg(varname,sora::SoraAny(any),arg);}
		static void addDefaultArg(const std::string &varname, const char *any, ARGUMENTS &arg) { addDefaultArg(varname,std::string(any),arg);}

		static inline int asInt(const std::string &name, ARGUMENTS &arg)
		{
			makeSure(name,arg);
			return AnyCast<int>(arg[name]);
		}
		static inline double asDouble(const std::string &name, ARGUMENTS &arg)
		{
			makeSure(name,arg);
			SoraAny &any = arg[name];
			if(IsAnyFloat(any))
				return AnyCast<float>(any);
			if(IsAnyInt(any))
				return AnyCast<int>(any);
			return AnyCast<double>(any);
		}
		static inline float asFloat(const std::string &name, ARGUMENTS &arg)
		{
			makeSure(name,arg);
			SoraAny &any = arg[name];
			if(IsAnyDouble(any))
				return AnyCast<double>(any);
			if(IsAnyInt(any))
				return AnyCast<int>(any);
			return AnyCast<float>(any);
		}
		static inline std::string asString(const std::string &name, ARGUMENTS &arg)
		{
			makeSure(name,arg);
			return AnyCast<std::string>(arg[name]);
		}
		static inline bool asBool(const std::string &name, ARGUMENTS &arg)
		{
			makeSure(name,arg);
			return AnyCast<bool>(arg[name]);
		}
	};
}

#endif