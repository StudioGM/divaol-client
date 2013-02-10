/*
 *  gnet.cpp
 *
 *  Created by tempbuffer on 8/7/12.
 *  Copyright 2012 tempbuffer. All rights reserved.
 *
 */

#include "gnet.h"

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include <arpa/inet.h>
#endif

namespace gnet
{
	namespace BinaryUtility
	{
		Bytes operator+(const Bytes &a, const Bytes &b)
		{
			Bytes ret;
			for(Bytes::const_iterator ptr = a.begin(); ptr != a.end(); ptr++)
				ret.push_back(*ptr);
			for(Bytes::const_iterator ptr = b.begin(); ptr != b.end(); ptr++)
				ret.push_back(*ptr);
			return ret;
		}

		// convertToBytes series	
		Bytes convertToBytes(uint32 data)
		{
			Bytes bytes(sizeof(uint32));
			dword tmp = htonl(data);
			memcpy((void*)(&bytes[0]),(void*)(&tmp),sizeof(uint32));
			return bytes;
		}
		Bytes convertToBytes(int32 data)
		{
			return convertToBytes(static_cast<uint32>(data));
		}
		Bytes convertToBytes(uint64 data)
		{
			return convertToBytes(static_cast<dword>(data>>32))+convertToBytes(static_cast<dword>(data&(~0)));
		}
		Bytes convertToBytes(int64 data)
		{
			return convertToBytes(static_cast<uint64>(data));
		} 
		Bytes convertToBytes(uint8 data)
		{
			Bytes ret(sizeof(uint8));
			memcpy((void*)(&ret[0]),(void*)(&data),sizeof(uint8));
			return ret;
		}
		/*Bytes convertToBytes(char data)
		{
			return convertToBytes(static_cast<uint8>(data));
		}*/
		Bytes convertToBytes(int8 data)
		{
			return convertToBytes(static_cast<uint8>(data));
		}
		Bytes convertToBytes(const Binary &data)
		{
			Bytes bytes(data.size());
			memcpy(&bytes[0],data.c_str(),data.size());
			return convertToBytes(data.size())+bytes;
		}
		Bytes convertToBytes(const Bytes &data)
		{
			return convertToBytes(data.size())+data;
		}
		Bytes convertToBytes(double data)
		{
			uint64 tmp;
			memcpy(&tmp,&data,sizeof(double));
			return convertToBytes(tmp);
		}

		// convertToHost series
		template<typename T>
		T convertToHost(Bytes &bytes)
		{
			throw "Unknown Type";
		}
		template<>
		uint8 convertToHost<uint8>(Bytes &bytes)
		{
			return static_cast<uint8>(bytes[0]);
		}
		template<>
		int8 convertToHost<int8>(Bytes &bytes)
		{
			return static_cast<int8>(bytes[0]);
		}
		template<>
		uint32 convertToHost<uint32>(Bytes &bytes)
		{
			uint32 tmp;
			memcpy(&tmp,&bytes[0],sizeof(uint32));
			return ntohl(tmp);
		}
		template<>
		int32 convertToHost<int32>(Bytes &bytes)
		{
			return static_cast<int32>(convertToHost<uint32>(bytes));
		}
		template<>
		uint64 convertToHost<uint64>(Bytes &bytes)
		{
			return ((uint64)ntohl(*((uint32*)(&bytes[0])))<<32) + (uint64)ntohl(*((uint32*)(&bytes[4])));
		}
		template<>
		int64 convertToHost<int64>(Bytes &bytes)
		{
			return static_cast<int64>(convertToHost<uint64>(bytes));
		}
		template<>
		std::string convertToHost<Binary>(Bytes &bytes)
		{
			std::string ret;
			size_t size = convertToHost<size_t>(bytes);
			for(Bytes::const_iterator ptr = bytes.begin()+sizeof(size_t); ptr != bytes.end(); ptr++)
				ret += (char)(*ptr);
			return ret;
		}
		template<>
		double convertToHost<double>(Bytes &bytes)
		{
			uint64 tmp = (convertToHost<uint64>(bytes));
			double ret;
			memcpy(&ret,&tmp,sizeof(double));
			return ret;
		}
		template<>
		Bytes convertToHost<Bytes>(Bytes &bytes)
		{
			Bytes ret;
			size_t size = convertToHost<size_t>(bytes);
			for(Bytes::const_iterator ptr = bytes.begin()+sizeof(size_t); ptr != bytes.end(); ptr++)
				ret.push_back(*ptr);
			return ret;
		}
	};

	std::string ItemBase::getHex()
	{
		std::string ret = "";
		char tmpBuffer[10];
		Bytes bytes = getItem();
		for(Bytes::iterator ptr = bytes.begin(); ptr != bytes.end(); ptr++)
		{
			sprintf_s(tmpBuffer,"%02x",uint32(*ptr));
			ret += tmpBuffer;
		}
		return ret;
	}

	uint64 ItemBase::getUInt() {
		return ItemUtility::getUInt(this);
	}
	int64 ItemBase::getInt() {
		return ItemUtility::getInt(this);
	}
	double ItemBase::getValue() {
		return ItemUtility::getValue(this);
	}
	std::string ItemBase::getString() {
		return ItemUtility::getString(this);
	}

	Item<Tuple>* ItemUtility::formatTuple(const char* format, va_list ArgPtr)
	{
		Item<Tuple> *tuple = new Item<Tuple>;

		for(;*format;++format)
		{
			if(*format!='%')
				*tuple += format;
			else
			{
				++format;
				if('\0'==*format)
					break;
				switch(*format)
				{
				case 'a':
					*tuple += (Atom)va_arg(ArgPtr,char*);
					break;
				case 'd':
					*tuple += (int32)va_arg(ArgPtr,int32);
					break;
				case 'D':
					*tuple += (int64)va_arg(ArgPtr,int64);
					break;
				case 's':
					*tuple += (Binary)va_arg(ArgPtr,char*);
					break;
				case 'f':
					*tuple += (double)va_arg(ArgPtr,double);
					break;
				case 'c':
					*tuple += (int8)va_arg(ArgPtr,char);
					break;
				case 'b':
					*tuple += (int8)va_arg(ArgPtr,bool);
					break;
				case 'B':
					*tuple += (ItemBase*)new Item<Binary>((Bytes)va_arg(ArgPtr,Bytes),true);
					break;
				case 'W':
				    *tuple += (ItemBase*)new Item<Binary>(ItemUtility::ToBytes((std::wstring)va_arg(ArgPtr,std::wstring)),true);
					break;
				case 'S':
					*tuple += (ItemBase*)new Item<Binary>((std::string)va_arg(ArgPtr,std::string));
					break;
				}
			}
		}

		return tuple;
	}

	Item<Tuple>* ItemUtility::formatTuple(const char* format, ...)
	{
		va_list	ArgPtr;

		va_start(ArgPtr, format);
		//vsprintf(Message, format, ArgPtr);

		Item<Tuple> *tuple = formatTuple(format,ArgPtr);

		va_end(ArgPtr);

		return tuple;
	}

	void ItemUtility::formatReadTuple(Item<Tuple> *tuple, const char *format, va_list ArgPtr)
	{
		int index = 0;
		for(;*format;++format)
		{
			if(*format!='%')
				continue;
			else
			{
				++format;
				if('\0'==*format)
					break;
				switch(*format)
				{
				case 'a':
					{
					char *tmp = va_arg(ArgPtr,char*);
					memcpy(tmp,(*tuple)[index]->getString().c_str(),sizeof(char)*(*tuple)[index]->getString().length()+1);
					//*tuple += (Atom)va_arg(ArgPtr,char*);
					}
					break;
				case 'd':
					{
					int32 *tmp = va_arg(ArgPtr,int32*);
					*tmp = static_cast<int32>((*tuple)[index]->getInt());
					//*tuple += (Atom)va_arg(ArgPtr,char*);
					}
					break;
				case 's':
					{
					char *tmp = va_arg(ArgPtr,char*);
					memcpy(tmp,(*tuple)[index]->getString().c_str(),sizeof(char)*(*tuple)[index]->getString().length()+1);
					//*tuple += (Atom)va_arg(ArgPtr,char*);
					}
					break;
				case 'f':
					{
					double *tmp = va_arg(ArgPtr,double*);
					*tmp = (*tuple)[index]->getValue();
					//*tuple += (Atom)va_arg(ArgPtr,char*);
					}
					break;
				case 'c':
					{
					char *tmp = va_arg(ArgPtr,char*);
					*tmp = static_cast<char>((*tuple)[index]->getInt());
					//*tuple += (Atom)va_arg(ArgPtr,char*);
					}
					break;
				case 'b':
					{
					bool *tmp = va_arg(ArgPtr,bool*);
					*tmp = ((*tuple)[index]->getInt())!=0;
					//*tuple += (Atom)va_arg(ArgPtr,char*);
					}
				}
				index++;
			}
		}
	}
	void ItemUtility::formatReadTuple(Item<Tuple> *tuple, const char *format, ...)
	{
		va_list	ArgPtr;

		va_start(ArgPtr, format);
		//vsprintf(Message, format, ArgPtr);

		formatReadTuple(tuple, format,ArgPtr);

		va_end(ArgPtr);
	}

	uint64 ItemUtility::getUInt(ItemBase *item)
	{
		switch(item->getType())
		{
		case GNET_TYPE_UINT_8:
			return ((Item<uint8>*)item)->getData();
		case GNET_TYPE_UINT_32:
			return ((Item<uint32>*)item)->getData();
		case GNET_TYPE_UINT_64:
			return ((Item<uint64>*)item)->getData();
		default:
			throw "type error";
		}
		return 0;
	}
	int64 ItemUtility::getInt(ItemBase *item)
	{
		switch(item->getType())
		{
		case GNET_TYPE_INT_8:
			return ((Item<int8>*)item)->getData();
		case GNET_TYPE_INT_32:
			return ((Item<int32>*)item)->getData();
		case GNET_TYPE_INT_64:
			return ((Item<int64>*)item)->getData();
		default:
			return getUInt(item);
		}
		return 0;
	}
	double ItemUtility::getValue(ItemBase *item)
	{
		if(item->getType()==GNET_TYPE_DOUBLE)
			return ((Item<double>*)item)->getData();
		else
			return static_cast<double>(getInt(item));
	}
	std::string ItemUtility::getString(ItemBase *item)
	{
		if(item==NULL)
			return "";
		else if(item->getType()==GNET_TYPE_BINARY)
			return ((Item<Binary>*)item)->getData();
		else if(item->getType()==GNET_TYPE_ATOM)
			return ((Item<Binary>*)item)->getData();
<<<<<<< HEAD
		else if(item->getType()==GNET_TYPE_UINT_8) {
=======
		else if(item->getType()==GNET_TYPE_UINT_8||item->getType()==GNET_TYPE_INT_8) {
>>>>>>> 8fac03783867a4916e28db1e466348ee4dc2cf87
			std::string ret;
			ret += (char)((Item<uint8>*)item)->getData();
			return ret;
		}
		else if(item->getType()==GNET_TYPE_LIST) {
			Item<List> *list = item->as<Item<List>>();
			std::string ret = "";
			for(int i = 0; i < list->size(); i++)
				ret += getString(list->getItem(i));
			return ret;
		}
		else
			return item->getDescription();
	}
	std::wstring ItemUtility::getWString(ItemBase *item)
	{
		if(item==NULL)
			return L"";
		else if(item->getType()==GNET_TYPE_BINARY)
			return BytesTo<std::wstring>(((Item<Binary>*)item)->getRaw());
		else if(item->getType()==GNET_TYPE_ATOM)
			return Base::String(((Item<Binary>*)item)->getData());
		else if(item->getType()==GNET_TYPE_UINT_8||item->getType()==GNET_TYPE_INT_8) {
			std::string ret;
			ret += (char)((Item<uint8>*)item)->getData();
			return Base::String(ret);
		}
		else if(item->getType()==GNET_TYPE_LIST) {
			Item<List> *list = item->as<Item<List>>();
			std::wstring ret = L"";
			for(int i = 0; i < list->size(); i++)
				ret += getWString(list->getItem(i));
			return ret;
		}
		else
			return L"";
	}

	//special
	Bytes ItemUtility::ToBytes(const std::wstring &data) {
		Bytes ret;
		for(int i = 0; i < data.size(); i++)
			ret.push_back(byte(data[i]>>8)), ret.push_back(byte(data[i]&0xFFFFFFFF));
		return ret;
	}
	template<>
	std::wstring ItemUtility::BytesTo<std::wstring>(Bytes &bytes) {
		std::wstring ret = L"";
		size_t size = bytes.size()>>1;
		for(int i = 0; i < size; i++)
			ret += wchar_t((bytes[i<<1]<<8)+bytes[(i<<1)+1]);
		return ret;
	}
}
