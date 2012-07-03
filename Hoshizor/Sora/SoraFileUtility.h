#ifndef SORA_FILE_UTILITY_H
#define SORA_FILE_UTILITY_H

#include "SoraPlatform.h"
#include "SoraString.h"
#include <fstream>

namespace sora {

	class SORA_API SoraFileUtility {
	public:
		static bool FileExists(const StringType& filepath);
		
		static StringType GetAbsolutePath(const StringType& filePath);
		
		/* 
		 get path in which can use to write file
		 specified for ios,
		 because ios limit writting files in app
		 also some part of osx
		 */
		static StringType GetWrittablePath(const StringType& filePath);
		
		static StringType GetApplicationPath();

		static StringType GetFontPath(const StringType& file);

		static StringType GetFullPath(const StringType& file);
		
		static StringType GetFileName(const StringType& file);
    };

} // namespace sora

#endif