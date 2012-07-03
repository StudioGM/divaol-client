/*
 *  lexer.cpp
 *  lScript_lexer
 *
 *  Created by GriffinBu on 10年1月7日.
 *  Copyright 2010 RobertBu(Project L). All rights reserved.
 *
 */

#include "SoraLexer.h"

#define LEX_STATE_START					0
#define LEX_STATE_INT					1
#define LEX_STATE_FLOAT					2
#define LEX_STATE_IDENT					5

#define LEX_STATE_OP					6
#define LEX_STATE_DELIM					7

#define LEX_STATE_STRING				8
#define LEX_STATE_STRING_ESCAPE			9
#define LEX_STATE_STRING_CLOSE_QUOTE	10

#if !defined(__APPLE__)
#include <malloc.h>
#endif

#include "SoraCore.h"
#include "SoraResourceFile.h"
#include "SoraString.h"

namespace sora {
    
    
    //determines if a character is numeric
    inline bool isCharNumericLex(char cChar) {
        if(cChar >= '0' &&
           cChar <= '9')
            return true;
        return false;
    }
    
    inline bool isCharNumericSpecial(char cChar) {
        if(
           // hex
           cChar == 'x'
           )
            return true;
        return false;
    }
    
    //determines if a character is whitespace
    inline bool isCharWhitespaceLex(char cChar) {
        if(cChar == ' ' ||
           cChar == '\t' ||
           cChar == '\n' ||
           cChar == '\r')
            return true;
        return false;
    }
    
    //determines if a character is ASCII character
    inline bool isCharASCIIChar(char cChar) {
        // ascii head 0
        return ((cChar&0x80) == 0);
    }
    
    //determines if a charatcter could be part of a valid identifier
    inline bool isCharIdentLex(char cChar) {
        if((cChar >= '0' && cChar <= '9') ||
           (cChar >= 'A' && cChar <= 'Z') ||
           (cChar >= 'a' && cChar <= 'z') ||
           (cChar == '_') ||
           (!isCharASCIIChar(cChar)))
            return true;
        return false;
    }
    
    //determines if a character is a delimiter
    inline bool isCharDelimiterLex(char cChar) {
        if(cChar == ',' ||
           cChar == '(' ||
           cChar == ')' ||
           cChar == '[' ||
           cChar == ']' ||
           cChar == ';' ||
           cChar == '{' ||
           cChar == '}' ||
           cChar == '.' ||
           cChar == ':')
            return true;
        return false;
    }
    
    
    SoraLexer::SoraLexer() {
        reset();
    }
    
    SoraLexer::~SoraLexer() {
        
    }
    
    void SoraLexer::reset() {
        // reset lexer
        currState.iCurrLexemeIndex = currState.iCurrOp = currState.iCurrLine = 0;
        currState.tkTokenSubType = TokenSubTypeNone;
        currState.strCurrLexeme.clear();
        
        usErrorCode = LexNoError;
        strInvalidChar = 0;
        
        contSource.clear();
        contLexerState.clear();
    }
    
    bool SoraLexer::loadFile(const char* pstrFileName) {
        SoraResourceFile data(pstrFileName);
        
        return loadFileMem(data, data.size());
    }
    
    bool SoraLexer::loadFileMem(void* ptr, unsigned long size) {
        if(ptr && size != 0) {
            const char* pstrSource = (const char*)ptr;
            unsigned long c = 0;
            
            char cCurrChar;
            std::string sCurrLine;
            while(c < size) {
                cCurrChar = pstrSource[c];
                
                // ASCII character
                if(isCharASCIIChar(cCurrChar)) {
                    if(cCurrChar == '\\')
                        cCurrChar = '/';
                    
                    //*nix LF, windows LF+CR
                    //ignore CR
                    if(cCurrChar == '\r') {
                        ++c;
                        continue;
                    }
                    else if(cCurrChar == '\n') {
                        contSource.push_back(sCurrLine);
                        sCurrLine.clear();
                    } else {
                        sCurrLine += cCurrChar;
                    }
                    ++c;
                } else {
                    // just add to source
                    sCurrLine += cCurrChar;
                    ++c;
                }
            }
            contSource.push_back(sCurrLine);
            
            currState.iCurrLexemeIndex = currState.iCurrOp = currState.iCurrLine = 0;
            currState.tkTokenSubType = TokenSubTypeNone;
            currState.strCurrLexeme.clear();
            
            usErrorCode = LexNoError;
            strInvalidChar = 0;
            
            return true;
        }
        usErrorCode = LexFileIOError;
        return false;
    }
    
    Token SoraLexer::errorInvalidChar(char cCurrChar) {
        strInvalidChar = cCurrChar;
        usErrorCode = LexInvalidInput;
        return TokenInvalidInput;
    }
    
    char SoraLexer::getNextChar() {
        if(currState.iCurrLexemeIndex < contSource[currState.iCurrLine].size()) {
            return contSource[currState.iCurrLine][currState.iCurrLexemeIndex++];
        } else {
            //	printf("line %d/%d, char %d/%d, %s :", iCurrLine, contSource.size(), iCurrLexemeIndex, contSource[iCurrLine].size(), contSource[iCurrLine].c_str());
            currState.iCurrLexemeIndex = 0;
            ++currState.iCurrLine;
            return '\n';
        }
    }
    
    void SoraLexer::rewindChar() {
        if(currState.iCurrLexemeIndex > 0) {
            --currState.iCurrLexemeIndex;
        }
    }
    
    char SoraLexer::getLookAheadChar() {
        if(currState.iCurrLexemeIndex < contSource[currState.iCurrLine].size()-1) {
            return contSource[currState.iCurrLine][currState.iCurrLexemeIndex+1];
        } else {
            return 0;
        }
    }
    
    Token SoraLexer::toNextToken() {
        if(usErrorCode == LexNoError) return getNextToken();
        else if(usErrorCode == LexInvalidInput) {
            rewindChar();
            char cCurrChar = getNextChar();
            while(!(isCharWhitespaceLex(cCurrChar) ||
                    isCharDelimiterLex(cCurrChar) ||
                    isCharIdentLex(cCurrChar) ||
                    operators.checkChar(cCurrChar))) {
                cCurrChar = getNextChar();
            }
            usErrorCode = LexNoError;
            rewindChar();
            return getNextToken();
        }
        return TokenEndOfStream;
    }
    
    Token SoraLexer::getNextToken() {
        if(usErrorCode != LexNoError) return TokenInvalidInput;
        
        if(currState.iCurrLine >= contSource.size())
            return TokenEndOfStream;
        
        currState.tkTokenSubType = TokenSubTypeNone;
        currState.strCurrLexeme.clear();
        
        if(currState.iCurrLine != -1)
            contLexerState.push_back(currState);
        
        int iCurrState = LEX_STATE_START;
        bool iLexemeDone = false;
        
        int iCurrOpStateIndex = -1;
        
        char cCurrChar;
        bool iAddCurrChar;
        
        while(true) {
            if(currState.iCurrLine >= contSource.size())
                return TokenEndOfStream;
            
            cCurrChar = getNextChar();
            if(cCurrChar == '\0')
                break;
            //	if(cCurrChar == '\n')
            //		continue;
            
            iAddCurrChar = true;
            
            switch (iCurrState) {
                case LEX_STATE_START:
                    if(isCharWhitespaceLex(cCurrChar)) {
                        iAddCurrChar = false;
                    } else if(isCharNumericLex(cCurrChar)) {
                        // octal number begins with '0'
                        if(cCurrChar == '0')
                            currState.tkTokenSubType = TokenSubtypeOctalInt;
                        
                        iCurrState = LEX_STATE_INT;
                    } else if(cCurrChar == '.') {
                        if(isCharNumericLex( getLookAheadChar() ))
                            iCurrState = LEX_STATE_FLOAT;
                        else
                            iCurrState = LEX_STATE_DELIM;
                    } else if(isCharIdentLex(cCurrChar)) {
                        iCurrState = LEX_STATE_IDENT;
                    } else if(operators.checkChar(cCurrChar)) {
                        currState.iCurrOp = iCurrOpStateIndex;
                        
                        iCurrState = LEX_STATE_OP;
                    } else if(isCharDelimiterLex(cCurrChar)) {
                        iCurrState = LEX_STATE_DELIM;
                    } else if(cCurrChar == '"') {
                        iAddCurrChar = false;
                        iCurrState = LEX_STATE_STRING;
                    }
                    else {
                        return errorInvalidChar(cCurrChar);
                    }
                    break;
                    
                case LEX_STATE_INT:
                    if(isCharNumericLex(cCurrChar))
                        iCurrState = LEX_STATE_INT;
                    else if(cCurrChar == '.') {
                        // no special for float
                        if(currState.tkTokenSubType != TokenSubTypeNone) {
                            return errorInvalidChar(cCurrChar);
                        }
                        iCurrState = LEX_STATE_FLOAT;
                    }
                    else if(isCharNumericSpecial(cCurrChar)) {
                        // determine which kind of number
                        switch (cCurrChar) {
                            case 'x': currState.tkTokenSubType = TokenSubtypeHexInt; break;
                        }
                        iCurrState = LEX_STATE_INT;
                    }
                    else if(isCharWhitespaceLex(cCurrChar) || isCharDelimiterLex(cCurrChar) || operators.checkChar(cCurrChar)) {
                        iAddCurrChar = false;
                        iLexemeDone = true;
                    }
                    else {
                        return errorInvalidChar(cCurrChar);
                    }
                    break;
                    
                case LEX_STATE_FLOAT:
                    if(isCharNumericLex(cCurrChar))
                        iCurrState = LEX_STATE_FLOAT;
                    else if(isCharWhitespaceLex(cCurrChar) || isCharDelimiterLex(cCurrChar) || operators.checkChar(cCurrChar)) {
                        iAddCurrChar = false;
                        iLexemeDone = true;
                    } else if(cCurrChar == 'f') {
                        iAddCurrChar = false;
                    }
                    else {
                        return errorInvalidChar(cCurrChar);
                    }
                    break;
                    
                case LEX_STATE_IDENT:
                    if(isCharWhitespaceLex(cCurrChar) || isCharDelimiterLex(cCurrChar) || operators.checkChar(cCurrChar)) {
                        iAddCurrChar = false;
                        iLexemeDone = true;
                    } else if(isCharIdentLex(cCurrChar)) {
                        iCurrState = LEX_STATE_IDENT;
                    } else {
                        return errorInvalidChar(cCurrChar);
                    }
                    break;
                    
                case LEX_STATE_OP:
                    if((iCurrOpStateIndex = operators.check((currState.strCurrLexeme+cCurrChar))) != -1) {
                        currState.iCurrOp = iCurrOpStateIndex;
                    } else {
                        currState.iCurrOp = operators.check(currState.strCurrLexeme);
                        if(currState.iCurrOp == -1)
                            return errorInvalidChar(cCurrChar);
                        
                        iAddCurrChar = false;
                        iLexemeDone = true;
                    }
                    break;
                    
                case LEX_STATE_DELIM:
                    iAddCurrChar = false;
                    iLexemeDone = true;
                    break;
                    
                case LEX_STATE_STRING:
                    if(cCurrChar == '"') {
                        iAddCurrChar = false;
                        iCurrState = LEX_STATE_STRING_CLOSE_QUOTE;
                    } else if(cCurrChar == '\\') {
                        iAddCurrChar = false;
                        iCurrState = LEX_STATE_STRING_ESCAPE;
                    }
                    break;
                    
                case LEX_STATE_STRING_ESCAPE:
                    iCurrState = LEX_STATE_STRING;
                    break;
                    
                case LEX_STATE_STRING_CLOSE_QUOTE:
                    iAddCurrChar = false;
                    iLexemeDone = true;
                    break;
            }
            if(iAddCurrChar) {
                currState.strCurrLexeme += cCurrChar;
            }
            
            if(iLexemeDone)
                break;
        }
        
        if(currState.iCurrLexemeIndex > 0)
            --currState.iCurrLexemeIndex;
        
        Token tokenType;
        switch (iCurrState) {
            case LEX_STATE_INT:
                tokenType = TokenInt;
                break;
                
            case LEX_STATE_FLOAT:
                tokenType = TokenFloat;
                break;
                
            case LEX_STATE_IDENT:
                tokenType = TokenIdent;
                break;
                
            case LEX_STATE_DELIM:
                switch (currState.strCurrLexeme[0]) {
                    case ',': tokenType = TokenDelimComma; break;
                    case '(': tokenType = TokenDelimOpenParen; break;
                    case ')': tokenType = TokenDelimCloseParen; break;
                    case '[': tokenType = TokenDelimOpenBrace; break;
                    case ']': tokenType = TokenDelimCloseBrace; break;
                    case '{': tokenType = TokenDelimOpenCurlyBrace; break;
                    case '}': tokenType = TokenDelimCloseCurlyBrace; break;
                    case ';': tokenType = TokenDelimSemicolon; break;
                    case '.': tokenType = TokenDelimPeriod; break;
                    case ':': tokenType = TokenDelimColon; break;
                }
                break;
                
            case LEX_STATE_OP:
                tokenType = TokenOperator;
                break;
                
            case LEX_STATE_STRING_CLOSE_QUOTE:
                tokenType = TokenString;
                break;
                
            default:
                tokenType = TokenEndOfStream;
                break;
        }
        
        return tokenType;
    }
    
    int SoraLexer::rewindTokenStream() {
        if(contLexerState.size() > 0) {
            currState = contLexerState.back();
            contLexerState.pop_back();
            return true;
        }
        return false;
    }
    
    int SoraLexer::asInt() const {
        StringType str = getCurrLexeme();
        return str.asInt();
    }
    
    bool SoraLexer::asBool() const {
        StringType str = getCurrLexeme();
        return str.asBool();
    }
    
    float SoraLexer::asFloat() const {
        StringType str = getCurrLexeme();
        return str.asFloat();
    }
    
    const char* SoraLexer::getCurrLexeme() const {
        return currState.strCurrLexeme.c_str();
    }
    
    int SoraLexer::getCurrOp() const {
        return currState.iCurrOp;
    }
    
    char SoraLexer::getCurrInvalidChar() const {
        return strInvalidChar;
    }
    
    Token SoraLexer::getCurrTokenSubtype() const {
        return currState.tkTokenSubType;
    }
    
    size_t SoraLexer::getCurrCharIndex() const {
        return currState.iCurrLexemeIndex;
    }
    
    size_t SoraLexer::getCurrLineIndex() const {
        return currState.iCurrLine;
    }
    
    size_t SoraLexer::getSourceLineCount() const {
        return contSource.size();
    }
    
    size_t SoraLexer::getSourceSize() const {
        return lSourceSize;
    }
    
    const char*	SoraLexer::getCurrLine() const {
        if(currState.iCurrLine < contSource.size())
            return contSource[currState.iCurrLine].c_str();
        return 0;
    }
    
    void SoraLexer::addOperator(const std::string& op, int ident) {
        operators.add(op, ident);
    }

    
} // namespace sora