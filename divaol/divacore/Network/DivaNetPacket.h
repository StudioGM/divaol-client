/*
 *  DivaNetPacket.h
 *
 *  Created by Hyf042 on 3/4/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#ifndef DIVA_NET_PACKET
#define DIVA_NET_PACKET

namespace divacore
{
	namespace network
	{
		class Packet
		{
			typedef unsigned char byte;
			typedef short int int16;
			typedef int int32;
			typedef unsigned int uint32;

			static const uint32 MAX_BUFFER_SIZE = 1024;
			uint32 pos;
		public:
			//id of packet
			uint32 id;
			//std::string tag;
			float time;
			//ARGUMENTS arg;
			//byte* buffer;
			std::vector<byte> buffer;

			Packet(const char* buffer, uint32 size) 
			{
				cleanup();
				write(buffer,size);
				reset();
				id = readInt32();
				time = readFloat();
			}
			Packet(uint32 id, float time = 0):id(id),time(time) 
			{
				cleanup();
				write(id);
				write(time);
			}
			uint32 getID() {return id;}
			void setID(byte id)
			{
				if(size()>=sizeof(byte))
					memcpy(&buffer[0],&id,sizeof(byte));
			}
			float getTime() {return time;}

			inline void append(uint32 size)
			{
				buffer.insert(buffer.end(),size,0);
			}
			inline byte* getPointer() {if(size()) return &buffer[0]; return NULL;}

			inline uint32 size() {return buffer.size();}
			inline void cleanup() {buffer.clear();reset();}
			inline void reset() {pos = 0;}
			inline void wTOr() {
				reset();
				id = readInt32();
				time = readFloat();}
			template<typename T>
			T read()
			{
				T value;
				if(pos+sizeof(T)<=size())
					memcpy(&value,&buffer[pos],sizeof(T)), pos += sizeof(T);
				return value;
			}
			byte readByte() 
			{
				return read<byte>();
			}
			int16 readInt16() 
			{
				return read<int16>();
			}
			int32 readInt32() 
			{
				return read<int32>();
			}
			float readFloat() 
			{
				return read<float>();
			}
			double readDouble() 
			{
				return read<double>();
			}
			bool readBool() 
			{
				return read<bool>();
			}
			std::string readString()
			{
				std::string value = "";
				int size = readInt32();
				for(int i = 0; i < size; i++)
					value += buffer[pos], pos++;
				return value;
			}
			template<typename T>
			void write(const T&value)
			{
				append(sizeof(T));
				memcpy(&buffer[pos],&value,sizeof(T)), pos += sizeof(T);
			}
			template<>
			void write(const std::string&value)
			{
				write(value.size());
				if(value.size())
				{
					append(value.size());
					memcpy(&buffer[pos],value.c_str(),value.size()), pos += value.size();
				}
			}
			void write(const char *data, uint32 size)
			{
				if(size==0)
					return;
				append(size);
				memcpy(&buffer[pos],data,size), pos += size;
			}
			void write(const char* format, va_list	ArgPtr)
			{
				//vsprintf(Message, format, ArgPtr);

				for(;*format;++format)
				{
					if(*format!='%')
						write(*format);
					else
					{
						++format;
						if('\0'==*format)
							break;
						switch(*format)
						{
						case 'd':
							{
								int tmp = (int)va_arg(ArgPtr,int);
								write(tmp);
							}
							break;
						case 's':
							write((std::string)va_arg(ArgPtr,char*));
							break;
						case 'f':
							write((double)va_arg(ArgPtr,double));
							break;
						case 'c':
							write((char)va_arg(ArgPtr,char));
							break;
						case 'b':
							write((bool)va_arg(ArgPtr,bool));
							break;
						}
					}
				}

				//print(Message,strlen(Message));
			}
			void write(const char* format, ...)
			{
				va_list	ArgPtr;

				va_start(ArgPtr, format);
				//vsprintf(Message, format, ArgPtr);

				write(format,ArgPtr);

				va_end(ArgPtr);


				//print(Message,strlen(Message));
			}
		};
	}
}

#endif