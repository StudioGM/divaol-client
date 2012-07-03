/*
 *  common.h
 *  Sora
 *
 *  Created by GriffinBu on 1/20/11.
 *  Copyright 2011 Griffin Bu(Project L). All rights reserved.
 *
 */

#ifndef SORA_COMMON_H_
#define SORA_COMMON_H_

namespace sora {
	
	// example
	// for_each(STL_CONT.begin(), STL_CONT.end(), DeleteSTLPtr());
	// for_each(STL_MAP.begin(), STL_MAP.end(), DeleteSTLMapPtr());
	
	struct DeleteSTLPtr {
		template<typename T>
		void operator() (const T* ptr) const {
			if(ptr) {
				delete ptr;
				ptr = 0;
			}
		}
	};
	
	struct DeleteSTLPairPtr {
		template<typename Ty1, typename Ty2>
		void operator() (const std::pair<Ty1, Ty2>& ptr) const {
			if(ptr.second) {
				delete ptr.second;
			}
		}
	};
						
    // interface for read & write anything into a file
	
    template<typename T>
    inline T cFileReadT(FILE* file, bool& err) {
        T i;
        fread(&i, sizeof(T), 1, file);
        if(ferror(file))
            err = true;
        return i;
    }
    
    template<typename T>
    inline bool cFileWriteT(FILE* file, T i) {
        fwrite(&i, sizeof(T), 1, file);
        if(ferror(file))
            return true;
        return false;
    }
} // namespace sora

#endif