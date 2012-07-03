/*
 *  SoraFolderResourceManager.h
 *  Sora
 *
 *  Created by GriffinBu on 1/12/11.
 *  Copyright 2011 Griffin Bu(Project L). All rights reserved.
 *
 */

#ifndef SORA_FOLDER_RESOURCE_MANAGER_H_
#define SORA_FOLDER_RESOURCE_MANAGER_H_

#include "SoraResourceManager.h"
#include "SoraFileUtility.h"
#include "SoraStringId.h"
#include "SoraLogger.h"
#include "SoraPath.h"
#include "util/SoraHash.h"

#include <map>

namespace sora {
	
	class SORA_API SoraFolderResourceManager: public SoraResourceManager {
	public:
		SoraFolderResourceManager();
		~SoraFolderResourceManager() {}
		
		SoraResourceHandle	loadResourcePack	(const StringType& folder);
		void                attachResourcePack  (SoraResourceHandle handle);
		void                detachResourcePack  (SoraResourceHandle handle);
		
		void*   readResourceFile(const StringType& file, uint32 pos, uint32 size);
		void*   getResourceFile(const StringType& file, uint32& size);
		uint32  getResourceFileSize(const StringType& file);
		void    freeResourceFile(void* p);
		
		StringType getName() const		{ return "FolderRM"; }
		bool isFormatSupported(const StringType& format) const { return format.size() == 0; }
		
		bool enumFiles(std::vector<StringType>& cont, const StringType& folder);
	
    private:
		struct folderDescription {
			SoraPath folderName;
			SoraStringId folderHash;
			folderDescription(const StringType& folder): folderName(folder.get()), folderHash(crc32(folder.get())) {
                folderName.makeDirectory();
            }
		};
		typedef std::vector<folderDescription> FOLDER_CONT;
		FOLDER_CONT folders;
		StringType applicationPath;
		
		typedef std::map</*fileNameHash*/StringType, /*fileFullPath*/StringType> FILE_CACHE;
		FILE_CACHE fileCache;
		
		inline StringType getFullPath(const StringType& fileName);
	};

	
} // namespace sora

#endif