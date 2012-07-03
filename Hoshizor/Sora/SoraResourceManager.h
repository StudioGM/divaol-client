#ifndef SORA_RESOURCE_MANAGER_H
#define SORA_RESOURCE_MANAGER_H

#include "SoraPlatform.h"
#include "SoraString.h"
#include "SoraFunction.h"

namespace sora {
	
	class SORA_API SoraResourceManager {
	public:
		virtual ~SoraResourceManager() {}

		virtual SoraResourceHandle	loadResourcePack    (const StringType& file) = 0;
		virtual void                attachResourcePack	(SoraResourceHandle handle) = 0;
		virtual void                detachResourcePack  (SoraResourceHandle handle) = 0;
		
		virtual void*   readResourceFile        (const StringType& file, uint32 pos, uint32 size) = 0;
		virtual void*   getResourceFile         (const StringType& file, uint32& size) = 0;
		virtual uint32  getResourceFileSize     (const StringType& file) = 0;
		virtual void    freeResourceFile        (void* p) = 0;

		/* name of this resource manager */
		virtual StringType getName() const = 0;
    
		/*
		 determines if a format is supported by this resource manager
		 @param format, resource format string, zip etc
		 */
		virtual bool isFormatSupported(const StringType& format) const = 0;
		
		virtual bool enumFiles(std::vector<StringType>& cont, const StringType& folder) = 0;
        
        static SoraResourceHandle   LoadAndAttachResourcePack(const StringType& file);
        static void                 DetachResourcePack(SoraResourceHandle handle);
        static void*                LoadResourceFile(const StringType& file, uint32* size);
        static uint32               GetResourceFileSize(const StringType& file);
        static void                 FreeResourceFile(void* p);
        
#ifdef SORA_ENABLE_MULTI_THREAD
        
        typedef SoraFunction<void(void*, SoraHandle, void*)> AsyncNotification;
        static void LoadResourceFileAsync(const StringType& file, const AsyncNotification& handler, void* puserdata=0);
        
#endif
	};
	

} // namespace sora;


#endif