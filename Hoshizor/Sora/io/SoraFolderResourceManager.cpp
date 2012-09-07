/*
 *  SoraFolderResourceManager.cpp
 *  Sora
 *
 *  Created by GriffinBu on 1/12/11.
 *  Copyright 2011 Griffin Bu(Project L). All rights reserved.
 *
 */

#include "SoraFolderResourceManager.h"
#include "SoraFileUtility.h"
#include "SoraPath.h"
#include "SoraFileStream.h"
#include "SoraDirectoryIterator.h"
#include "SoraLogger.h"

namespace sora {
    
    SoraFolderResourceManager::SoraFolderResourceManager(): applicationPath(SoraFileUtility::GetApplicationPath()) {
#ifdef OS_IOS
        loadResourcePack(SoraPath::writtable());
        loadResourcePack(SoraPath::resource());
#else
        loadResourcePack(SoraPath::resource());
#endif
        loadResourcePack(SoraPath::font());
#ifdef OS_OSX
        loadResourcePack(L"./");
#endif
    }

	SoraResourceHandle SoraFolderResourceManager::loadResourcePack(const StringType& folder) {
		folders.push_back(folderDescription(folder));
        return (SoraResourceHandle)folders.back().folderHash;
	}

	void SoraFolderResourceManager::attachResourcePack(SoraResourceHandle handle) {
		// for compability with old version
	}

	void SoraFolderResourceManager::detachResourcePack(SoraResourceHandle handle) {
		for(size_t i=0; i<folders.size(); ++i) {
			//modify by tempbuffer, here SoraHandle is uint32, so I have to convert it, maybe it will make an error
			if((SoraResourceHandle)folders[i].folderHash == handle) {
				folders.erase(folders.begin()+i);
				break;
			}
		}
	}

	StringType SoraFolderResourceManager::getFullPath(const StringType& fileName) {
		FILE_CACHE::iterator itFile = fileCache.find(fileName);
		if(itFile != fileCache.end()) {
			return itFile->second;
		}

        if(SoraFileUtility::FileExists(fileName))
            return fileName;
		

		for(size_t i=0; i<folders.size(); ++i) {
			StringType fullPath = folders[i].folderName.toString() + fileName;
			if(SoraFileUtility::FileExists(fullPath))
                return fullPath;
		}
        return StringType();
	}

	void* SoraFolderResourceManager::readResourceFile(const StringType& file, uint32 pos, uint32 size) {
		SoraFileStream stream;
        if(stream.open(getFullPath(file).get())) {
            uint8* pdata = (uint8*)sora_malloc((uint32)stream.size()+1);
			if(pdata != NULL) {
                sora_assert(pos + size <= stream.size());
                
                stream.pos(pos, SoraFileStream::Begin);
                if(stream.read(pdata, size) != size) {
                    sora_free(pdata);
                    return 0;
                }
                pdata[size] = 0;
                return (void*)pdata;
			}
		}
		return 0;
	}

	void* SoraFolderResourceManager::getResourceFile(const StringType& file, uint32& size) {
		SoraFileStream stream;
        if(stream.open(getFullPath(file).get())) {
            uint8* pdata = (uint8*)sora_malloc((uint32)stream.size()+1);
			if(pdata != NULL) {
                size = stream.read(pdata, stream.size());
                pdata[size] = 0;
				return (void*)pdata;
			}
		}
		return 0;
	}

	uint32 SoraFolderResourceManager::getResourceFileSize(const StringType& file) {
		SoraFileStream stream;
        if(stream.open(getFullPath(file).get())) {
            return (uint32)stream.size();
        }
        return 0;
	}

	void SoraFolderResourceManager::freeResourceFile(void* p) {
		if(p) {
            sora_free(p);
        }
	}
	
	bool SoraFolderResourceManager::enumFiles(std::vector<StringType>& cont, const StringType& folder) {
        SoraDirectoryIterator itDir(folder.get());
        while(!itDir.isEnd()) {
            cont.push_back(itDir.path().toString());
            ++itDir;
        }
		return true;
	}
	
} // namespace sora
