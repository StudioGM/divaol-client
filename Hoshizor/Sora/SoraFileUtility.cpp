#include "SoraFileUtility.h"
#include "SoraStringConv.h"

#ifdef OS_WIN32
#include <Windows.h>
 #include "Shlwapi.h"
 #pragma comment(lib,"shlwapi.lib")
#endif

#ifdef OS_OSX
#include "SoraOSXFileUtility.h"
#endif

#ifdef OS_IOS
#include "soraiOSFileUtility.h"
#endif

#include "SoraPath.h"


namespace sora {

    bool SoraFileUtility::FileExists(const StringType& filepath) {
#ifdef WIN32
			return PathFileExistsA(filepath.c_str())?true:false;
#elif defined(OS_OSX)
			return osxFileExists(filepath);
#elif defined(OS_IOS)
			return appFileExists(filepath);
#endif
			std::ifstream file;
			file.open(filepath.c_str());
			if(!file.is_open()) return false;
			file.close();
			return true;
		}
		
	 StringType SoraFileUtility::GetAbsolutePath(const StringType& filePath) {
#ifdef WIN32
			wchar_t buffer[MAX_PATH];
			GetCurrentDirectoryW(MAX_PATH, buffer);
			
			return SoraWString(buffer)+L"/"+filePath;
#elif defined(OS_OSX)
            return std::string(osxApplicationPath()+"/")+filePath;
#elif defined(OS_IOS)
			return appApplicationPath()+L"/"+filePath;
#endif
			return "./"+filePath;
		}
		
		/* 
		 get path in which can use to write file
		 specified for ios,
		 because ios limit writting files in app
		 also some part of osx
		 */
	 StringType SoraFileUtility::GetWrittablePath(const StringType& filePath) {
#ifndef OS_IOS
			return GetAbsolutePath(filePath);
#else
			return appDocumentPath()+"/"+filePath;
#endif
			return "\0";
		}
		
	 StringType SoraFileUtility::GetApplicationPath() {
#ifdef WIN32
			wchar_t buffer[MAX_PATH];
			GetCurrentDirectoryW(MAX_PATH, buffer);
			
			return SoraWString(buffer)+L"/";
#elif defined(OS_OSX)
			return osxApplicationPath()+"/";
#elif defined(OS_IOS)
			return appResourcePath()+"/";
#endif
			return "./";
		}

	 StringType SoraFileUtility::GetFontPath(const StringType& file) {
         StringType fontPath = SoraPath::font() + "/";
         return fontPath + file;
     }

	 StringType SoraFileUtility::GetFullPath(const StringType& file) {
         StringType str = GetApplicationPath();
         str += std::string("/" + file);
         return str;
     }
		
	 StringType SoraFileUtility::GetFileName(const StringType& file) {
			SoraString op = file;
			for(size_t i=0; i<op.size(); ++i)
				if(op[i] == '\\')
					op[i] = '/';
			size_t rspos = op.rfind("/");
			if(rspos != SoraString::npos)
				op.erase(0, rspos+1);
			return op;
		}

} // namespace sora