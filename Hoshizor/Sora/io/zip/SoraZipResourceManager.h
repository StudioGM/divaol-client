#ifndef SORA_ZIP_RESOURCE_MANAGER_H
#define SORA_ZIP_RESOURCE_MANAGER_H

#include "SoraPlatform.h"
#include "SoraException.h"
#include "SoraAutoPtr.h"

#include "SoraZipFile.h"

#include "SoraResourceManager.h"

#include <map>

namespace sora {

	typedef SoraHandle SoraResourceHandle;
	
	class SoraZipResourceManager: public SoraResourceManager {
		typedef std::map<SoraHandle, SoraAutoPtr<SoraZipFile> > RESOURCE_PACK;
		
	public:
		~SoraZipResourceManager();

		SoraResourceHandle	loadResourcePack	(const StringType& file);
		void			attachResourcePack      (SoraResourceHandle handle);
		void			detachResourcePack      (SoraResourceHandle handle);
		SoraZipFile*	getResourcePack         (SoraResourceHandle handle);

		void* readResourceFile				(const StringType& file, uint32 pos, uint32 size);
		void* getResourceFile				(const StringType& file, uint32& size);
		uint32 getResourceFileSize	(const StringType& file);
		uint32 getResourceFileCRC	(const StringType& file);
		void  freeResourceFile(void* p);
		
		StringType getName() const { return "ZipRM"; }
		bool isFormatSupported(const StringType& format) const;
		
		bool enumFiles(std::vector<StringType>& cont, const StringType& folder);
		
	private:
		RESOURCE_PACK resourcePacks;
	};

} // namespace sora;


#endif