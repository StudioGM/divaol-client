/*
 *  DivaException.h
 *
 *  Created by Hyf042 on 1/13/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_EXCEPTION
#define DIVA_EXCEPTION

#include <string>

namespace divacore
{
	/*
	Exception class
	*/
	class Exception
	{
		int linenum;
		std::string content;
		std::string moduleName;
	public:
		Exception(const std::string &content = "error", const std::string &moduleName = "none", int linenum = 0):content(content),moduleName(moduleName),linenum(linenum){}
		void setContent(const std::string &content) {this->content=content;}
		void setModuleName(const std::string &moduleName) {this->moduleName=moduleName;}
		void setLinenum(int linenum) {this->linenum = linenum;}
		const std::string& getContent() {return content;}
		const std::string& getModuleName() {return moduleName;}
		int getLinenum() {return linenum;}
	};

//some utility defines
#define DIVA_EXCEPTION_MESSAGE(error) throw divacore::Exception(error)
#define DIVA_EXCEPTION_MODULE(error,module) throw divacore::Exception(error,module)
#define DIVA_EXCEPTION_LINE_NUMBER(error,module,line) throw divacore::Exception(error,module,line)
}

#endif