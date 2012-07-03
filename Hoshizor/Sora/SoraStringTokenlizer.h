//
//  SoraStringTokenlizer.h
//  Sora
//
//  Created by Robert Bu on 7/9/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraStringTokenlizer_h
#define Sora_SoraStringTokenlizer_h

#include "SoraPlatform.h"
#include "SoraStringConv.h"
#include <vector>

namespace sora {
    
    class SORA_API SoraStringTokenlizer {
    public:
        typedef std::vector<std::string> TokenContainer;
        typedef TokenContainer::iterator Iterator;
        typedef Iterator iterator;
        
        SoraStringTokenlizer();
        SoraStringTokenlizer(const std::string& str);
        SoraStringTokenlizer(const std::string& str, const std::string& deli);
        
        void parse(const std::string& str);
        void parse(const std::string& str, const std::string& deli);
        
        std::string first();
        std::string last();
    
        std::string front();
        std::string back();
        
        iterator begin();
        iterator end();
        
        size_t size() const;
        
        void operator=(const std::string& str);
        void operator=(const char* str);
        
        const TokenContainer& getTokens() const;
        
        std::string operator[](size_t index);
    
    protected:
        bool isSpace(char c);
        bool isAlpha(char c);
        
        TokenContainer mTokens;
    };
        
} // namespace sora


#endif
