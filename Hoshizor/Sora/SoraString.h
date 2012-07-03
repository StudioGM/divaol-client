#ifndef SORA_STRING_H
#define SORA_STRING_H

#include <string>
#include "SoraConfig.h"
#include "SoraStringConv.h"

namespace sora {
    
    namespace util {

        class String {
        public:
            String();
            String(const std::string& str);
            String(const std::wstring& str);
            String(const char* str);
            String(const wchar_t* str);
            
            std::string get() const;
            std::wstring getw() const;
            
            const char* c_str() const;
            const wchar_t* wc_str() const;
            
            uint64 uniqueId() const;
            
            void set(const std::string& str);
            void set(const std::wstring& str);
            void set(const char* str);
            void set(const wchar_t* str);
            
            bool operator==(const String& rhs) const;
            
            bool operator!=(const std::string& str) const;
            bool operator!=(const std::wstring& str) const;
            bool operator!=(const String& rhs) const;
            
            bool compare(const std::string& str) const;
            bool compare(const std::wstring& str) const;
            bool compare(const String& rhs) const;
            
            String toUpper() const;
            String toLower() const;
            
            String& operator=(const String& str);
            String operator+(const String& str) const;
            String& operator+=(const String& str);
            
            bool operator<(const String& rhs) const;
            bool operator>(const String& rhs) const;
           
            operator std::string() const {
                return mString;
            }
            operator std::wstring() const {
                return s2ws(mString);
            }
            
            char operator[](size_t index) const;
            
            char at(size_t index) const;
            wchar_t wat(size_t index) const;
            
            size_t size() const;
            bool empty() const;
            
            int asInt() const;
            bool asBool() const;
            float asFloat() const;
            
            bool isValid() const;
            
            void clear();
            
        protected:
            std::string mString;
        };
        
        inline int String::asInt() const {
            return atoi(mString.c_str());
        }
        
        inline bool String::asBool() const {
            static const char* bools[] = {
                "no", "yes", "off", "on", "false", "true", "0", "1", 0
            };
            int i = 0;
            while(bools[i] != 0) {
                if(mString == bools[i]) {
                    return 1 == (i & 1);
                }
                ++i;
            }
            return false;
        }
        
        inline String operator+(const std::string& l, const String& r) {
            return l + r.get();
        }
        
        inline String operator+(const std::wstring& l, const String& r) {
            return l + r.getw();
        }
        
        inline float String::asFloat() const {
            return float(atof(mString.c_str()));
        }
        
        
        inline bool String::compare(const std::string& str) const {
            return mString == str;
        }
        
        inline bool String::compare(const std::wstring& str) const {
            return str == s2ws(mString);
        }
        
        inline bool String::operator==(const String& rhs) const {
            return mString == rhs.mString;
        }
        
        inline bool String::operator!=(const std::string& str) const {
            return !(*this == str);
        }
        
        inline bool String::operator!=(const std::wstring& str) const {
            return !(*this == str);
        }
        
        inline bool String::operator!=(const String& rhs) const {
            return !(*this == rhs);
        }
        
        inline bool String::compare(const String& rhs) const {
            return mString == rhs.mString;
        }
        
        inline String String::operator+(const String& rhs) const {
            return mString + rhs;
        }
        
        inline String& String::operator+=(const String& rhs) {
            mString += rhs;
            return *this;
        }
        
        inline std::string String::get() const {
            return mString;
        }
        
        inline std::wstring String::getw() const {
            return s2ws(mString);
        }
        
        inline const char* String::c_str() const {
            return mString.c_str();
        }
        
        inline const wchar_t* String::wc_str() const {
            return s2ws(mString).c_str();
        }
        
        
        inline void String::set(const std::string& str) {
            mString = str;
        }
        
        inline void String::set(const std::wstring& str) {
            mString = ws2s(str); 
        }
        
        inline void String::set(const char* str) {
            mString = str;
        }
        
        inline void String::set(const wchar_t* str) {
            mString = ws2s(str);
        }
        
        inline String& String::operator=(const String& str) {
            mString = str.mString;
            return *this;
        }
        
        inline bool String::isValid() const {
            return !mString.empty();
        }
        
        inline void String::clear() {
            mString.clear();
        }
        
        inline bool String::operator<(const String& rhs) const {
            return mString < rhs.mString;
        }
        
        inline bool String::operator>(const String& rhs) const {
            return mString > rhs.mString;
        }
        
        inline bool String::empty() const {
            return mString.empty();
        }
    }
    
    typedef ::sora::util::String StringType;

} // namespace sora

#endif // SORA_STRING_H