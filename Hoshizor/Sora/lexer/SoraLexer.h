/*
 *  lexer.h
 *  lScript_lexer
 *
 *  Created by GriffinBu on 10年1月7日.
 *  Copyright 2010 RobertBu(Project L). All rights reserved.
 *
 */
#ifndef _SORA_LEXER_H_
#define _SORA_LEXER_H_

#include <cstdio>
#include <cstring>

#include <string>
#include <vector>

#define	SORA_LEXER_VER_MAJOR  1
#define	SORA_LEXER_VER_MINOR  3

#ifndef WIN32
#define stricmp strcasecmp
#endif

#include "SoraPlatform.h"
#include <map>


namespace sora {
    
    typedef int Token;
    
    /* Token Types */
    typedef enum {
        TokenInvalidInput       = -1, 	// invalid input
        TokenEndOfStream        = 0,	// end of a file
        TokenInt                = 1,	// int number
        TokenFloat              = 2,	// float number
        TokenIdent              = 3,	// a ident
        TokenString             = 4,	// a string
        TokenOperator			= 15,	// operators, see lex_operators
        
        // delimeters
        TokenDelimComma				= 16,	// ,
        TokenDelimOpenParen			= 17,	// (
        TokenDelimCloseParen		= 18,	// )
        TokenDelimOpenBrace			= 19,	// [
        TokenDelimCloseBrace		= 20,	// ]
        TokenDelimOpenCurlyBrace	= 21,	// {
        TokenDelimCloseCurlyBrace	= 22,	// }
        TokenDelimSemicolon         = 23,	// ;
        TokenDelimPeriod			= 24,	// .
        TokenDelimColon				= 25,	// :
    } SoraLexerTokenType;
    
    // token subtypes
    // int token special, hex & binary & octal
    typedef enum {
        TokenSubTypeNone			= 100,
        TokenSubtypeHexInt          = 101,
        TokenSubtypeOctalInt		= 102,
    } SoraLexerTokenSubtype;
    
    /* Error Codes */
    typedef enum {
        LexNoError          = 1,
        LexFileIOError      = -1,
        LexInvalidInput     = -2
    } SoraLexerErrorCode;
    
    class SoraLexemeState {
    public:
        int	iCurrOp;
        
        size_t	iCurrLexemeStart;
        size_t	iCurrLexemeIndex;
        size_t	iCurrLine;
        
        Token tkTokenSubType;
        
        std::string strCurrLexeme;
        
        SoraLexemeState(): iCurrOp(-1), iCurrLine(-1), iCurrLexemeIndex(0), iCurrLexemeStart(0), tkTokenSubType(TokenSubTypeNone) {}
    };
    
    class SoraLexerOperators {
    public:
        typedef std::map<std::string, int> OperatorMap;
        typedef std::map<std::string, int>::iterator iterator;
        OperatorMap mOperators;
        
        void add(const std::string& str, int id) {
            this->mOperators[str] = id;
        }
        int check(const std::string& str) {
            iterator it = this->mOperators.begin();
            if((it = this->mOperators.find(str)) != this->mOperators.end()) {
                return it->second;
            }
            return -1;
        }
        int checkChar(char cChar) {
            iterator it = mOperators.begin();
            while(it != this->mOperators.end()) {
                if(it->first[0] == cChar) return 1;
                ++it;
            }
            return 0;
        }
    };
    
    class SORA_API SoraLexer {
    public:
        SoraLexer();
        ~SoraLexer();
        /*
         reset the lexer to default state
         */
        void reset();
        
        /*
         load a file to lex
         @param, path of the file
         @retval, if succeed, return true
         */
        bool loadFile(const char*);
        /*
         load a file from memory
         @param ptr, the pointer to the memory block that contains the file data
         @param size, the size of the memory block
         @retval, if succeed, return true
         */
        bool loadFileMem(void* ptr, unsigned long size);
        
        /*
         get next token, return TOKEN_TYPE_INVALID_INPUT if there a invalid input met
         @retval, next token
         */
        Token getNextToken();
        /*
         get current token's subtype, see TOKEN_SUBTPYE_*
         if getNextToken() = TOKEN_TYPE_INT, then sub type can be 
         TOKEN_SUBTYPE_INT_OCTAL	- Octal Int 
         TOKEN_SUBTYPE_INT_HEX		- Hex Int
         TOKEN_SUBTYPE_NULL			- Demical Int
         @retval, current token's subtype
         */
        Token getCurrTokenSubtype() const;
        /*
         to next token, when LEXER_INVALID_INPUT error happened, use this function to skip to next token, 
         otherwise the function behaves the same as getNextToken()
         @retval, next token
         */
        Token toNextToken();
        
        /*
         get the lexeme of current token
         @retval, pointer to the lexeme
         */
        const char* getCurrLexeme() const;
        
        /*
         convert current lexeme into specified types
         */
        int   asInt() const;
        bool  asBool() const;
        float asFloat() const;
        
        /*
         get current operator, only available when getNextToken() == TOKEN_TYPE_OP
         @retval, current operator
         */
        int	getCurrOp() const;
        
        // information APIs
        /*
         get the line count of the source file
         @retval, line count of the source file
         */
        size_t getSourceLineCount() const;
        /*
         get the size of the source file
         @retval, the size of the source file
         */
        size_t getSourceSize() const;
        /*
         get current char index, use when LEX_INVALID_INPUT happens to get the invalid input char index
         @retval, current char index in the line
         */
        size_t getCurrCharIndex() const;
        /*
         get current line index, use when LEX_INVALID_INPUT happens to get the current line number of the source
         @retval, current line index in the source file
         */
        size_t getCurrLineIndex() const;
        /*
         get current line of the source file
         @retval, current line of the source file
         */
        const char*	getCurrLine() const;
        /*
         get invalid char, use when LEX_INVALID_INPUT happens to get the invalid char, returns 0 if there's no LEX_INVALID_INPUT error happened
         @retval, invalid char
         */
        char getCurrInvalidChar() const;
        
        /*
         rewind the token stream to previous state
         @retval, succeed or not
         */
        int	rewindTokenStream();
        
        /*
         add a operator ident to parser
         @param op, the string of the operator, eg "++"
         @param id, the id of the operator, see getNextToken() and toNextToken();
         */
        void addOperator(const std::string& op, int id);
    	
    private:
        SoraLexer(const SoraLexer&);
        SoraLexer& operator=(const SoraLexer&);
        
        inline char	getNextChar();
        inline void rewindChar();
        inline char getLookAheadChar();
        Token errorInvalidChar(char cCurrChar);
		
        char strInvalidChar;
        unsigned long lSourceSize;
        
        typedef std::vector<std::string> SourceCont;
        SourceCont contSource;
        
        SoraLexemeState currState;
        typedef std::vector<SoraLexemeState> LexemeStateCont;
        LexemeStateCont contLexerState;
        
        SoraLexerOperators operators;
        
        short usErrorCode;
    };

}

#endif