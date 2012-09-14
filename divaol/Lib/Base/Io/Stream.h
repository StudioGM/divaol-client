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

	class Stream
	{
	public:
		virtual ~Stream() {}

		virtual bool canRead() const = 0;
		virtual bool canWrite() const = 0;
		virtual bool canSeek() const = 0;

		virtual bool seek(size_t pos) = 0;
		virtual size_t read(binary buffer, size_t length) = 0;
		virtual size_t write(const binary bufer, size_t length) = 0;

		virtual byte readByte();
		virtual void writeByte(byte data);

		virtual size_t pos() const = 0;
		virtual size_t size() const = 0;
		virtual bool valid() const = 0;

		virtual void close() = 0;
		virtual void flush() = 0;
		virtual bool eof() const = 0;

		operator bool() {return !eof();}

		BASE_REGISTER_CLASS_NAME(Stream);
	};

	class FileStream
	{
	public:
		FileStream():mFile(0),mCanwrite(false),mNocache(false) {}
		FileStream(const String &filename, uint32 flag = 0):mFile(0){open(filename,flag);}
		virtual ~FileStream() {close();}
		virtual bool open(const String &filename, uint32 flag = 0);

		bool canRead() const {return true;}
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
		size_t write(const binary bufer, size_t length);

		BASE_REGISTER_CLASS_NAME(FileStream);

	private:
		bool mNocache;
		bool mCanwrite;
		FILE *mFile;
	};
}

#endif