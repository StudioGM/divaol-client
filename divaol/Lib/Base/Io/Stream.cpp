/****************************************************
 *  Stream.cpp
 *
 *  Created by tempbuffer on 8/12/12.
 *  Copyright 2012 tempbuffer. All rights reserved.
 *
 *  A base class that suppot stream io
 ****************************************************/

#include "Stream.h"

namespace Base
{
	byte Stream::readByte() {
		if(!canRead()) {
			BASE_THROW_EXCEPTION("cannot read byte because the stream cannot read");
		}
		byte buffer;
		this->read(&buffer, 1);
		return buffer;
	}

	void Stream::writeByte(byte data) {
		if(!canRead()) {
			BASE_THROW_EXCEPTION("cannot write byte because the stream cannot write");
		}
		this->write(&data, 1);
	}

	bool FileStream::open(const String &filename, uint32 flag)
	{
		if(mFile != NULL)
			close();

		this->mNocache = (flag&Io_nocache)>0;
		this->mCanwrite = (flag&Io_write)>0;
#ifndef BASE_OS_WINDOWS
		mFile = fopen(filename.ansi_str(), mCanwrite ? ((flag&Io_append)>0 ? "a+b" : "r+b") : "rb");
#else
		mFile = _wfopen(filename.unicode_str(), mCanwrite ? ((flag&Io_append)>0 ? L"a+b" : L"r+b") : L"rb");
#endif
		if(mFile == NULL) 
		{
			if(mCanwrite)
#ifndef BASE_OS_WINDOWS
				mFile = fopen(filename.ansi_str(), "w+b");
#else
				mFile = _wfopen(filename.unicode_str(), L"w+b");
#endif
			if(mFile == NULL)
				return false;
		}
		return true;
	}

	bool FileStream::eof() const {
		if(mFile)
			return feof(mFile)!=0?true:false;
		return true;
	}

	size_t FileStream::pos() const {
		if(mFile) 
			return ftell(mFile); 
		return 0;
	}

	size_t FileStream::size() const {
		if(!mFile)
			return 0;
		size_t pos = ftell(mFile);
		fseek(mFile, 0, SEEK_END);
		size_t size = ftell(mFile);
		fseek(mFile, pos, SEEK_SET);
		return size;
	}

	bool FileStream::seek(size_t pos) 
	{
		if(mFile != 0)
			return fseek(mFile, pos, SEEK_SET) == 0 ? true : false;
		return false;
	}

	bool FileStream::seek(size_t pos, SeekType origin) 
	{
		if(mFile != 0)
			return fseek(mFile, pos, origin) == 0 ? true : false;
		return false;
	}

	size_t FileStream::read(binary buffer, size_t length)
	{
		if(mFile != 0)
			return fread(buffer, 1, length, mFile);
		return 0;
	}
	
	size_t FileStream::write(const binary buffer, size_t length)
	{
		if(mFile == 0)
			return 0;

		if(mNocache) 
			return fwrite(buffer, length, 1, mFile);
		else 
		{
			size_t size = fwrite(buffer, length, 1, mFile);
			fflush(mFile);
			return size;
		}
	}
}