//
//  Any.h
//  Sora
//
//  Created by Robert Bu on 7/8/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_Any_h
#define Sora_Any_h

#include <algorithm>
#include <typeinfo>

#include "SoraException.h"

namespace sora {
    
    /**
     * A class that holds a general type
     * Code from Boost Library and Poco Library, Original Copyright by Kevlin Henney and Applied Infomatics
     * Modified for Hoshizora by Griffin Bu
     **/
    
    class SORA_API SoraAny {
    public:
        SoraAny():
        mContent(NULL) {
        }
        
        template<typename ValueType>
        SoraAny(const ValueType& value):
        mContent(new Holder<ValueType>(value)) {
        }
        
        SoraAny(const SoraAny& other):
        mContent(other.mContent ? other.mContent->clone(): 0) {
        }
        
        ~SoraAny() {
            if(mContent)
                delete mContent;
        }
        
        SoraAny& swap(SoraAny& rhs) {
            std::swap(mContent, rhs.mContent);
            return *this;
        }
        
        template<typename ValueType>
        SoraAny& operator=(const ValueType& rhs) {
            SoraAny(rhs).swap(*this);
            return *this;
        }

		SoraAny& operator=(const SoraAny& rhs) {
            SoraAny(rhs).swap(*this);
            return *this;
        }
        
        bool empty() const {
            return !mContent;
        }
        
        const std::type_info& type() const {
            return mContent ? mContent->type(): typeid(void);
        }
        
    private:
        class Placeholder {
        public:
            virtual ~Placeholder() {
                
            }
            
            virtual const std::type_info& type() const = 0;
            virtual Placeholder* clone() const = 0;
        };
        
        template<typename ValueType>
        class Holder: public Placeholder {
        public:
            Holder(const ValueType& value):
            mHeld(value) {
                
            }
            
            virtual const std::type_info& type() const {
                return typeid(ValueType);
            }
            
            virtual Placeholder* clone() const {
                return new Holder(mHeld);
            }
            
            ValueType mHeld;
        };
        
    private:
        template<typename ValueType>
        friend ValueType* AnyCast(SoraAny*);
        
        template<typename ValueType>
        friend ValueType* UnsafeAnyCast(SoraAny*);
        
        Placeholder* mContent;
    };
    
    template<typename ValueType>
    inline ValueType* AnyCast(SoraAny* operand) {
        return operand && operand->type() == typeid(ValueType) ?
        &static_cast<SoraAny::Holder<ValueType>*>(operand->mContent)->mHeld
        : 0;
    }
    
    template<typename ValueType>
    inline const ValueType* AnyCast(const SoraAny* operand) {
        return AnyCast<ValueType>(const_cast<SoraAny*>(operand));
    }
    
    template<typename ValueType>
    inline ValueType AnyCast(const SoraAny& operand) {
        ValueType* result = AnyCast<ValueType>(const_cast<SoraAny*>(&operand));
        if(!result)
            THROW_SORA_EXCEPTION(BadCastException, "Faild to conver between const any types");
        return *result;
    }
    
    template<typename ValueType>
    inline ValueType AnyCast(SoraAny& operand) {
        ValueType* result = AnyCast<ValueType>(&operand);
        if(!result)
            THROW_SORA_EXCEPTION(BadCastException, "Faild to conver between const any types");
        return *result;
    }
    
    template<typename ValueType>
    inline const ValueType& RefAnyCast(const SoraAny& operand) {
        ValueType* result = AnyCast<ValueType>(const_cast<SoraAny*>(&operand));
        if(!result)
            THROW_SORA_EXCEPTION(BadCastException, "Faild to conver between const any types");
        return *result;
    }
    
    template<typename ValueType>
    inline ValueType* UnsafeAnyCast(SoraAny* operand) {
        return &static_cast<SoraAny::Holder<ValueType>*>(operand->mContent)->mHeld;
    }
    
    template<typename ValueType>
    inline const ValueType* UnsafeAnyCast(const SoraAny* operand) {
        return AnyCast<ValueType>(const_cast<SoraAny*>(operand));
    }
    
    inline bool IsAnyInt(const SoraAny& any) {
        return any.type() == typeid(int32);
    }
    
    inline bool IsAnyFloat(const SoraAny& any) {
        return any.type() == typeid(float);
    }
    
    inline bool IsAnyUInt(const SoraAny& any) {
        return any.type() == typeid(uint32);
    }
    
    inline bool IsAnyDouble(const SoraAny& any) {
        return any.type() == typeid(double);
    }
    
    inline bool IsAnyLong(const SoraAny& any) {
        return any.type() == typeid(long);
    }
    
    inline bool IsAnyChar(const SoraAny& any) {
        return any.type() == typeid(char);
    }
    
    inline bool IsAnyUChar(const SoraAny& any) {
        return any.type() == typeid(unsigned char);
    }
    
    inline bool IsAnyULong(const SoraAny& any) {
        return any.type() == typeid(SoraHandle);
    }
    
    inline bool IsAnyInt64(const SoraAny& any) {
        return any.type() == typeid(int64);
    }
    
    inline bool IsAnyUInt64(const SoraAny& any) {
        return any.type() == typeid(uint64);
    }
    
 /*   static bool isAnyStringId(const SoraAny& any) {
        return any.type() == typeid(SoraStringId);
    }
   */ 
    template<typename T>
    inline bool IsAnyType(const SoraAny& any) {
        return any.type() == typeid(T);
    }
    
    template<typename T>
    inline const char* GetTypeName() {
        return typeid(T).name();
    }
} // namespace sora

#endif
