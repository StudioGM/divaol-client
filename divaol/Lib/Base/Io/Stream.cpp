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
	/*
	 * Stream
	 */

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

	/*
	 * FileStream
	 */

	bool FileStream::open(const String &filename, uint32 flag)
	{
		if(mFile != NULL)
			close();
		if((Io_write&flag)==0&&(Io_read&flag)==0)
			flag |= Io_read;

		this->mNocache = (flag&Io_nocache)>0;
		this->mCanwrite = (flag&Io_write)>0;
		this->mCanread = (flag&Io_read)>0;
#ifndef BASE_OS_WINDOWS
		if(mCanread)
			mFile = fopen(filename.ansi_str(), mCanwrite ? ((flag&Io_append)>0 ? L"a+b" : L"r+b") : L"rb");
		else
			mFile = fopen(filename.ansi_str(), mCanwrite ? ((flag&Io_append)>0 ? L"ab" : L"wb") : L"");
#else
		if(mCanread)
			mFile = _wfopen(filename.unicode_str(), mCanwrite ? ((flag&Io_append)>0 ? L"a+b" : L"r+b") : L"rb");
		else
			mFile = _wfopen(filename.unicode_str(), mCanwrite ? ((flag&Io_append)>0 ? L"ab" : L"wb") : L"");
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

	/*
	 * MemoryStream
	 */
	MemoryStream::MemoryStream()
		:mPos(0) {}
	MemoryStream::MemoryStream(const MemoryStream &other)
		:mPos(0),mData(other.mData) {}
	MemoryStream::MemoryStream(const binary data, size_t length)
		:mPos(0) {
		mData = BinaryArray(length);
		memcpy(&mData[0],data,sizeof(byte)*length);
	}
	MemoryStream& MemoryStream::operator=(const MemoryStream &other)
	{
		if(this!=&other) {
			mPos = other.mPos;
			mData = other.mData;
		}
		return *this;
	}
	byte MemoryStream::operator[](size_t index) {
		base_assert(index < size());
		return mData[index];
	}

	bool MemoryStream::seek(size_t pos)
	{
		 if(pos>size())
			 return false;
		 mPos = pos;
		 return true;
	}

	size_t MemoryStream::read(binary buffer, size_t length)
	{
		if(!valid()||mPos==size())
			return 0;

		base_assert(buffer!=0);

		if(mPos+length>size())
			length = size()-mPos;

		memcpy(buffer,static_cast<void*>(data()+mPos),length);

		mPos += length;

		return length;
	}
	size_t MemoryStream::write(const binary buffer, size_t length)
	{
		if(mPos+length>size())
			append(mPos+length-size());
		
		memcpy(static_cast<void*>(data()+mPos),buffer,sizeof(byte)*length);
		mPos += length;

		return length;
	}
	void MemoryStream::append(size_t size, byte newByte)
	{
		mData.insert(mData.end(),size,newByte);
	}
	void MemoryStream::alloc(size_t size, byte newByte)
	{
		close();
		append(size, newByte);
		seek(0);
	}
	void MemoryStream::resize(size_t size, byte newByte)
	{
		if(size<this->size())
			mData.resize(size);
		else if(size>this->size())
			append(size-this->size(),newByte);
	}
	binary MemoryStream::data()
	{
		if(valid())
			return &mData[0];
		else
			return 0;
	}
	binary MemoryStream::dataOf(size_t offset)
	{
		if(offset>size())
			return 0;
		return data()+offset;
	}

	/*
	 * BufferStream
	 */
	size_t BufferStream::write(const binary buffer, size_t length)
	{
		size_t pos = mBuffer.size();
		mBuffer.insert(mBuffer.end(),length,0);
		memcpy(&mBuffer[pos],buffer,sizeof(byte)*length);
		return length;
	}

	void BufferStream::flush()
	{
		if(mStreamPtr->valid()&&mStreamPtr->canWrite()&&!mBuffer.empty())
		{
			mStreamPtr->write(&mBuffer[0],mBuffer.size());
			mBuffer.clear();
			mStreamPtr->flush();
		}
	}
}