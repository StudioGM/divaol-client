//
//  SoraFileStream.cpp
//  Sora
//
//  Created by Robert Bu on 7/7/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraFileStream.h"

#include "SoraStringConv.h"

namespace sora {
    
#ifdef OS_WIN32
    
    SoraFileStreamWin32::SoraFileStreamWin32() {
        file = NULL;
    }
    
    SoraFileStreamWin32::~SoraFileStreamWin32() {
        close();
    }
    
    bool SoraFileStreamWin32::open(const std::string& filename, bool canwrite, bool append, bool nocache) {
        if(file)
            return false;
        
        file = CreateFileA(filename.c_str(), 
                           (canwrite ? GENERIC_WRITE : 0) | GENERIC_READ, FILE_SHARE_READ, NULL, canwrite ? OPEN_ALWAYS : OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | (nocache ? FILE_FLAG_WRITE_THROUGH : 0), 
                           NULL);
        if(file == NULL || file == INVALID_HANDLE_VALUE) {
            return false;
        }
        if(append) {
            SetFilePointer(file, 0, NULL, FILE_END);
        }
        return true;
    }
    
    bool SoraFileStreamWin32::open(const std::wstring& filename, bool canwrite, bool append, bool nocache) {
        if(file)
            return false;
        
        file = CreateFileW(filename.c_str(), 
                           (canwrite ? GENERIC_WRITE : 0) | GENERIC_READ, FILE_SHARE_READ, NULL, canwrite ? OPEN_ALWAYS : OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | (nocache ? FILE_FLAG_WRITE_THROUGH : 0), 
                           NULL);
        if(file == NULL || file == INVALID_HANDLE_VALUE) {
            return false;
        }
        if(append) {
            SetFilePointer(file, 0, NULL, FILE_END);
        }
		return true;
    }
    
    void SoraFileStreamWin32::close() {
        if(file != NULL) {
            CloseHandle(file);
            file = NULL;
        }
    }
    
    void SoraFileStreamWin32::truncate() {
        SetEndOfFile(file);
    }
    
    uint64 SoraFileStreamWin32::size() {
        DWORD low, high;
        low = GetFileSize(file, &high);
        return (uint64)low + (((uint64)high)<<32);
    }
    
    uint64 SoraFileStreamWin32::pos() {
        DWORD low;
        LONG high = 0;
        low = SetFilePointer(file, 0, &high, FILE_CURRENT);
        return (uint64)low + (((uint64)high)<<32);
    }
    
    void SoraFileStreamWin32::pos(uint64 pos, SeekType type) {
        LONG high = (LONG)(pos >> 32);
        SetFilePointer(file, pos & 0xFFFFFFFF, (high > 0 ? &high : NULL), type);
    }
                       
    uint32 SoraFileStreamWin32::read(void* data, uint32 len) {
        DWORD size;
        if(ReadFile(file, data, len, &size, NULL))
            return size;
        return 0;
    }
    
    uint32 SoraFileStreamWin32::write(const void* data, uint32 len) {
        DWORD size;
        if(WriteFile(file, data, len, &size, NULL))
            return size;
        return 0;
    }
    
#else 
    
    SoraFileStreamPosix::SoraFileStreamPosix() {
        nocache = false;
        file = NULL;
    }
    
    SoraFileStreamPosix::~SoraFileStreamPosix() {
        close();
    }
     
    bool SoraFileStreamPosix::open(const std::string& filename, bool canwrite, bool append , bool nocache) {
        if(file != NULL)
            return false;
        
        this->nocache = nocache;
        file = fopen(filename.c_str(), canwrite ? (append ? "a+b" : "r+b") : "rb");
        if(file == NULL) {
            if(canwrite)
                file = fopen(filename.c_str(), "w+b");
            if(file == NULL)
                return false;
        }
        return true;
    }
    
    bool SoraFileStreamPosix::open(const std::wstring& filename, bool canwrite, bool append, bool nocache) {
        std::string sname = ws2s(filename);
        return open(sname);
    }
    
    void SoraFileStreamPosix::close() {
        if(file != NULL) {
            fclose(file);
            file = NULL;
        }
    }
    
    void SoraFileStreamPosix::truncate() {
        
    }
    
    uint64 SoraFileStreamPosix::size() {
        uint32 pos = (uint32)ftell(file);
        fseek(file, 0, SEEK_END);
        uint64 size = (uint64)ftell(file);
        fseek(file, pos, SEEK_SET);
        return size;
    }
    
    uint64 SoraFileStreamPosix::pos() {
        return (uint64)ftell(file);
    }
    
    void SoraFileStreamPosix::pos(uint64 pos, SeekType type) {
        fseek(file, (uint32)pos, type);
    }
    
    uint32 SoraFileStreamPosix::read(void* data, uint32 len) {
        return (uint32)fread(data, 1, len, file);
    }
    
    uint32 SoraFileStreamPosix::write(const void* data, uint32 len) {
        if(nocache) 
            return (uint32)fwrite(data, 1, len, file);
        else {
            uint32 size = (uint32)fwrite(data, 1, len, file);
            fflush(file);
            return size;
        }
    }

    
#endif // OS_WIN32
    
} // namespace sora