//
//  SoraFourCC.h
//  Sora
//
//  Created by Ruiwei Bu on 10/2/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraFourCC_h
#define Sora_SoraFourCC_h

#include "SoraPlatform.h"

#include <string>

namespace sora {
    
    /**
     * Simple four-character-code implemention
     * Can be compared and calculated from/to string
     **/
    
    class SoraFourCC {
    public:
        SoraFourCC();
        SoraFourCC(uint32 f);
        SoraFourCC(const std::string& s);
        
        bool operator==(const SoraFourCC& rhs) const;
        bool operator!=(const SoraFourCC& rhs) const;
        bool operator<(const SoraFourCC& rhs) const;
        bool operator>(const SoraFourCC& rhs) const;
        bool operator<=(const SoraFourCC& rhs) const;
        bool operator>=(const SoraFourCC& rhs) const;
        
        bool isValid() const;
        void fromUInt(uint32 f);
        uint32 asUInt() const;
        
        void fromString(const std::string& s);
        std::string toString() const;
        
        static std::string ToString(const SoraFourCC& f);
        static SoraFourCC FromString(const std::string& s);
        
    private:
        uint32 mFourCC;
    };
    
    inline SoraFourCC::SoraFourCC():
    mFourCC(0) {
        
    }
    
    inline SoraFourCC::SoraFourCC(uint32 f):
    mFourCC(f) {
        
    }
    
    inline SoraFourCC::SoraFourCC(const std::string& f):
    mFourCC(f) {
        this->fromString(f);
    }
    
    inline bool SoraFourCC::operator==(const SoraFourCC& rhs) const {
        return this->mFourCC == rhs.mFourCC;
    }
    
    inline bool SoraFourCC::operator!=(const SoraFourCC& rhs) const {
        return this->mFourCC != rhs.mFourCC;
    }
    
    inline bool SoraFourCC::operator<(const SoraFourCC& rhs) const {
        return this->mFourCC < rhs.mFourCC;
    }
    
    inline bool SoraFourCC::operator>(const SoraFourCC& rhs) const {
        return this->mFourCC > rhs.mFourCC;
    }
    
    inline bool SoraFourCC::operator<=(const SoraFourCC& rhs) const {
        return this->mFourCC <= rhs.mFourCC;
    }
    
    inline bool SoraFourCC::operator>=(const SoraFourCC& rhs) const {
        return this->mFourCC >= rhs.mFourCC;
    }
    
    inline bool SoraFourCC::isValid() const {
        return this->mFourCC != 0;
    }
    
    inline void SoraFourCC::fromString(const std::string& s) {
        sora_assert(!s.empty() &&
                    s.size() >= 4);
        
        mFourCC = uint32(s[0] | s[1]<<8 | s[2]<<16 | s[3]<<24);
    }
    
    inline std::string SoraFourCC::toString() const {
        sora_assert(isValid());
        
        std::string str("1234");
        str[0] = static_cast<char>((mFourCC & 0xFF000000) >> 24);
        str[1] = static_cast<char>((mFourCC & 0x00FF0000) >> 16);
        str[2] = static_cast<char>((mFourCC & 0x0000FF00) >> 8);
        str[3] = static_cast<char>((mFourCC & 0x000000FF));
        return str;
    }
    
    inline void SoraFourCC::fromUInt(uint32 i) {
        mFourCC = i;
    }
    
    inline uint32 SoraFourCC::toUInt() const {
        return mFourCC;
    }
    
    inline SoraFourCC SoraFourCC::FromString(const std::string& s) {
        SoraFourCC cc;
        cc.fromString(s);
        return cc;
    }
    
    inline std::string SoraFourCC::ToString(const SoraFourCC& rhs) {
        return rhs.toString();
    }
    
} // namespace sora



#endif
