#ifndef SORA_STREAM_H_
#define SORA_STREAM_H_

#include "SoraAutoPtr.h"

#include <vector>

namespace sora {
    
    /**
     *  Original form Poco C++ Library by Applied Infomatics
     *  Modified for Hoshizora and add wstring support by Robert Bu
     **/

    
	class SORA_API SoraStream: public SoraSharedClass<SoraStream> {
	public:
		SoraStream(): _pos(0) {}
		SoraStream(const uint8* buf, size_t len): _pos(0) {
			if(len > 0) {
				_buf.assign(buf, buf + len);
			}
		}
        
		SoraStream(std::vector<uint8>& v): _pos(0) {
			_buf.assign(v.begin(), v.end());
		}
        
		inline operator std::vector<uint8>&() {
            return _buf;
        }
		
        inline operator uint8*() {
            return _buf.empty() ? NULL : &_buf[0];
        }
        
		inline size_t size() {
            return _buf.size();
        }
        
		inline size_t left() {
            return _buf.size() - _pos;
        }
        
		inline bool empty() {
            return _buf.empty();
        }
        
		inline void reset() {
			_pos = 0;
		}
		
        inline void clear() {
			_buf.clear();
			reset();
		}
        
		size_t first_of(uint8 b) {
			size_t c = _buf.size();
			for(size_t i = 0; i < c; ++ i)
			{
				if(_buf[i] == b)
				{
					return i;
				}
			}
			return (size_t)-1;
		}
        
		inline void pop_front(size_t n) {
			if(n >= _buf.size())
				_buf.clear();
			else
				_buf.erase(_buf.begin(), _buf.begin() + n);
		}
        
		inline void remove(size_t s, size_t n) {
			if(s >= _buf.size())
				return;
			if(s + n > _buf.size())
				n = _buf.size() - s;
			_buf.erase(_buf.begin() + s, _buf.begin() + s + n);
		}
        
		inline void append_zero(size_t len) {
			if(len > 0)
				_buf.resize(_buf.size() + len, 0);
		}
        
		inline void append(const uint8 * buf, size_t len) {
			_buf.insert(_buf.end(), buf, buf + len);
		}
        
		inline void prepend(const uint8 * buf, size_t len) {
			_buf.insert(_buf.begin(), buf, buf + len);
		}
        
		inline size_t read(uint8 * buf, size_t len) {
			if(_pos + len > _buf.size())
				len = _buf.size() - _pos;
			if(len == 0)
				return 0;
			memcpy(buf, &_buf[_pos], len);
			_pos += len;
			return len;
		}
        
		template<typename T>
		SoraStream& operator>>(T& v) {
			read((uint8 *)&v, sizeof(T));
			return *this;
		}
        
		template<typename T>
		SoraStream& operator<<(const T& v) {
			append((uint8 *)&v, sizeof(T));
			return *this;
		}
        
		SoraStream& operator>>(std::vector<uint8>& v) {
			read(&v.front(), v.size());
			return *this;
		}
        
		SoraStream& operator<<(const std::vector<uint8>& v) {
			_buf.insert(_buf.end(), v.begin(), v.end());
			return *this;
		}
        
		template<typename T>
		SoraStream& operator>>(std::vector<T>& v) {
			uint32 n;
			v.clear();
			(*this)>>n;
			if(n > 0)
			{
				v.resize(n);
				for(uint32 i = 0; i < n; ++ i)
					read(&v[i], sizeof(T));
			}
			return *this;
		}
        
		template<typename T>
		SoraStream& operator<<(const std::vector<T>& v) {
			uint32 n = v.size();
			(*this)<<n;
			for(uint32 i = 0; i < n ; ++ i) {
				append((uint8 *)&v[i], sizeof(T));
			}
			return *this;
		}
        
		SoraStream& operator>>(std::string& v) {
			size_t s = _buf.size();
			v.clear();
			size_t n = _pos;
			while(n < s && _buf[n] != 0) {
				++ n;
			}
			if(n > _pos) {
				v.resize(n - _pos);
				memcpy(&v[0], &_buf[_pos], n - _pos);
			}
			if(n == s)
				_pos = n;
			else
				_pos = n + 1;
			return *this;
		}
        
		SoraStream& operator<<(const std::string& v) {
			_buf.insert(_buf.end(), v.begin(), v.end());
			_buf.push_back((uint8)0);
			return *this;
		}
        
		SoraStream& operator<<(const char * v) {
			_buf.insert(_buf.end(), v, v + strlen(v) + 1);
			return *this;
		}
        
		SoraStream& operator+=(uint32 n) {
			_pos += n;
			if(_pos > _buf.size())
				_pos = _buf.size();
			return *this;
		}
        
		SoraStream& operator-=(uint32 n) {
			if(_pos < n)
				_pos = 0;
			else
				_pos -= n;
			return *this;
		}
        
	protected:
		size_t _pos;
		std::vector<uint8> _buf;
	};
}

#endif // _STREAM_H_
