#include "SoraZipFile.h"
#include "SoraMemory.h"
#include <string>
#include <iostream>
#include <memory.h>
#include <string.h>

#if !defined(__APPLE__)
#include <malloc.h>
#endif

namespace sora {
    
    inline void _sstrupr(std::string& str) {
        for(size_t i=0; i<str.size(); ++i) {
            if(islower(str[i]))
                str[i] = toupper(str[i]);
        }
    }
    
    unzFile unzOpenFromMemory(voidpf data, uLong size);
    
    SoraZipFile::SoraZipFile() {
        setOpen(false);
    }
    
    SoraZipFile::SoraZipFile(const StringType& filePath) {
        open(filePath);
    }
    
    SoraZipFile::~SoraZipFile() {
        close();
    }
    
    bool SoraZipFile::open(const StringType& filePath) {
        if(isOpen()) close();
        
        m_UnzFile = unzOpen(filePath.get().c_str());
        
        if(m_UnzFile != 0) {
            _cache();
            setOpen(true);
            setFilePath(filePath);
            return true;
        }
        return false;
    }
    
    void SoraZipFile::_cache() {
        unzFile hFile = m_UnzFile;
        int result = unzGoToFirstFile(hFile);
        
        while(result == UNZ_OK) {
            unz_file_info info;
            char szCurrentFile[512];
            unzGetCurrentFileInfo(hFile, &info, szCurrentFile, 512, NULL, 0, NULL, 0);
            for (size_t nChar = 0; szCurrentFile[nChar]; ++nChar) {
                if (szCurrentFile[nChar] == '\\')
                    szCurrentFile[nChar] = '/';
            }
            std::string file = szCurrentFile;
            _sstrupr(file);
            
            uLong nSeekPos = unzGetOffset(hFile);
            m_Seeker.insert(TSEEKER::value_type(file, fileInfo(file, info.uncompressed_size, nSeekPos)));
            
            result = unzGoToNextFile(hFile);
        }
    }
    
    void SoraZipFile::setPackCode(const char* pstrCode) {
        packCode = pstrCode;
    }
    
    bool SoraZipFile::open(void* lpszPackData, uLong nPackSize) {
        if(isOpen()) close();
        
        m_UnzFile = unzOpenFromMemory(lpszPackData, nPackSize);
        
        if(m_UnzFile != 0) {
            _cache();
            setOpen(true);
            return true;
        }
        return false;
    }
    
    void SoraZipFile::close() {
        if(isOpen()) {
            
            if(m_UnzFile) {
                unzClose(m_UnzFile);
                m_UnzFile = 0;
            }
            
            m_Seeker.clear();
        }
    }
    
    void* SoraZipFile::getFile(const StringType& filename, uint32 readSize) {
        if(isOpen()) {
            if(readSize != 0) {
                return _getfile(filename, readSize);
            } else
                return 0;
        } else {
            return 0;
        }
        return 0;
    }
    
    void* SoraZipFile::_getfile(const StringType& filename, uLong readSize) {
        const char*	lpszPackCode = packCode.empty() ? 0 : packCode.c_str();
       
        SoraString rfilename = filename;
        if(getFilePath().size() != 0) {
            size_t packpos = rfilename.find(getFilePath());
            if(packpos != SoraWString::npos && packpos == 0) {
                rfilename.erase(0, getFilePath().size()+1);
            }
        }
        std::string sfilename = rfilename;
        for (size_t nChar = 0; nChar < sfilename.size(); ++nChar) {
            if (sfilename[nChar] == '\\')
                sfilename[nChar] = '/';
        }
        _sstrupr(sfilename);
        
        unzFile hZipFile = m_UnzFile;
        int nResult = UNZ_ERRNO;
        
        TSEEKER::iterator anItr;
        anItr = m_Seeker.find(sfilename.c_str());
        if (anItr != m_Seeker.end()) {
            nResult = unzSetOffset(hZipFile, (uLong)anItr->second.seekPos);
        }
        
        if (UNZ_OK != nResult) {
            nResult = unzGoToFirstFile(hZipFile);
        }
        
        while (UNZ_OK == nResult) {
            char szCurrentFile[512];
            unz_file_info _UnzFileInfo;
            unzGetCurrentFileInfo(hZipFile, &_UnzFileInfo, szCurrentFile, sizeof(szCurrentFile), NULL, 0, NULL, 0);
            for (size_t nChar = 0; szCurrentFile[nChar]; ++nChar) {
                if (szCurrentFile[nChar] == '\\')
                    szCurrentFile[nChar] = '/';
            }
            std::string scurrentfile = szCurrentFile;
            _sstrupr(scurrentfile);
            
            if (scurrentfile == sfilename) {
                if (UNZ_OK == unzOpenCurrentFilePassword(hZipFile, lpszPackCode)) {
                    if (readSize > _UnzFileInfo.uncompressed_size)
                        readSize = _UnzFileInfo.uncompressed_size;
                    
                    uint8* buffer = (uint8*)sora_malloc(readSize);
                    sora_assert(buffer);
                    
                    memset(buffer, 0, readSize);
                    
                    int nReadBytes = unzReadCurrentFile(hZipFile, buffer, readSize);
                    
                    unzCloseCurrentFile(hZipFile);
                    
                    if(nReadBytes == readSize)
                        return (void*)buffer;
                    else {
                        delete buffer;
                        return 0;
                    }
                }
                return 0;
            }
            
            nResult = unzGoToNextFile(hZipFile);
        }
        
        return 0;
    }
    
    uint32 SoraZipFile::getFileSize(const StringType& filename) {
        if(isOpen()) {
            std::string sFileName = filename.get().c_str();
            for (size_t nChar = 0; nChar < sFileName.size(); ++nChar) {
                if (sFileName[nChar] == '\\')
                    sFileName[nChar] = '/';
            }
            _sstrupr(sFileName);
            
            TSEEKER::iterator anItr;
            anItr = m_Seeker.find(sFileName.c_str());
            if (anItr != m_Seeker.end()) {
                return anItr->second.fileSize;
            }
        }
        return 0;
    }
    
    uint32 SoraZipFile::getFileCRC(const StringType& filename) {
        if(isOpen()) {
            unz_file_info* lpUnzFileInfo = _getUnzFileInfo(filename.c_str());
            return lpUnzFileInfo ? lpUnzFileInfo->crc : 0;
        }
        return 0;
    }
    
    unz_file_info* SoraZipFile::_getUnzFileInfo(const char* lpszFileName) {
        if (0 == m_UnzFile)
            return NULL;
        
        if (0 == lpszFileName || 0 == *lpszFileName)
            return NULL;
        
        const char*	lpszPackCode = packCode.empty() ? 0 : packCode.c_str();
        
        char szFileName[512];
        strcpy(szFileName, lpszFileName);
        for (size_t nChar = 0; szFileName[nChar]; ++nChar) {
            if (szFileName[nChar] == '\\')
                szFileName[nChar] = '/';
        }
        std::string filename = szFileName;
        _sstrupr(filename);
        
        unzFile hZipFile = m_UnzFile;
        
        int nResult = UNZ_ERRNO;
        if (0 != m_Seeker.size()) {
            TSEEKER::iterator anItr;
            anItr = m_Seeker.find(filename.c_str());
            if (anItr != m_Seeker.end()) {
                nResult = unzSetOffset(hZipFile, (uLong)anItr->second.seekPos);
            }
        }
        
        if (UNZ_OK != nResult) {
            nResult = unzGoToFirstFile(hZipFile);
        }
        
        while (UNZ_OK == nResult) {
            char szCurrentFile[512];
            unz_file_info _UnzFileInfo;
            unzGetCurrentFileInfo(hZipFile, &_UnzFileInfo, szCurrentFile, 260, NULL, 0, NULL, 0);
            for (size_t nChar = 0; szCurrentFile[nChar]; ++nChar) {
                if (szCurrentFile[nChar] == '\\')
                    szCurrentFile[nChar] = '/';
            }
            std::string cfilename = szCurrentFile;
            _sstrupr(cfilename);
            
            if (cfilename == lpszFileName) {
                if (UNZ_OK != unzOpenCurrentFilePassword(hZipFile, lpszPackCode)) {
                    return 0;
                }
                
                unzCloseCurrentFile(hZipFile);
                
                
                memcpy(&m_UnzFileInfo, &_UnzFileInfo, sizeof(_UnzFileInfo));
                return &m_UnzFileInfo;
            }
            
            nResult = unzGoToNextFile(hZipFile);
        }
        
        return 0;
    }
	
	bool SoraZipFile::enumFiles(std::vector<StringType>& cont, const StringType& folder) {
		unzFile hFile = m_UnzFile;
		int result = unzGoToFirstFile(hFile);
        
		while(result == UNZ_OK) {
			unz_file_info info;
			char szCurrentFile[512];
			unzGetCurrentFileInfo(hFile, &info, szCurrentFile, 512, NULL, 0, NULL, 0);
			
            std::string tmpFile = szCurrentFile;
            if(tmpFile.find(folder) != std::wstring::npos)
                cont.push_back(tmpFile);
            
			result = unzGoToNextFile(hFile);
		}
        return true;
	}
    
    struct zlib_memory_info {
        voidpf	buff;
        uLong	size;
    };
    
    struct zlib_memory_file : public zlib_memory_info {
        uLong	mode;
        uLong	pos;
    };
    
    zlib_memory_info* create_zlib_memory_info(voidpf buff, uLong size) {
        if (0 == buff || 0 == size)
            return 0;
        
        zlib_memory_info* zmi = new zlib_memory_info;
        if (0 != zmi) {
            memset(zmi, 0, sizeof(zlib_memory_info));
            zmi->buff	= buff;
            zmi->size	= size;
        }
        return zmi;
    }
    
    void delete_zlib_memory_info(zlib_memory_info* zmi) {
        if (0 != zmi)
            delete zmi;
    }
    
    zlib_memory_file* create_zlib_memory_file(zlib_memory_info* zmi) {
        if (0 == zmi)
            return 0;
        
        zlib_memory_file* zmf = new zlib_memory_file;
        if (0 != zmf) {
            memset(zmf, 0, sizeof(zlib_memory_file));
            zmf->buff = zmi->buff;
            zmf->size = zmi->size;
        }
        return zmf;
    }
    
    void delete_zlib_memory_file(zlib_memory_file* zmf) {
        if (0 != zmf)
            delete zmf;
    }
    
    voidpf open_file_func(voidpf opaque, const char* filename, int mode) {
        if (0 == opaque || 0 != filename)
            return 0;
        
        if (ZLIB_FILEFUNC_MODE_READ != (mode & ZLIB_FILEFUNC_MODE_READ))
            return 0;
        
        if (ZLIB_FILEFUNC_MODE_WRITE == (mode & ZLIB_FILEFUNC_MODE_WRITE))
            return 0;
        
        zlib_memory_file* zmf = create_zlib_memory_file((zlib_memory_info*)opaque);
        if (0 != zmf) {
            zmf->mode = mode;
            zmf->pos = 0;
        }
        
        return (voidpf)zmf;
    }
    
    uLong read_file_func(voidpf opaque, voidpf stream, void* buf, uLong size) {
        zlib_memory_file* zmf = (zlib_memory_file*)stream;
        if (0 != zmf) {
            if (size > (zmf->size - zmf->pos))
                size = (zmf->size - zmf->pos);
            
            memcpy(buf, (const char*)zmf->buff + zmf->pos, size);
            zmf->pos += size;
            return size;
        }
        
        return 0;
    }
    
    uLong write_file_func(voidpf opaque, voidpf stream, const void* buf, uLong size) {
        return 0;
    }
    
    long tell_file_func(voidpf opaque, voidpf stream) {
        zlib_memory_file* zmf = (zlib_memory_file*)stream;
        if (0 == zmf)
            return -1;
        return zmf->pos;
    }
    
    long seek_file_func(voidpf opaque, voidpf stream, uLong offset, int origin) {
        zlib_memory_file* zmf = (zlib_memory_file*)stream;
        if (0 == zmf)
            return -1;
        
        if (ZLIB_FILEFUNC_SEEK_CUR == origin) {
            if ((zmf->pos + offset) < 0)
                return -1;
            zmf->pos = zmf->pos + offset;
        }
        else if (ZLIB_FILEFUNC_SEEK_END == origin) {
            if (offset < 0 && (zmf->size + offset) < 0)
                return -1;
            zmf->pos = zmf->size + offset;
        }
        else if (ZLIB_FILEFUNC_SEEK_SET == origin) {
            if (offset < 0)
                return -1;
            zmf->pos = offset;
        }
        else {
            return -1;
        }
        
        return 0;
    }
    
    int close_file_func(voidpf opaque, voidpf stream) {
        delete_zlib_memory_file((zlib_memory_file*)stream);
        return 0;
    }
    
    int error_file_func(voidpf opaque, voidpf stream) {
        return 0;
    }
    
    unzFile unzOpenFromMemory(voidpf data, uLong size) {
        unzFile hZipFile = 0;
        
        zlib_memory_info* zsi = create_zlib_memory_info(data, size);
        if (0 != zsi) {
            zlib_filefunc_def zf;
            zf.zopen_file	= open_file_func;
            zf.zread_file	= read_file_func;
            zf.zwrite_file	= write_file_func;
            zf.ztell_file	= tell_file_func;
            zf.zseek_file	= seek_file_func;
            zf.zclose_file	= close_file_func;
            zf.zerror_file	= error_file_func;
            zf.opaque		= (voidpf)zsi;
            
            hZipFile = unzOpen2(0, &zf);
            
            delete_zlib_memory_info(zsi);
        }
        return hZipFile;
    }

} // namespace sora