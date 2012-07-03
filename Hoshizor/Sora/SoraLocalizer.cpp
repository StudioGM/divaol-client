/*
 *  SoraLocalizer.cpp
 *  Sora
 *
 *  Created by GriffinBu on 1/14/11.
 *  Copyright 2011 Griffin Bu(Project L). All rights reserved.
 *
 */

#include "SoraLocalizer.h"
#include "SoraGlobalMessageRouter.h"
#include "function/SoraBind.h"

#include "SoraStringConv.h"
#include "SoraCore.h"
#include "lexer/SoraLexer.h"
#include "message/SoraMessageEvent.h"

namespace sora {

	SoraLocalizer* SoraLocalizer::mInstance = NULL;

	SoraLocalizer* SoraLocalizer::Instance() {
		if(!mInstance)
			mInstance = new SoraLocalizer;
		return mInstance;
	}

	void SoraLocalizer::Destroy() {
		if(mInstance) {
			delete mInstance;
			mInstance = NULL;
		}
	}
    
    SoraLocalizer::SoraLocalizer() {
        AcceptMessage("setlocale", sora::Bind(this, &SoraLocalizer::onMessage));
    }
	
	SoraLocalizer::~SoraLocalizer() {
	}
	
    void SoraLocalizer::onMessage(SoraMessageEvent* message) {
        try {
            SoraString data = message->getData<SoraString>();
            setCurrentLocale(data);
            
            message->consume();
        } catch(...) {
            
        }
    }
    
	bool SoraLocalizer::readToken(SoraLexer* lexer, int tokenExpected) {
		return (lexer->getNextToken() == tokenExpected);
	}
	
	SoraString SoraLocalizer::readLocaleIdent(SoraLexer* confLexer) {
		readToken(confLexer, TokenIdent);
		if(strcmp(confLexer->getCurrLexeme(), "locale") == 0) {
			readToken(confLexer, TokenOperator);
			readToken(confLexer, TokenIdent);

			return confLexer->getCurrLexeme();
		}
		return "\0";
	}
	
	bool SoraLocalizer::readLocaleString(SoraLexer* confLexer, LocaleStringMap& strMap) {
		SoraString strIdent = confLexer->getCurrLexeme();
		if(!readToken(confLexer, TokenOperator)) return false;
		if(!readToken(confLexer, TokenString)) return false;
		strMap[strIdent] = s2ws(confLexer->getCurrLexeme());
		return true;
	}
	
	bool SoraLocalizer::readLocaleResource(SoraLexer* confLexer, LocaleStringMap& strMap) {
		SoraString strIdent = confLexer->getCurrLexeme();
		if(!readToken(confLexer, TokenOperator)) return false;
		if(!readToken(confLexer, TokenString)) return false;
		strMap[strIdent] = s2ws(confLexer->getCurrLexeme());
		return true;
	}
	
	void SoraLocalizer::reportError(SoraLexer* lexer) {
		SoraString strErrorMssg = ("Error loading locale file\nCurrent Line=");
        SoraCore::Ptr->messageBox(strErrorMssg+lexer->getCurrLine()+"\nCurrent Char="+lexer->getCurrInvalidChar(),
						 "Error",
						 MB_OK | MB_ICONERROR);
	}
    
    SoraString SoraLocalizer::getCurrentLocale() const { 
        return currentLocale; 
    }
	
	bool SoraLocalizer::addLocaleConf(const StringType& confPath) {
		SoraLexer* confLexer = new SoraLexer;
		confLexer->addOperator("@", 1);
		confLexer->addOperator("=", 2);
		
		if(confLexer->loadFile(confPath.c_str())) {
			SoraString localeIdent;
			LocaleStringMap localeStrMap;
			
			Token token = confLexer->getNextToken();
			while(token != TokenEndOfStream && token != TokenInvalidInput) {
				if(token == TokenOperator) {
					if(confLexer->getCurrOp() == 1) {
						localeIdent = readLocaleIdent(confLexer);
					}
				} else if(token == TokenIdent) {
					if(!readLocaleString(confLexer, localeStrMap)) {
						reportError(confLexer);
						return false;
					}
					
				} else if(token == TokenString) {
					if(readLocaleResource(confLexer, localeStrMap)) {
						reportError(confLexer);
						return false;
					}
				}
				token = confLexer->getNextToken();
			}
			
			delete confLexer;
			
			if(localeIdent.size() != 0) {
				localeConfs[localeIdent].insert(localeStrMap.begin(), localeStrMap.end());
				if(currentLocale.size() == 0) {
					currentLocale = localeIdent;
					currentLocaleMap = localeConfs.find(localeIdent);
					
					return true;
				}
			} else if(currentLocale.size() != 0) {
				localeConfs[currentLocale].insert(localeStrMap.begin(), localeStrMap.end());
				
				return true;
			} else
				THROW_SORA_EXCEPTION(NotFoundException, "no locale ident read");
		} else {
			sora::SoraCore::Instance()->messageBox("Cannot find locale file: "+confPath, "Error", MB_OK | MB_ICONERROR);
			return false;
		}
		return false;
	}
	
	StringType SoraLocalizer::getStr(const StringType& ident) {
		LocaleStringMap::iterator itStr = currentLocaleMap->second.find(ident);
		if(itStr != currentLocaleMap->second.end())
			return itStr->second;
		return StringType();
	}

	void SoraLocalizer::setCurrentLocale(const SoraString& localeIdent) {
		LocaleConfMap::iterator itConf = localeConfs.find(localeIdent);
		if(itConf != localeConfs.end()) {
			currentLocale = localeIdent;
			currentLocaleMap = itConf;
		}
	}
		
	StringType SoraLocalizer::localizeResourceName(const StringType& resourceName) {
        std::string rfile = resourceName;
		size_t dotPos = rfile.rfind('.');
		if(dotPos != std::string::npos) {
			rfile.insert(dotPos, "@"+currentLocale);
		}
		return rfile;
	}


} // namespace sora