/*
 *  SoraResourceFile.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 6/28/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "SoraResourceFile.h"
#include "SoraCore.h"
#include "SoraLogger.h"

namespace sora {
    
    SoraResourceFile::SoraResourceFile():
    SoraResource(SoraResource::RawData),
    mData(NULL),
    mSize(0),
    mRetain(false) {
        
    }

	SoraResourceFile::SoraResourceFile(void* data, uint32 size, bool retain):
    SoraResource(SoraResource::RawData),
	mData(data),
	mSize(size),
	mRetain(retain) {
	}
	
	SoraResourceFile::SoraResourceFile(const StringType& file, bool retain):
    SoraResource(SoraResource::RawData),
	mRetain(retain) {
		mData = SoraCore::Ptr->getResourceFile(file, mSize);
		if(!mData)
			log_error(vamssg("SoraResourceFile: Error loading resource file: %s", file.c_str()));
	}
    
    SoraResourceFile::SoraResourceFile(const StringType& file, uint32 pos, uint32 size, bool retain):
    SoraResource(SoraResource::RawData),
    mRetain(retain) {
        mData = SoraCore::Ptr->readResourceFile(file, pos, size);
        if(!mData)
			log_error(vamssg("SoraResourceFile: Error loading resource file: %s", file.c_str()));
    }
	
	SoraResourceFile::~SoraResourceFile() {
		if(mData && !mRetain) {
			SoraCore::Ptr->freeResourceFile(mData);
		}
	}
    
    SoraResourceFile& SoraResourceFile::operator=(const StringType& file) {
        mData = SoraCore::Ptr->getResourceFile(file, mSize);
		if(!mData)
			log_error(vamssg("SoraResourceFile: Error loading resource file: %s", file.c_str()));
        mRetain = false;
		return *this;
    }
    
    void* SoraResourceFile::data() const {
        return mData;
    }
    
    uint32 SoraResourceFile::size() const {
        return mSize;
    }
	
	bool SoraResourceFile::isValid() const {
		return mData!=0;
	}
	
	SoraResourceFile::operator void*() {
		return mData;
	}
	
	SoraResourceFile::operator char*() {
		return static_cast<char*>(mData);
	}
	
	SoraResourceFile::operator int*() {
		return static_cast<int*>(mData);
	}
	
	SoraResourceFile::operator long*() {
		return static_cast<long*>(mData);
	}
	
} // namespace sora