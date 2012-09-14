/*
 *  SoraFileSystem.cpp
 *  Sora
 *
 *  Created by GriffinBu on 1/12/11.
 *  Copyright 2011 Griffin Bu(Project L). All rights reserved.
 *
 */

#include "SoraFileSystem.h"
#include "SoraResourceManager.h"
#include "SoraStringTokenlizer.h"

#include "SoraCore.h"
#include "SoraTexture.h"
#include "SoraFont.h"
#include "SoraConfigUtil.h"
#include "SoraResourceFile.h"
#include "SoraSoundSystem.h"
#include "SoraShader.h"
#include "SoraSpriteSheet.h"

#include "SoraLogger.h"

namespace sora {
    
    uint64 SoraFileSystem::ResourceMemory = 0;
	
	SoraFileSystem::SoraFileSystem() {
	}
	
	SoraFileSystem::~SoraFileSystem() {
#ifdef SORA_ENABLE_MULTI_THREAD
        SoraMutexGuard guard(mThreadMutex);

        mIOThreadPool.stop();
#endif
	}
	
	void SoraFileSystem::attachResourceManager(SoraResourceManager* rm) {
		mResourceManagers.push_back(SoraAutoPtr<SoraResourceManager>(rm));
	}
	
	void SoraFileSystem::detachResourceManager(const StringType& name) {
        ResourceManagerCont::iterator it = mResourceManagers.begin();
        ResourceManagerCont::iterator end = mResourceManagers.end();
        
		for(; it != end; ++it) {
			if(name.compare((*it)->getName()) == 0) {
				mResourceManagers.erase(it);
				break;
			}
		}
	}

	void* SoraFileSystem::readResourceFile(const StringType& file, uint32 pos, uint32 size) {
		void* data;
        
        ResourceManagerCont::iterator it = mResourceManagers.begin();
        ResourceManagerCont::iterator end = mResourceManagers.end();

		for(; it != end; ++it) {
			if((data = (*it)->readResourceFile(file, pos, size)) != NULL) {
#ifdef SORA_ENABLE_MULTI_THREAD
                SoraMutexGuard guard(mIOMutex);
#endif
                ResourceMemory += size;
                
                AutoPtrType resource(static_cast<uint8*>(data));
                
                ResourceInfo info;
                info.mResource = resource;
                info.mSize = size;
                mResourceCaches.insert(std::make_pair((SoraHandle)data, info));
                
				return data;
            }
		}
		return 0;
	}
	
	void* SoraFileSystem::getResourceFile(const StringType& file, uint32& size) {
		void* data;
        
        ResourceManagerCont::iterator it = mResourceManagers.begin();
        ResourceManagerCont::iterator end = mResourceManagers.end();
        
		for(; it != end; ++it) {
			if((data = (*it)->getResourceFile(file, size)) != NULL) {
#ifdef SORA_ENABLE_MULTI_THREAD
                SoraMutexGuard guard(mIOMutex);
#endif
                ResourceMemory += size;
                
                AutoPtrType resource(static_cast<uint8*>(data));
                
                ResourceInfo info;
                info.mResource = resource;
                info.mSize = size;
                mResourceCaches.insert(std::make_pair((SoraHandle)data, info));

				return data;
            }
		}
		return 0;
	}
	
	uint32 SoraFileSystem::getResourceFileSize(const StringType& file) { 
		uint32 size;

        ResourceManagerCont::iterator it = mResourceManagers.begin();
        ResourceManagerCont::iterator end = mResourceManagers.end();

		for(; it != end; ++it) {
			if((size = (*it)->getResourceFileSize(file)) != 0) {

				return size;
            }
		}
		return 0;
	}
	
	void SoraFileSystem::freeResourceFile(void* p) {
		if(!p)
            return;
        
#ifdef SORA_ENABLE_MULTI_THREAD
        SoraMutexGuard guard(mIOMutex);
#endif
        
        ResourceCacheMap::iterator itResource = mResourceCaches.find((SoraHandle)p);
        if(itResource != mResourceCaches.end()) {
            ResourceMemory -= itResource->second.mSize;
            mResourceCaches.erase(itResource);
        }
	}
	
	SoraHandle	SoraFileSystem::loadResourcePack(const StringType& file) {
		for(size_t i=1; i<mResourceManagers.size(); ++i) {
			SoraResourceManager* prm = mResourceManagers[i].get();
				
			SoraHandle r = prm->loadResourcePack(file);
			if(r) {
				//prm->attachResourcePack(r);
				return r;
			}
		}
		/*
		 mResourceManagers[0] = sorafodlerresoucemanager
		 just add the pack as a folder
		 */
		SoraHandle r = mResourceManagers[0]->loadResourcePack(file);
        mResourceManagers[0]->attachResourcePack(r);
		//modify by tempbuffer, return handle value
		return r;
	}
	
	void SoraFileSystem::detachResourcePack(SoraHandle handle) {
		for(size_t i=0; i<mResourceManagers.size(); ++i) {
			mResourceManagers[i]->detachResourcePack(handle);
		}
	}
	
	bool SoraFileSystem::enumFiles(std::vector<StringType>& cont, const StringType& folder) {
		for(size_t i=1; i<mResourceManagers.size(); ++i) {
			SoraResourceManager* prm = mResourceManagers[i].get();
			
			if(prm->enumFiles(cont, folder))
				return true;
		}
		
		return mResourceManagers[0]->enumFiles(cont, folder);
	}
    
    void SoraFileSystem::addResource(SoraResource::Ptr r, StringType& tag) {
        mResources.insert(std::make_pair(tag, r));
    }
    
    SoraResource::Ptr SoraFileSystem::addResource(const StringType& file, SoraResource::Type type, const StringType& tag) {
        std::string rtag = tag.empty() ? file : tag;
        
        SoraResource::Ptr resource;
        switch(type) {
            case SoraResource::Texture: {
                SoraTextureHandle tex = SoraTexture::LoadFromFile(file);
                if(tex) {
                    resource = new SoraTextureResource(tex);
                } else
                    log_mssg(vamssg("SoraFileSystem::error loading resource %s\n", file.c_str()));
                break;
            }
                
            case SoraResource::Font: {
                SoraFont* font = SoraFont::LoadFromFile(file, 20);
                if(font) {
                    resource = new SoraFontResource(font);
                } else
                    log_mssg(vamssg("SoraFileSystem::error loading resource %s\n", file.c_str()));
                break;
            }
            
            case SoraResource::Config: {
                SoraConfigParser* parser = new SoraConfigParser;
                if(parser->open(file)) {
                    resource = new SoraConfigResource(parser);
                } else {
                    log_mssg(vamssg("SoraFileSystem::error loading resource %s\n", file.c_str()));
                    delete parser;
                }
                break;
            }
            
            case SoraResource::Music: {
                SoraMusicFile* music = SoraSoundSystem::LoadMusicFromFile(file);
                if(music) {
                    resource = new SoraMusicResource(music);
                } else {
                    log_mssg(vamssg("SoraFileSystem::error loading resource %s\n", file.c_str()));
                }
                break;
            }
            
            case SoraResource::SoundEffect: {
                SoraSoundEffectFile* se = SoraSoundSystem::LoadSoundEffectFromFile(file);
                if(se) {
                    resource = new SoraSoundEffectResource(se);
                } else {
                    log_mssg(vamssg("SoraFileSystem::error loading resource %s\n", file.c_str()));
                }
                break;
            }
            
            case SoraResource::RawData: {
                SoraResourceFile* res = new SoraResourceFile(file);
                if(!res->isValid()) {
                    log_mssg(vamssg("SoraFileSystem::error loading resource %s\n", file.c_str()));
                    delete res;
                    res = 0;
                } else
                    resource = res;
                break;
            }
                
            case SoraResource::SpriteSheet: {
                SoraSpriteSheet* sheet = new SoraSpriteSheet;
                if(sheet->load(file)) {
                    resource = sheet;
                } else {
                    log_mssg(vamssg("SoraFileSystem::error loading resource %s\n", file.c_str()));
                    delete sheet;
                    sheet = 0;
                }
                break;
            }
                
            case SoraResource::Shader: {
                SoraShaderContext* res = SoraCore::Ptr->createShaderContext();
                if(res) {
                    SoraStringTokenlizer token(file);
                    SoraShader* result = 0;
                    
                    if(token.size() == 4) {
                        result = res->attachFragmentShader(token[0], token[1]);
                        result = res->attachVertexShader(token[2], token[3]);
                    } 
                    else if(token.size() == 2) {
                        result = res->attachFragmentShader(token[0], token[1]);
                    } 
                    else if(token.size() == 1) {
                        std::string ident = token[0];
                        size_t dotPos = ident.rfind('.');
                        if(dotPos != std::string::npos) {
                            result = res->attachFragmentShader(ident.substr(0, dotPos), ident.substr(dotPos+1, ident.size()));
                        }
                    }
                    if(!result) {
                        log_error(vamssg("SoraFileSystem::error loading resource %s\n", file.c_str()));
                        
                        delete res;
                        break;
                    }
                    resource = new SoraShaderResource(res);
                } else
                    log_error(vamssg("SoraFileSystem::error loading resource %s\n", file.c_str()));
                break;
            }
        }
        
        if(resource.isValid()) {
            mResources.insert(std::make_pair(rtag, resource));
        }
        return resource;
    }
    
    SoraResource::Ptr SoraFileSystem::getResource(const StringType& tag) {
        ResourceMap::iterator it = mResources.find(tag);
        if(it != mResources.end()) {
            return it->second;
        }
        return SoraResource::Ptr();
    }
    
    void SoraFileSystem::freeResource(const StringType& tag) {
        ResourceMap::iterator it = mResources.find(tag);
        if(it != mResources.end()) {
            mResources.erase(it);
        }
    }
    
    bool SoraFileSystem::loadResourceScript(const StringType& script) {
        SoraConfigParser parser;
        if(parser.open(script)) {
            if(parser.toNode("/resources")) {
                parser.toFirstChild();
                do {
                    std::string strtype = parser.getString("type");
                    if(strtype.empty())
                        strtype = "raw";
                    
                    SoraResource::Type type = SoraResource::IdentToType(strtype);
                    if(type == SoraResource::Invalid) {
                        log_error(vamssg("SoraFileSystem::loadResourceScript: invalid type identifier %s", strtype.c_str()));
                        continue;
                    }
                    
                    std::string path = parser.getString("path");
                    if(path.empty() && type != SoraResource::Shader) {
                        log_error("SoraFileSystem::loadResourceScript: no path specified for node");
                        continue;
                    }
                                        
                    std::string tag = parser.getString("tag");
                    if(tag.empty())
                        tag = path;
                    
                    if(type == SoraResource::Shader) {
                        SoraShaderContext* context = SoraCore::Ptr->createShaderContext();
                        if(context) {
                            std::string frag = parser.getString("fragment");
                            std::string fragEntry = parser.getString("fragment-entry");
                            std::string vertex = parser.getString("vertex");
                            std::string vertexEntry = parser.getString("vertex-entry");
                            
                            if(!frag.empty() && !fragEntry.empty()) 
                                context->attachFragmentShader(frag, fragEntry);
                            if(!vertex.empty() && !vertexEntry.empty())
                                context->attachVertexShader(vertex, vertexEntry);
                            
                        } else 
                            log_error("SoraFileSystem: error creating shader context");
                        
                        SoraResource::Ptr resource;
                        resource = new SoraShaderResource(context);
                        mResources.insert(std::make_pair(tag, resource));
                        
                    } else {
                        addResource(path, type, tag);
                    }
                } while(parser.toNextChild());
                
                return true;
            }
        }
        return false;
    }
    
    SoraTextureHandle SoraFileSystem::getTextureResource(const StringType& tag) {
        SoraResource::Ptr rptr = getResource(tag);
        if(rptr.isValid()) {
            sora_assert(rptr->getType() == SoraResource::Texture);
            return static_cast<SoraTextureResource*>(rptr.get())->getTexture();
        } else {
            rptr = addResource(tag, SoraResource::Texture, tag);
            if(rptr.isValid()) 
                return static_cast<SoraTextureResource*>(rptr.get())->getTexture(); 
        }
        log_error(vamssg("SoraFileSystem: error finding resource with tag %s", tag.c_str()));
        return 0;
    }
    
    SoraFont* SoraFileSystem::getFontResource(const StringType& tag) {
        SoraResource::Ptr rptr = getResource(tag);
        if(rptr.isValid()) {
            sora_assert(rptr->getType() == SoraResource::Font);
            return static_cast<SoraFontResource*>(rptr.get())->getFont();
        } else {
            rptr = addResource(tag, SoraResource::Font, tag);
            if(rptr.isValid()) 
                return static_cast<SoraFontResource*>(rptr.get())->getFont();
        }
        log_error(vamssg("SoraFileSystem: error finding resource with tag %s", tag.c_str()));
        return 0;
    }
    
    SoraMusicFile* SoraFileSystem::getMusicResource(const StringType& tag) {
        SoraResource::Ptr rptr = getResource(tag);
        if(rptr.isValid()) {
            sora_assert(rptr->getType() == SoraResource::Music);
            return static_cast<SoraMusicResource*>(rptr.get())->getMusic();
        } else {
            rptr = addResource(tag, SoraResource::Music, tag);
            if(rptr.isValid()) 
                return static_cast<SoraMusicResource*>(rptr.get())->getMusic();
        }
        log_error(vamssg("SoraFileSystem: error finding resource with tag %s", tag.c_str()));
        return 0;
    }
    
    SoraSoundEffectFile* SoraFileSystem::getSoundEffectResource(const StringType& tag) {
        SoraResource::Ptr rptr = getResource(tag);
        if(rptr.isValid()) {
            sora_assert(rptr->getType() == SoraResource::SoundEffect);
            return static_cast<SoraSoundEffectResource*>(rptr.get())->getSoundEffect();
        } else {
            rptr = addResource(tag, SoraResource::SoundEffect, tag);
            if(rptr.isValid()) 
                return static_cast<SoraSoundEffectResource*>(rptr.get())->getSoundEffect();
        }
        log_error(vamssg("SoraFileSystem: error finding resource with tag %s", tag.c_str()));
        return 0;
    }
    
    SoraConfigParser* SoraFileSystem::getConfigResource(const StringType& tag) {
        SoraResource::Ptr rptr = getResource(tag);
        if(rptr.isValid()) {
            sora_assert(rptr->getType() == SoraResource::Config);
            return static_cast<SoraConfigResource*>(rptr.get())->getConfig();
        } else {
            rptr = addResource(tag, SoraResource::Texture, tag);
            if(rptr.isValid()) 
                return static_cast<SoraConfigResource*>(rptr.get())->getConfig();
        }
        log_error(vamssg("SoraFileSystem: error finding resource with tag %s", tag.c_str()));
        return 0;
    }
    
    SoraResourceFile* SoraFileSystem::getRawDataResource(const StringType& tag) {
        SoraResource::Ptr rptr = getResource(tag);
        if(rptr.isValid()) {
            sora_assert(rptr->getType() == SoraResource::RawData);
            return static_cast<SoraResourceFile*>(rptr.get());
        } else {
            rptr = addResource(tag, SoraResource::RawData, tag);
            if(rptr.isValid()) 
                return static_cast<SoraResourceFile*>(rptr.get());
        }
        log_error(vamssg("SoraFileSystem: error finding resource with tag %s", tag.c_str()));
        return 0;
    }
    
    SoraShaderContext* SoraFileSystem::getShaderResource(const StringType& tag) {
        SoraResource::Ptr rptr = getResource(tag);
        if(rptr.isValid()) {
            sora_assert(rptr->getType() == SoraResource::Shader);
            return static_cast<SoraShaderResource*>(rptr.get())->getShaderContext();
        }
        log_error(vamssg("SoraFileSystem: error finding resource with tag %s", tag.c_str()));
        return 0;
    }
    
    SoraSpriteSheet* SoraFileSystem::getSpriteSheetResource(const StringType& tag) {
        SoraResource::Ptr rptr = getResource(tag);
        if(rptr.isValid()) {
            sora_assert(rptr->getType() == SoraResource::SpriteSheet);
            return static_cast<SoraSpriteSheet*>(rptr.get());
        }
        log_error(vamssg("SoraFileSystem: error finding resource with tag %s", tag.c_str()));
        return 0;
    }
    
#ifdef SORA_ENABLE_MULTI_THREAD
    
    bool SoraFileSystem::loadResourceScriptAsync(const StringType& script) {
        SoraConfigParser parser;
        if(parser.open(script)) {
            if(parser.toNode("/resources")) {
                parser.toFirstChild();
                do {
                    std::string path = parser.getString("path");
                    if(path.empty()) {
                        log_error("SoraFileSystem::loadResourceScript: no path specified for node");
                        continue;
                    }
                    
                    std::string strtype = parser.getString("type");
                    if(strtype.empty())
                        strtype = "raw";
                    
                    SoraResource::Type type = SoraResource::IdentToType(strtype);
                    if(type == SoraResource::Invalid) {
                        log_error(vamssg("SoraFileSystem::loadResourceScript: invalid type identifier %s", strtype.c_str()));
                        continue;
                    }
                    
                    std::string tag = parser.getString("tag");
                    if(tag.empty())
                        tag = path;
                    
                    addResourceAsync(path, type, tag);
                    
                } while(parser.toNextChild());
                
                return true;
            }
        }
        return false;
    }
    
    void SoraFileSystem::resource_async_not(void* data, uint32 size, void* userData) {
        ResourceIoParam* param = static_cast<ResourceIoParam*>(userData);
        
        SoraResource::Ptr resource;
        switch(param->mType) {
            case SoraResource::Texture: {
                SoraTextureHandle tex = SoraTexture::LoadFromMemory((uint32*)data, size);
                if(tex) {
                    resource = new SoraTextureResource(tex);
                } else
                    log_mssg(vamssg("SoraFileSystem::error loading resource %s\n", param->mName.c_str()));
                break;
            }
                
            case SoraResource::Font: {
                SoraFont* font = SoraFont::LoadFromMemory(data, size, 20, param->mName);
                if(font) {
                    resource = new SoraFontResource(font);
                } else
                    log_mssg(vamssg("SoraFileSystem::error loading resource %s\n", param->mName.c_str()));
                break;
            }
                
            case SoraResource::Config: {
                SoraConfigParser* parser = new SoraConfigParser;
                if(parser->open(data, size)) {
                    resource = new SoraConfigResource(parser);
                } else {
                    log_mssg(vamssg("SoraFileSystem::error loading resource %s\n", param->mName.c_str()));
                    delete parser;
                }
                break;
            }
                
            case SoraResource::SpriteSheet: {
                SoraSpriteSheet* sheet = new SoraSpriteSheet;
                if(sheet->loadFromMemory(data, size)) {
                    resource = sheet;
                } else {
                    log_mssg(vamssg("SoraFileSystem::error loading resource %s\n", param->mName.c_str()));
                    delete sheet;
                    sheet = 0;
                }
                break;
            }
                
            case SoraResource::Music: {
                SoraMusicFile* music = SoraSoundSystem::LoadMusicFromMemory(data, size);
                if(music) {
                    resource = new SoraMusicResource(music);
                } else {
                    log_mssg(vamssg("SoraFileSystem::error loading resource %s\n", param->mName.c_str()));
                }
                break;
            }
                
            case SoraResource::SoundEffect: {
                SoraSoundEffectFile* se = SoraSoundSystem::LoadSoundEffectFromMemory(data, size);
                if(se) {
                    resource = new SoraSoundEffectResource(se);
                } else {
                    log_mssg(vamssg("SoraFileSystem::error loading resource %s\n", param->mName.c_str()));
                }
                break;
            }
                
            case SoraResource::Shader: {
                break;
            }
        }
        
        if(resource.isValid()) {
            mResources.insert(std::make_pair(param->mTag, resource));
        }
        
        delete param;
    }
    
    void SoraFileSystem::addResourceAsync(const StringType& file, SoraResource::Type type, const StringType& tag) {
        std::string rtag = tag.empty() ? file : tag;
        
        ResourceIoParam* param = new ResourceIoParam;
        param->mName = file;
        param->mTag = tag.empty() ? file : tag;
        param->mType = type;
        
        sora_assert(type != SoraResource::Shader);
        
        loadResourceFileAsync(file, Bind(this, &SoraFileSystem::resource_async_not), param);
    }
    
    void SoraFileSystem::ioThreadFunc(void* arg) {
        
        IoThreadParam* param = static_cast<IoThreadParam*>(arg);
        uint32 size;
        void* pdata = getResourceFile(param->mName, size);
        param->mNotification(pdata, size, param->mUserData);
        
        delete param;
    }
    
    void SoraFileSystem::loadResourceFileAsync(const StringType& file, const AsyncNotification& notification, void* userdata) {
        if(!mIOThreadPool.isRunning()) {
            mIOThreadPool.start(SORA_MAX_IO_THREAD);
        }
        SoraThreadTask task(&SoraFileSystem::ioThreadFunc, this);
        IoThreadParam* param = new IoThreadParam;
        param->mName = file;
        param->mNotification = notification;
        param->mUserData = userdata;
        task.setArg(param);
        
        SoraMutexGuard guard(mThreadMutex);
        mIOThreadPool.run(task);
    }
    
#endif
    
} // namespace sora