/*
 *  SoraResourceManagerSolver.h
 *  Sora
 *
 *  Created by GriffinBu on 1/12/11.
 *  Copyright 2011 Griffin Bu(Project L). All rights reserved.
 *
 */

#ifndef SORA_RESOURCE_MANAGER_SOLVER_H_
#define SORA_RESOURCE_MANAGER_SOLVER_H_

#include "SoraException.h"
#include "SoraPlatform.h"
#include "SoraAutoPtr.h"
#include "SoraStringId.h"
#include "SoraRefCounted.h"
#include "SoraFunction.h"
#include "SoraResource.h"
#include <vector>
#include <map>

#ifdef SORA_ENABLE_MULTI_THREAD
#include "thread/SoraThreadPool.h"
#include "thread/SoraMutex.h"
#endif

namespace sora {
	
	class SoraResourceManager;
    class SoraResourceFile;
    class SoraSpriteSheet;
    	
	class SORA_API SoraFileSystem {
    private:
        SoraFileSystem();
		~SoraFileSystem();
		
        friend class SoraCore;
        
	public:
		
        static uint64 ResourceMemory;
        
        void attachResourceManager(SoraResourceManager* rm);
		void detachResourceManager(const StringType& name);
		
		SoraResourceHandle  loadResourcePack    (const StringType& file);
		void                detachResourcePack  (SoraResourceHandle handle);
		
		void*   readResourceFile    (const StringType& file, uint32 pos, uint32 size);
		void*   getResourceFile     (const StringType& file, uint32& size);
		uint32  getResourceFileSize (const StringType& file);
		void    freeResourceFile    (void* p);
		
		bool    enumFiles(std::vector<StringType>& cont, const StringType& folder);
        
    public:
        /** New **/
        /**
         * Add a resource 
         * Tag is optical but better for performance
         * If tag is empty then tag = file
         **/
        SoraResource::Ptr addResource(const StringType& file, SoraResource::Type type, const StringType& tag=StringType());
        void              addResource(SoraResource::Ptr r, StringType& tag);
        SoraResource::Ptr getResource(const StringType& tag);
        void              freeResource(const StringType& tag);
        
        /**
         * Load a resource config script
         * Root node should be resources
         * Can have three fields, type, path and tag
         *  <resources>
         *      <node type="texture" path="my_texture.png" tag="st1_background">
         *      ...
         *  </resources>
         * etc
         **/
        bool loadResourceScript(const StringType& script);
        
        SoraTextureHandle       getTextureResource      (const StringType& tag);
        SoraFont*               getFontResource         (const StringType& tag);
        SoraMusicFile*          getMusicResource        (const StringType& tag);
        SoraSoundEffectFile*    getSoundEffectResource  (const StringType& tag);
        SoraConfigParser*       getConfigResource       (const StringType& tag);
        SoraResourceFile*       getRawDataResource      (const StringType& tag);
        SoraShaderContext*      getShaderResource       (const StringType& tag);
        SoraSpriteSheet*        getSpriteSheetResource  (const StringType& tag);
        
#ifdef SORA_ENABLE_MULTI_THREAD
        
        bool loadResourceScriptAsync(const StringType& script);
        void addResourceAsync(const StringType& file, SoraResource::Type type, const StringType& tag=StringType());
        
    private:
        struct ResourceIoParam {
            std::string         mName;
            std::string         mTag;
            SoraResource::Type  mType;
        };
        void resource_async_not(void*, uint32, void*);
#endif
        
    private:
        typedef sora_hash_map<std::string, SoraResource::Ptr> ResourceMap;
        ResourceMap mResources;
                
    public:
        
#ifdef SORA_ENABLE_MULTI_THREAD
        typedef SoraFunction<void(void*, SoraHandle, void*)> AsyncNotification;
        void loadResourceFileAsync(const StringType& file, const AsyncNotification& notification, void* userdata);
        
    private:
        struct IoThreadParam {
            void*               mUserData;
            StringType          mName;
            AsyncNotification   mNotification;
        };
        void ioThreadFunc(void* arg);
#endif
        
	private:		
		typedef std::vector<SoraAutoPtr<SoraResourceManager> > ResourceManagerCont;
		ResourceManagerCont mResourceManagers;
        
        template<class C>
        struct ReleasePolicy {
            static void Release(C* ptr) {
                sora_free(static_cast<void*>(ptr));
            }
        };
        
        typedef SoraAutoPtr<uint8, autoptr::RefCounter, ReleasePolicy<uint8> > AutoPtrType;
        struct ResourceInfo {
            AutoPtrType mResource;
            uint32 mSize;
        };
        typedef std::map<SoraStringId, ResourceInfo> ResourceCacheMap;
        ResourceCacheMap mResourceCaches;
        
#ifdef SORA_ENABLE_MULTI_THREAD
        SoraThreadPool mIOThreadPool;
        SoraMutexLock mIOMutex;
        SoraMutexLock mThreadMutex;
#endif
	};
	
}

#endif