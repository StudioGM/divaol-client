//
//  SoraStringTokenlizer.cpp
//  Sora
//
//  Created by Robert Bu on 7/10/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraStringTokenlizer.h"

namespace sora {
    
    SoraStringTokenlizer::SoraStringTokenlizer() {
        
    }
    
    SoraStringTokenlizer::SoraStringTokenlizer(const std::string& str) {
        parse(str, std::string());
    }
     
    SoraStringTokenlizer::SoraStringTokenlizer(const std::string& str, const std::string& deli) {
        parse(str, deli);
    }
    
    std::string SoraStringTokenlizer::first() {
        if(mTokens.size() != 0)
            return mTokens.front();
        return std::string();
    }
    
    std::string SoraStringTokenlizer::last() {
        if(mTokens.size() != 0)
            return mTokens.back();
        return std::string();
    }

    std::string SoraStringTokenlizer::front() {
        if(mTokens.size() != 0)
            return mTokens.front();
        return std::string();
    }
    
    std::string SoraStringTokenlizer::back() {
        if(mTokens.size() != 0)
            return mTokens.back();
        return std::string();
        
    }
    
    SoraStringTokenlizer::iterator SoraStringTokenlizer::begin() {
        return mTokens.begin();
    }
    
    SoraStringTokenlizer::iterator SoraStringTokenlizer::end() {
        return mTokens.end();
    }
    
    const SoraStringTokenlizer::TokenContainer& SoraStringTokenlizer::getTokens() const {
        return mTokens;
    }
    
    bool SoraStringTokenlizer::isSpace(char c) {
        return (c == '\n' || c == ' ' || c == 'r' || c == '\t');
    }
    
    bool SoraStringTokenlizer::isAlpha(char c) {
        return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'));
    }
    
    void SoraStringTokenlizer::parse(const std::string& str) {
        parse(str, std::string());
    }
    
    std::string SoraStringTokenlizer::operator[](size_t index) {
        sora_assert(index < mTokens.size());
        
        return mTokens[index];
    }
    
    void SoraStringTokenlizer::parse(const std::string& str, const std::string& deli) {
        size_t start = 0, end = 0;
        if(deli.size() != 0) {
            while(end < str.size()) {
                while((deli.find(str[end]) == std::string::npos) && end < str.size()) { ++end; }
                if(end == str.size()) {
                    while(deli.find(str[end]) != std::string::npos)
                        --end;
                    mTokens.push_back( str.substr(start, end-start) );
                    break;
                }
                if(end != start)
                    mTokens.push_back( str.substr(start, end-start) );
                
                ++end; 
                while((deli.find(str[end]) != std::string::npos || isSpace(str[end])) && end < str.size()) ++end;
                
                start = end;
            }
        } else {
            while(end < str.size()) {
                while(isAlpha(str[end]) && end < str.size()) { ++end; }
                if(end == str.size()) {
                    mTokens.push_back( str.substr(start) );
                    break;
                }
                mTokens.push_back( str.substr(start, end-start) );
                
                ++end; 
                while(!isAlpha(str[end]) && end < str.size()) {
                    if(str[end] == '"') {
                        ++end;
                        start = end;
                        while(str[end] != '"' && end < str.size())
                            ++end;
                        mTokens.push_back( str.substr(start, end-start) );
                    }
                    ++end;
                
                }
                start = end;
            }
        }
    }
    
    void SoraStringTokenlizer::operator=(const std::string& str) {
        parse(str, std::string());
    }
    
    void SoraStringTokenlizer::operator=(const char* str) {
        parse(str, std::string());
    }
    
    size_t SoraStringTokenlizer::size() const {
        return mTokens.size();
    }
} // namespace sora
