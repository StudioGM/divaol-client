/****************************************************
 *  Codec.h
 *
 *  Created by tempbuffer on 12/15/12.
 *  Copyright 2012 tempbuffer. All rights reserved.
 *
 *  Some Codec 
 ****************************************************/

#ifndef _CODEC_H_
#define _CODEC_H_

#include "../Common.h"
#include "../Timer.h"
#include "../Pattern/Singleton.h"
#include "../Net/Net.h"

namespace Base
{
	class Serilization
	{
	};

	namespace Codec
	{
		class Codec
		{
		public:
			virtual Raw encode(const Raw &src) = 0;
			virtual Raw decode(const Raw &src) = 0;
		};

		class RFA : public Codec, public Singleton<RFA>
		{
			uint64 seed;
			static const uint64 MAGIC_NUMBER = 42;
			static const uint64 MULTIPLIER = 10807;
			static const uint64 MOD = 0x7fffffff;
		public:
			RFA():seed(42) {}

			void setSeed(uint32 seed) {
				this->seed = seed;
			}
			uint32 rand() {
				seed = (seed*MULTIPLIER)&MOD;
				return (seed+MAGIC_NUMBER)*seed&MOD;
			}
			Raw encode(const Raw &src) {
				setSeed(uint32(TimeUtil::currentTime()));

				Raw ret;
				dword secretKey = rand();
				setSeed(secretKey);

				ret.appendAny(Net::Utility::htonl(secretKey));
				ret.appendAny(Net::Utility::htonl(_crypt(src.size(), secretKey)));
				secretKey = src.size();
				for(uint32 index = 0; index < src.size(); index+=4)
				{
					dword tmp = Net::Utility::htonl(src.getAny<dword>(index));
					ret.appendAny(Net::Utility::htonl(_crypt(tmp, secretKey)));
					secretKey = tmp;
				}

				return ret;
			}
			Raw decode(const Raw &src) {
				Raw ret;
				dword secretKey = Net::Utility::ntohl(src.getAny<dword>(0));
				setSeed(secretKey);

				uint32 size = _crypt(Net::Utility::ntohl(src.getAny<dword>(4)), secretKey);
				secretKey = size;

				for(uint32 index = 8; index < src.size(); index += 4)
				{
					dword tmp = Net::Utility::ntohl(src.getAny<dword>(index));
					tmp = _crypt(tmp, secretKey);
					ret.appendAny(Net::Utility::ntohl(tmp));
					secretKey = tmp;
				}
				ret.resize(size);

				return ret;
			}
			uint32 decodeLength(const Raw &src) {
				if (src.size() < 8)
					BASE_THROW_EXCEPTION("RFA decoder ilegal size");
				Raw ret;
				dword secretKey = Net::Utility::ntohl(src.getAny<dword>(0));
				setSeed(secretKey);

				return _crypt(Net::Utility::ntohl(src.getAny<dword>(4)), secretKey);
			}
		private:
			dword _crypt(dword word, dword key) {
				return word^key^rand();
			}
		};
	}
}

#endif