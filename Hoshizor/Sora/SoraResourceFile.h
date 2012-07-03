/*
 *  SoraResourceFile.h
 *  Sora
 *
 *  Created by Griffin Bu on 6/28/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef SORA_RESOURCE_FILE_H_
#define SORA_RESOURCE_FILE_H_

#include "SoraPlatform.h"
#include "SoraUncopyable.h"
#include "SoraString.h"
#include "SoraResource.h"

namespace sora {
	
	/**
	 *	a helper class that can release the resource file when it deallocs
	 *	for situation that the user may forget to free the resource data and cause memory leaks
	 *	this class can also request for a resource file directly
	 **/
	
	class SORA_API SoraResourceFile: public SoraResource {
	public:
        SoraResourceFile();
		explicit SoraResourceFile(const StringType& file, bool retain=false);
        
		SoraResourceFile(void* data, uint32 size, bool retain=false);
        SoraResourceFile(const StringType& file, uint32 pos, uint32 size, bool retain=false);
		~SoraResourceFile();
        
        SoraResourceFile& operator=(const StringType& file);
		
        void*   data() const;
        uint32  size() const;
		
		operator void*();
		operator char*();
		operator int*();
		operator long*();
		
		bool isValid() const;
		
	private:
		void*   mData;
		uint32  mSize;
		bool    mRetain;
    };
	
} // namespace sora

#endif // SORA_RESOURCE_FILE_H_