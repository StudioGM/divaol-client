//
//  SoraDirectoryIterator.h
//  Sora
//
//  Created by Robert Bu on 7/11/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraDirectoryIterator_h
#define Sora_SoraDirectoryIterator_h

#include "SoraPlatform.h"
#include "SoraPath.h"

namespace sora {
    
    class SoraDirectoryIteratorImpl;
    
    class SORA_API SoraDirectoryIterator {
    public:
        SoraDirectoryIterator();
        SoraDirectoryIterator(const SoraDirectoryIterator& iterator);
        explicit SoraDirectoryIterator(const std::string& path);
        explicit SoraDirectoryIterator(const std::wstring& path);
        explicit SoraDirectoryIterator(const char* path);
        explicit SoraDirectoryIterator(const wchar_t* path);
        explicit SoraDirectoryIterator(const SoraPath& path);
        ~SoraDirectoryIterator();
        
        std::string name() const;
        const SoraPath& path() const;
        
        bool isEnd() const;
        
        SoraDirectoryIterator& operator=(const SoraDirectoryIterator& it);
        
        SoraDirectoryIterator& operator++();
        
        bool operator == (const SoraDirectoryIterator& iterator) const;
        bool operator != (const SoraDirectoryIterator& iterator) const;
        
    private:
        SoraPath mPath;
        SoraDirectoryIteratorImpl* mImpl;
        bool mIsEnd;
    };
    
    inline std::string SoraDirectoryIterator::name() const {
        return mPath.getFileName();
    }
    
    inline const SoraPath& SoraDirectoryIterator::path() const {
        return mPath;
    }
    
    inline bool SoraDirectoryIterator::isEnd() const {
        return mIsEnd;
    }
    
    inline bool SoraDirectoryIterator::operator ==(const SoraDirectoryIterator& rhs) const {
        return name() == rhs.name();
    }
    
    inline bool SoraDirectoryIterator::operator !=(const SoraDirectoryIterator& rhs) const {
        return name() != rhs.name();
    }
} // namespace sora


#endif
