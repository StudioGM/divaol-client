#ifndef SORA_TEXTURE_MAP_H
#define SORA_TEXTURE_MAP_H

#include "SoraPlatform.h"
#include "SoraSingleton.h"
#include "SoraStringId.h"

namespace sora {
	
	/*
	 texture map poll for faster texture access
	 */
	class SORA_API SoraTextureMap: public SoraSingleton<SoraTextureMap> {
		friend class SoraSingleton<SoraTextureMap>;

	public:
		~SoraTextureMap();
		void clear();

		void add(const StringType& key, SoraTextureHandle tex);
        void del(SoraTextureHandle tex);		
		
		void addRf(const StringType& key);
		void addRf(SoraTextureHandle tex);
		void decRf(SoraTextureHandle tex);
		
		SoraTextureHandle get(const StringType& key);
        
        bool exist(SoraTextureHandle tex);
        SoraStringId getTextureName(SoraTextureHandle tex);
        
	private:
		typedef sora_hash_map<SoraStringId, SoraTextureHandle> TextureMap;
		TextureMap texMap;
		typedef sora_hash_map<SoraTextureHandle, SoraStringId> TextureMapRev;
		TextureMapRev texMapRv;
		
		typedef sora_hash_map<SoraStringId, int32> TextureUseCount;
		TextureUseCount texRefs;
	};
};

#endif