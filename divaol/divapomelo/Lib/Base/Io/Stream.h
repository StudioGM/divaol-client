/****************************************************
 *  Stream.h
 *
 *  Created by tempbuffer on 8/12/12.
 *  Copyright 2012 tempbuffer. All rights reserved.
 *
 *  A base class that suppot stream io
 ****************************************************/

#ifndef _BASE_STREAM_H_
#define _BASE_STREAM_H_

#include "../Common.h"

namespace Base
{
	class Stream;
	typedef std::vector<byte> BinaryArray;

	enum IoFlag
	{
		Io_read = 1,
		Io_write = 2,
		Io_append = 4,
		Io_nocache = 8
	};

	enum SeekType
	{
		Seek_begin,
		Seek_pos,
		Seek_end
	};

	#define BASE_STREAM_SUPPORT_OPERATOR(_class)\
		template<typename T>\
		friend _class& operator>>(_class &stream, T &tmp) {\
			stream.readAny(tmp);\
			return stream;\
		}\
		template<typename T>\
		friend _class& operator<<(_class &stream, const T &tmp) {\
			stream.writeAny(tmp);\
			return stream;\
		}

	class Stream
	{
	public:
		virtual ~Stream() {}

		virtual bool canRead() const = 0;
		virtual bool canWrite() const = 0;
		virtual bool canSeek() const = 0;

		virtual bool seek(size_t pos) = 0;
		virtual size_t read(binary buffer, size_t length) = 0;
		virtual size_t write(const binary buffer, size_t length) = 0;

		virtual byte readByte();
		virtual void writeByte(byte data);

		template<typename T>
		size_t readAny(T &data) {return read((binary)(&data),sizeof(T));}
		template<typename T>
		size_t writeAny(const T &data) {return write((const binary)(&data),sizeof(T));}
		template<>
		size_t writeAny(const String &data);

		virtual size_t pos() const = 0;
		virtual size_t size() const = 0;
		virtual bool valid() const = 0;

		virtual void close() = 0;
		virtual void flush() = 0;
		virtual bool eof() const = 0;

		operator bool() {return !eof();}

		BASE_REGISTER_CLASS_NAME(Stream);
	};

	template<>
	size_t Stream::writeAny(const String &data)
	{
		std::string str = data.asUTF8();
		return write((binary)str.c_str(),str.size());
	}

	class FileStream : public Stream
	{
	public:
		FileStream():mFile(0),mCanwrite(false),mNocache(false),mCanread(false) {}
		FileStream(const String &filename, uint32 flag = Io_read):mFile(0){open(filename,flag);}
		virtual ~FileStream() {close();}
		virtual bool open(const String &filename, uint32 flag = 0);
		BASE_STREAM_SUPPORT_OPERATOR(FileStream);

		bool canRead() const {return mCanread;}
		bool canWrite() const {return mCanwrite;}
		bool canSeek() const {return true;}

		void close() {if(mFile)fclose(mFile); mFile = 0;}
		void flush() {if(mFile)fflush(mFile);}
		bool eof() const;

		size_t size() const;
		size_t pos() const;
		bool valid() const {return mFile!=0;}

		bool seek(size_t pos);
		bool seek(size_t pos, SeekType origin);
		size_t read(binary buffer, size_t length);
		size_t write(const binary buffer, size_t length);

		BASE_REGISTER_CLASS_NAME(FileStream);

	private:
		bool mNocache;
		bool mCanwrite;
		bool mCanread;
		FILE *mFile;
	};

	class MemoryStream : public Stream
	{
	public:
		MemoryStream();
		MemoryStream(const MemoryStream &other);
		MemoryStream(const binary data, size_t length);
		MemoryStream(size_t size, byte newByte=0) {alloc(size,newByte);}
		virtual ~MemoryStream() {}
		
		MemoryStream& operator=(const MemoryStream &other);
		byte operator[](size_t index);
		BASE_STREAM_SUPPORT_OPERATOR(MemoryStream);

		bool canRead() const {return true;}
		bool canWrite() const {return true;}
		bool canSeek() const {return true;}

		void close() { mData.clear(); }
		void flush() {}

		bool seek(size_t pos);
		size_t read(binary buffer, size_t length);
		size_t write(const binary buffer, size_t length);

		bool eof() const {return mPos==mData.size();}
		size_t size() const {return mData.size();}
		size_t capacity() const {return mData.capacity();}
		size_t pos() const {return mPos;}
		bool valid() const {return !mData.empty();}

		void append(size_t size, byte newByte=0);
		void alloc(size_t size, byte newByte=0);
		void resize(size_t size, byte newByte=0);
		binary data();
		binary dataOf(size_t offset);

		BASE_REGISTER_CLASS_NAME(MemoryStream);

	private:
		size_t mPos;
		BinaryArray mData;
	};

	class BufferStream : public Stream
	{
	public:
		BufferStream(StreamPtr streamPtr):mStreamPtr(streamPtr) {}
		BufferStream(StreamPtr streamPtr, size_t size)
			:mStreamPtr(streamPtr),mBuffer(size) {}
		~BufferStream() {}

		BASE_STREAM_SUPPORT_OPERATOR(BufferStream);

		bool canRead() const {return mStreamPtr->canRead();}
		bool canWrite() const {return mStreamPtr->canWrite();}
		bool canSeek() const {return mStreamPtr->canSeek();}

		bool seek(size_t pos) {return mStreamPtr->seek(pos);}
		size_t read(binary buffer, size_t length) {return mStreamPtr->read(buffer,length);}
		size_t write(const binary buffer, size_t length);

		virtual size_t pos() const {return mStreamPtr->pos();}
		virtual size_t size() const {return mStreamPtr->size();}
		virtual bool valid() const {return mStreamPtr->valid();}

		virtual void close() {mStreamPtr->close();}
		virtual void flush();
		virtual bool eof() const {return mStreamPtr->eof();}

		BASE_REGISTER_CLASS_NAME(BufferStream);

	private:
		StreamPtr mStreamPtr;
		BinaryArray mBuffer;
	};
}

#endif