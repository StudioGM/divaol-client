/*
 *  SoraMemoryBuffer.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 6/4/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */


#include "SoraMemoryBuffer.h"
#include "io/SoraFileStream.h"

namespace sora {
	
	SoraMemoryBuffer::SoraMemoryBuffer(): 
    length(0), 
    currPos(0), 
    realSize(0),
    mInternal(false) {
        
    }
    
	SoraMemoryBuffer::SoraMemoryBuffer(void* pData, uint32 _length):
	currPos(0), 
    length(_length),
    mInternal(false) {
		apCData = (uint8*)pData;
	}
	
	SoraMemoryBuffer::~SoraMemoryBuffer() {
        if(mInternal && apCData)
            delete apCData;
    }
    
    void SoraMemoryBuffer::release() {
        if(apCData)
            delete apCData;
    }
	
	void SoraMemoryBuffer::set(void* pData, uint32 _length) {
        if(mInternal && apCData)
            delete apCData;
        
		currPos = 0;
		length = _length;
		apCData = (uint8*)pData;
	}
	
	bool SoraMemoryBuffer::alloc(uint32 size) {
		/* round size to be divisible by 16 */
		//	size = (size + 15) & uint32(-16);
		uint8* data = new uint8[size];
		if(!data) return false;
		memset(data, 0, length);
		apCData = data;
		length = size;
		seek(0);
        mInternal = true;
		return true;
	}
	
	void SoraMemoryBuffer::resize() {
		if(currPos != 0) {
			uint8* tmpData = new uint8[realSize];
			memcpy(tmpData, (void*)(get()), realSize);
			length = realSize;
			
			apCData = tmpData;
		}
	}
	
	bool SoraMemoryBuffer::push(void* pdata, uint32 size) {
		if(realSize + size <= length) {
			memcpy((void*)(get()+realSize), pdata, size);
			realSize += size;
			return true;
		} else {
			// buffer not enough, reallocate
			
			// save current data
			if(realSize != 0) {
				uint8* tempData = new uint8[realSize];
				memcpy(tempData, (void*)(get()), realSize);
				
				// rellocate data
				uint32 prevLength = realSize;
				length = length*2 + 1024;
				alloc(length);
				memcpy((void*)(get()), tempData, prevLength);
				realSize = prevLength;
				
				delete tempData;
				return push(pdata, size);
			} else {
				alloc(size);
				memcpy((void*)(get()), pdata, size);
				realSize = size;
				return true;
			}
		}
		return false;
	}
	
	bool SoraMemoryBuffer::writeToFile(const StringType& path) {
		SoraFileStream stream;
        if(stream.open(path.get(), true)) {
            stream.write((void*)get(), realSize);
            stream.close();
            return true;
        }
        return true;
	}
	
	uint32 SoraMemoryBuffer::read(void* pv, uint32 size) {
		if(!valid()) return 0;
		if(currPos == length) return 0;
		
		assert(pv != NULL);
		
		if(currPos+size > length) {
			size = length - currPos;
		}
		memcpy(pv, (void*)(get()+currPos), size);
		currPos += size;
		return size;
	}
	
	uint32 SoraMemoryBuffer::read(uint32 offset, void* pv, uint32 size) {
		if(!valid()) return false;
		
		uint32 tempPos = currPos;
		if(!seek(offset))
			seek(0);
		uint32 result = read(pv, size);
		seek(tempPos);
		return result;
	}
	
	uint8* SoraMemoryBuffer::get() { 
        return apCData;
    }
    
	uint8* SoraMemoryBuffer::get(uint32 offset) { 
		if(offset > length) offset = 0;
		return (apCData+offset);
	}
	
	bool SoraMemoryBuffer::seek(uint32 pos) {
		if(pos <= length) {
			currPos = pos;
			return true;
		}
		return false;
	}
	
	uint32 SoraMemoryBuffer::size() const { 
        return length;
    }
    
	uint32 SoraMemoryBuffer::realsize() const { 
        return realSize;
    }
    
	uint32 SoraMemoryBuffer::offset() const { 
        return currPos; 
    }
    
	bool SoraMemoryBuffer::valid() const { 
        return apCData != NULL;
    }

} // namespace sora