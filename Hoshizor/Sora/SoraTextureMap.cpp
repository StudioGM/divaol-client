//
//  SoraTextureMap.cpp
//  Sora
//
//  Created by Ruiwei Bu on 10/4/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraTextureMap.h"
#include "SoraCore.h"
#include "SoraLogger.h"

#include "SoraTexture.h"
#include "SoraException.h"
#include "SoraEnvValues.h"

namespace sora {
    
    SoraTextureMap::~SoraTextureMap() {
        for(TextureMap::iterator p = texMap.begin(); p != texMap.end(); ++p) {
            SoraCore::Instance()->releaseTexture(p->second);
        }
        texMap.clear();
        texMapRv.clear();
    }
    
    void SoraTextureMap::clear() {
        for(TextureMap::iterator p = texMap.begin(); p != texMap.end(); ++p) {
            if(texRefs[p->first] == 0) {
                SoraCore::Instance()->releaseTexture(p->second);
#ifdef _DEBUG
                log_mssg(vamssg("Free Texture: %lu, tex=%s\n", p->second, (id2str(p->second)).c_str()));
#endif
                texMapRv.erase(texMapRv.find(p->second));
                texMap.erase(p);
            }
        }
    }
    
    void SoraTextureMap::add(const StringType& key, SoraTextureHandle tex) {			
        SoraStringId uiKey = key.uniqueId();
        TextureMap::iterator pos = texMap.find(uiKey);
        if(pos == texMap.end()) {
            texMap[uiKey] = tex;
            texMapRv[tex] = uiKey;
            texRefs[uiKey] = 0;
        }
        else {
            del(pos->second);
            texMap[uiKey] = tex;
            texMapRv[tex] = uiKey;
            texRefs[uiKey] = 0;
        }
        
        SET_ENV_INT("TextureMapSize", (int32)texMap.size());
    }
    
    void SoraTextureMap::del(SoraTextureHandle tex) {
        TextureMapRev::iterator p = texMapRv.find(tex);
        if(p != texMapRv.end()) {
            texRefs.erase(texRefs.find(p->second));
            texMap.erase(texMap.find(p->second));
            texMapRv.erase(p);
        }
        
        SET_ENV_INT("TextureMapSize", (int32)texMap.size());
    }
    
    void SoraTextureMap::addRf(const StringType& key) {
        SoraStringId uiKey = key.uniqueId();
        texRefs[uiKey]++;
    }
    
    void SoraTextureMap::addRf(SoraTextureHandle tex) {
        TextureMapRev::iterator p = texMapRv.find(tex);
        if(p != texMapRv.end()) {
            texRefs[p->second]++;
        }
    }
    
    void SoraTextureMap::decRf(SoraTextureHandle tex) {
        TextureMapRev::iterator p = texMapRv.find(tex);
        if(p != texMapRv.end()) {
            texRefs[p->second]--;
            
            if(texRefs[p->second] == 0) {
                SoraCore::Ptr->releaseTexture(p->first);
                del(tex);
            }
			
        }
    }
    
    SoraTextureHandle SoraTextureMap::get(const StringType& key) {
        SoraStringId uiKey = key.uniqueId();
        TextureMap::iterator p = texMap.find(uiKey);
        if(p != texMap.end()) {
            //SoraTexture* tex = (SoraTexture*)p->second;
            texRefs[uiKey]++;
            return p->second;
        }
        return 0;
    }
    
    bool SoraTextureMap::exist(SoraTextureHandle tex) {
        TextureMapRev::iterator p = texMapRv.find(tex);
        if(p != texMapRv.end()) {	
            return true;
        }
        return false;
    }
    
    SoraStringId SoraTextureMap::getTextureName(SoraTextureHandle tex) {
        TextureMapRev::iterator p = texMapRv.find(tex);
        if(p != texMapRv.end()) {
            return p->second;
        }
        return 0;
    }
    
	std::string SoraTextureMap::getTexRefInfo() const
	{
		std::string res = "";
		for (TextureUseCount::const_iterator i = texRefs.cbegin(); i != texRefs.cend(); i++)
		{
			char buf[100];
			//itoa(i->second, buf, 10);
			sprintf(buf, "(%I64u, %d)\n", i->first, i->second);
			res += buf;
		}
		return res;
	}
    
} // namespace sora
