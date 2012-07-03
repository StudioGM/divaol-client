/*
 *  AutoList.h
 *  Sora
 *
 *  Created by GriffinBu on 1/20/11.
 *  Copyright 2011 Griffin Bu(Project L). All rights reserved.
 *
 */

#ifndef SORA_AUTO_CONTAINER_H_
#define SORA_AUTO_CONTAINER_H_

#include <list>
#include <map>
#include <vector>

namespace sora {
	
	/*
	 Inherit objects from this to create a obj list,
	 whenever the object is created, it will be add to the list
	 whenever the object is destroyed, it will deleted from the list
	 */
	
	template<typename T>
	class AutoListElement {
	public:
		typedef typename std::list<T*> Members;
        typedef typename Members::iterator iterator;
        typedef typename Members::const_iterator const_iterator;
        
        
        static iterator begin() {
            return members.begin();
        }
        
        static iterator end() {
            return members.end();
        }
        
        static const_iterator const_begin() {
            const Members& m = members;
            return m.begin();
        }
        
        static const_iterator const_end() {
            const Members& m = members;
            return m.end();
        }
        
	protected:
		AutoListElement() {
			members.push_back(static_cast<T*> (this));
		}
											  
		
		~AutoListElement() {
			members.remove(static_cast<T*> (this));
		}
									
	public:		
		static Members members;
	};
    
    template<typename T>
    typename AutoListElement<T>::Members AutoListElement<T>::members;
    
    /**
     * Inherit bject form this to create a obj list
     * You must call insert and remove by yourself to get the object insert into 
     * the list
     **/
    template<typename T>
    class AutoListElement2 {
    public:
		typedef typename std::list<T*> Members;
        typedef typename Members::iterator iterator;
        typedef typename Members::const_iterator const_iterator;
        
        void link() {
            if(!mInList) {
                members.push_back(static_cast<T*>(this));
                mInList = true;
            }
        }
        
        void unlink() {
            if(mInList) {
                members.remove(static_cast<T*>(this));
                mInList = false;
            }
        }
        
        void isLinked() const {
            return mInList;
        }
        
        static iterator begin() {
            return members.begin();
        }
        
        static iterator end() {
            return members.end();
        }
        
        static const_iterator const_begin() {
            const Members& m = members;
            return m.begin();
        }
        
        static const_iterator const_end() {
            const Members& m = members;
            return m.end();
        }
        
        
    public:
        static Members members;
        
    private:
        bool mInList;
    };
    
    template<typename T>
    typename AutoListElement2<T>::Members AutoListElement2<T>::members;

  	
} // namespace sora


#endif