//
//  SoraFileStream.h
//  Sora
//
//  Created by Robert Bu on 7/7/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraFileStream_h
#define Sora_SoraFileStream_h

#include "SoraPlatform.h"

namespace sora {
    
    class SORA_API SoraFileStreamBasic {
    public:
        enum SeekType {
            Begin = 0,
            Current = 1,
            End = 2,
        };
        
        virtual ~SoraFileStreamBasic() {}
        virtual bool open(const std::string& filename, bool canwrite = false, bool append = false, bool nocache = false) = 0;
        virtual bool open(const std::wstring& filename, bool canwrite = false, bool append = false, bool nocache = false) = 0;
      
        virtual void close() = 0;
        virtual void truncate() = 0;
        virtual uint64 size() = 0;
        virtual uint64 pos() = 0;
        virtual void pos(uint64 pos, SeekType type) = 0;
        virtual uint32 read(void* data, uint32 len) = 0;
        virtual uint32 write(const void* data, uint32 len) = 0;
        template<typename T>
        inline SoraFileStreamBasic& operator<<(const T& val) {
            write(&val, sizeof(T));
            return *this;
        }
        template<typename T>
        inline SoraFileStreamBasic& operator>>(T& val) {
            read(&val, sizeof(T));
            return *this;
        }
    };
    
#ifdef OS_WIN32
    /*
     Win32 implemention
     */
    class SORA_API SoraFileStreamWin32: public SoraFileStreamBasic {
    public:
        SoraFileStreamWin32();
        virtual ~SoraFileStreamWin32();
        virtual bool open(const std::string& filename, bool canwrite = false, bool append = false, bool nocache = false);
        virtual bool open(const std::wstring& filename, bool canwrite = false, bool append = false, bool nocache = false);
        
        virtual void close();
        virtual void truncate();
        virtual uint64 size();
        virtual uint64 pos();
        virtual void pos(uint64 pos, SeekType type);
        virtual uint32 read(void* data, uint32 len);
        virtual uint32 write(const void* data, uint32 len);
        
    private:
        HANDLE file;
    };
    
    typedef SoraFileStreamWin32 SoraFileStream;
    
#else
    /*
     POSIX implemention
     */
    class SORA_API SoraFileStreamPosix: public SoraFileStreamBasic {
    public:
        SoraFileStreamPosix();
        virtual ~SoraFileStreamPosix();
        virtual bool open(const std::string& filename, bool canwrite = false, bool append = false, bool nocache = false);
        virtual bool open(const std::wstring& filename, bool canwrite = false, bool append = false, bool nocache = false);
        
        virtual void close();
        virtual void truncate();
        virtual uint64 size();
        virtual uint64 pos();
        virtual void pos(uint64 pos, SeekType type);
        virtual uint32 read(void* data, uint32 len);
        virtual uint32 write(const void* data, uint32 len);
        
    private:
        bool nocache;
        FILE* file;
    };
    
    typedef SoraFileStreamPosix SoraFileStream;
 
#endif //OS_WIN32
}

#endif
