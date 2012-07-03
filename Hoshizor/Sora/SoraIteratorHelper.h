//
//  SoraIteratorHelper.h
//  Sora
//
//  Created by Robert Bu on 8/19/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraIteratorHelper_h
#define Sora_SoraIteratorHelper_h

namespace sora {
    
    /**
     * Helper class to iterator objects in a container
     * Container iterator must have ++ and -- operator
     **/
    
    template<typename T>
    class ConstVectorIterator {
    public:        
        typedef typename T::const_iterator ConstIteratorType;
        typedef typename T::value_type ValueType;

        ConstVectorIterator(const T& container):
        mCurr(container.begin()),
        mEnd(container.end()) {
            
        }
        
        ConstVectorIterator(ConstIteratorType start, ConstIteratorType end):
        mCurr(start),
        mEnd(end) {
            
        }
        
        bool hasMoreElements() {
            return mCurr != mEnd;
        }
        
        ValueType getNext() {
            return *mCurr++;
        }
        
        const ValueType& getNextRef() {
            return *mCurr++;
        }
        
        ValueType peekNext() {
            return *mCurr;
        }
        
        void moveNext() {
            mCurr++;
        }
        
    private:
        ConstIteratorType mEnd;
        ConstIteratorType mCurr;
    };
    
    template<typename T>
    class VectorIterator {
    public:        
        typedef typename T::iterator IteratorType;
        typedef typename T::value_type ValueType;
        
        VectorIterator(const T& container):
        mCurr(container.begin()),
        mEnd(container.end()) {
            
        }
        
        VectorIterator(IteratorType start, IteratorType end):
        mCurr(start),
        mEnd(end) {
            
        }
        
        bool hasMoreElements() {
            return mCurr != mEnd;
        }
        
        ValueType getNext() {
            return *mCurr++;
        }
        
        const ValueType& getNextRef() {
            return *mCurr++;
        }
        
        ValueType peekNext() {
            return *mCurr;
        }
        
        void moveNext() {
            mCurr++;
        }
        
    private:
        IteratorType mEnd;
        IteratorType mCurr;
    };
    
    // helper macro to define a iterable class
    // container type must have both iterator and const_iterator;
#define SORA_ITERABLE(type, cont) \
    typedef type::iterator iterator; \
    typedef type::const_iterator const_iterator; \
    inline iterator begin() { \
        return cont.begin(); \
    } \
    inline iterator end() { \
        return cont.end(); \
    } \
    inline const_iterator begin() const { \
        return cont.begin(); \
    } \
    inline const_iterator end() const { \
        return cont.end(); \
    }
    
}

#endif
