#ifndef SORA_ZIP_FILE
#define SORA_ZIP_FILE

#include "SoraStringConv.h"
#include "SoraString.h"
#include "ZLIB/unzip.h"
#include "io/SoraFileBase.h"

#include <map>

namespace sora {

	class SoraZipFile: public SoraFileBase {
	public:
		SoraZipFile();
		SoraZipFile(const StringType& filePath);
		~SoraZipFile();

        bool open(const StringType& filePath);
        bool open(void* lpszPackData, uLong nPackSize);
        void close();
	
        void setPackCode(const char* lpszPackCode);

        void* getFile(const StringType& filename, uint32 readsize);

        uint32 getFileSize(const StringType& filename);
        uint32 getFileCRC(const StringType& filename);
		
        bool enumFiles(std::vector<StringType>& cont, const StringType& folder);

	private:
		void _releaseFile();
		void* _getfile(const StringType& filename, uLong readsize);
        void _cache();
        unz_file_info* _getUnzFileInfo(const char* lpszFileName);
        
		std::string packCode;
        
        unzFile m_UnzFile;		// modeRead
        unz_file_info	m_UnzFileInfo;	
        
        struct fileInfo {
            std::string fileName;
            uLong fileSize;
            uLong seekPos;
            
            fileInfo(const std::string& _fileName, uLong _fileSize, uLong _seekPos): fileName(_fileName), fileSize(_fileSize), seekPos(_seekPos) {}
        };
        
        typedef std::map<std::string, fileInfo> TSEEKER; 
        TSEEKER m_Seeker;
	};
} //namespace sora

#endif