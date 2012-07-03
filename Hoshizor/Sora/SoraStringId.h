/*
 *  SoraStringId.h
 *  Sora
 *
 *  Created by GriffinBu on 1/21/11.
 *  Copyright 2011 Griffin Bu(Project L). All rights reserved.
 *
 */

#ifndef SORA_STRING_ID_H_
#define SORA_STRING_ID_H_

#include "SoraPlatform.h"
#include "SoraSingleton.h"
#include "SoraString.h"
#include "SoraStringConv.h"
#include "util/SoraHash.h"

namespace sora {
	
	typedef uint64 stringId;
    typedef stringId SoraStringId;

	class SoraStringManager: public SoraSingleton<SoraStringManager> {
		friend class SoraSingleton<SoraStringManager>;
		
	public:
        // explicit add strings, no hash implied, be care
        inline void addString(const StringType& str, SoraStringId sid) {
            strings[sid] = str;
        }

		inline SoraStringId getStringId(const StringType& str) {
			SoraStringId sid = crc32(str.get());
			if(strings.find(sid) == strings.end()) {
				strings[sid] = str;
			}
			return sid;
		}
		
		static SoraStringId getStringIdNoCache(const StringType& str) {
			return crc32(str.get());
		}
		
		inline std::wstring getStringByIdW(SoraStringId sid) {
			if(sid == 0) return NULL;
			std::wstring str = s2ws(strings[sid]);
			if(str.size() != 0)
				return str;
			return std::wstring();;
		}
		
		inline std::string getStringById(SoraStringId sid) {
			if(sid == 0) return NULL;
			std::string str = strings[sid];
			if(str.size() != 0)
				return str;
			return std::string();
		}
		
	private:
		typedef sora_hash_map<SoraStringId, std::string> STR_MAP;
		STR_MAP strings;
	};
	
    #define SORA_STR_MANAGER SoraStringManager::Instance()
	#define str2id(str) SORA_STR_MANAGER->getStringId(str)
	#define id2strw(id) SORA_STR_MANAGER->getStringByIdW(id)
	#define id2str(id) SORA_STR_MANAGER->getStringById(id)
	#define str2idnc(str) SORA_STR_MANAGER->getStringIdNoCache(str)
    
    inline SoraStringId GetUniqueStringId(const StringType& name, bool cache=false) {
        if(!cache)
            return crc32(name.get());
        return SoraStringManager::Instance()->getStringId(name);
    }
    
    inline SoraString GetStringByUniqueId(SoraStringId sid) {
        return SoraStringManager::Instance()->getStringById(sid);
    }
    
    inline SoraWString GetStringByUniqueIdW(SoraStringId sid) {
        return SoraStringManager::Instance()->getStringByIdW(sid);
    }
} // namespace sora

#endif