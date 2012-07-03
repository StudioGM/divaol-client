#ifndef SORA_FILE_H
#define SORA_FILE_H

#include <fstream>

#include "SoraPlatform.h"
#include "SoraString.h"

namespace sora {

	class SORA_API SoraFileBase {
    public:
		SoraFileBase(): 
        mIsOpen(false) {
        }
		
        virtual ~SoraFileBase() {
        }
		
		bool isOpen() const { 
            return mIsOpen;
        }
		void  setOpen(bool flag) { 
            mIsOpen = flag;
        }
        
        StringType getFilePath() const {
            return mFilePath;
        }
        
        void setFilePath(const StringType& path) {
            mFilePath = path;
        }

	private:
		StringType mFilePath;

		bool mIsOpen;
	};

} // namespace sora

#endif // SORA_FILE_H