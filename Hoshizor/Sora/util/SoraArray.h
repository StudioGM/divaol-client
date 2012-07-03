//
//  SoraArray.h
//  Sora
//
//  Created by Ruiwei Bu on 10/2/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraArray_h
#define Sora_SoraArray_h

#include "SoraPlatform.h"
#include "SoraAutoPtr.h"

namespace sora {
  
    /**
     * A Simple expandable array class
     * Can get the data pointer directly
     * Used with most collection classes
     **/
    
    template<typename T>
    class SoraArray {
    public:
        typedef T* iterator;
        typedef const T* const_iterator;
        typedef T reference;
        typedef const T const_reference;
        
        typedef SoraArray<T> self_type;
        
        enum { InvalidIndex = -1 };
        
        SoraArray();
        SoraArray(uint32 capcity, uint32 grow);
        SoraArray(uint32 size, uint32 grow, const T& value);
        SoraArray(const SoraArray<T>& rhs);
        SoraArray(const T* arr, uint32 size);
        ~SoraArray();
        
        void operator=(const SoraArray<T>& rhs);
        T& operator[](uint32 index) const;
        
        bool operator==(const SoraArray<T>& rhs) const;
        bool operator!=(const SoraArray<T>& rhs) const;
        
        void append(const T& elm);
        void appendArray(const SoraArray<T>& rhs);
        void append(const T* arr, uint32 size);
        
        void reserve(uint32 num);
        uint32 size() const;
        uint32 capacity() const;
        
        T& front() const;
        T& back() const;
        
        bool empty() const;
        
        void erase(uint32 index);
        iterator erase(iterator iter);
        
        void insert(uint32 index, const T& elm);
        void insert(const T& elm);
        
        void push_back(const T& elm);
        void push_front(const T& elm);
        void pop_back();
        void pop_front();
        
        iterator begin();
        iterator end();
        
        const_iterator begin() const;
        const_iterator end() const;
        
        void clear();
        void reset();
        
        iterator find(const T& elm) const;
        uint32 findIndex(const T& elm) const;
        
        void fill(uint32 start, uint32 num, const T& elm);
        void realloc(uint32 size, uint32 grow);
        
         /* Sort and Search */
        void insertSorted(const T& elm);
        bool isSorted() const;
        void sort() const;
        uint32 binarySearchIndex(const T& elm);
        
        void map(T* mappedPtr, uint32 size);
        
    private:
        void destroyAll();
        void destroy(T* elm);
        void copy(const SoraArray<T>& src);
        void grow();
        void growTo(uint32 capacity);
        void move(uint32 index1, uint32 index2);
        
        uint32 mGrow;
        uint32 mCapacity;
        uint32 mSize;
        T* mElements;
        
        typedef SoraAutoPtr<T, autoptr::RefCounter, autoptr::FreeReleasePolicy> AutoPtrType;
        AutoPtrType mHolder;
        
        bool mMapped;
    };
    
    template<typename T>
    SoraArray<T>::SoraArray():
    mGrow(0),
    mCapacity(0),
    mSize(0),
    mElements(0),
    mMapped(false) {
        
    }
    
    template<typename T>
    SoraArray<T>::SoraArray(uint32 capacity, uint32 grow):
    mGrow(grow),
    mCapacity(capacity),
    mSize(0),
    mMapped(false) {
        if(capacity != 0) {
            this->mElements = sora_malloc_t(T, capacity);
            this->mHolder = this->mElements;
            this->mSize = this->mCapacity;
        } else 
            this->mElements = 0;
    }
    
    template<typename T>
    SoraArray<T>::SoraArray(uint32 size, uint32 grow, const T& value):
    mGrow(grow),
    mCapacity(size),
    mSize(size),
    mMapped(false) {
        if(size > 0) {
            this->mElements = (T*)sora_malloc(sizeof(T) * this->mCapacity);
            for(int32 i = 0; i < this->mCapacity; ++i) {
                this->mElements[i] = value;
            }
            this->mHolder = this->mElements;
        } else {
            this->mElements = 0;
        }
    }
    
    template<typename T>
    SoraArray<T>::SoraArray(const T* arr, uint32 size):
    mGrow(0),
    mCapacity(0),
    mSize(0),
    mMapped(false) {
        sora_assert(size > 0);
        
        reserve(size);
        memcpy(this->mElements, arr, sizeof(T) * size);
        this->mSize = size;
    }
    
    template<typename T>
    void SoraArray<T>::copy(const SoraArray<T>& src) {
        sora_assert(this->mElements == 0);
        
        this->mGrow = src.mGrow;
        this->mCapacity = src.mCapacity;
        this->mSize = src.mSize;
        if(this->mCapacity > 0) {
            this->mElements = (T*)sora_malloc(sizeof(T) * this->mCapacity);
            for(int32 i=0; i<this->mSize; ++i) {
                this->mElements[i] = src.mElements[i];
            }
            this->mHolder = this->mElements;
        }
    }
    
    template<typename T>
    void SoraArray<T>::clear() {
        this->mGrow = 0;
        this->mCapacity = 0;
        this->mSize = 0;
        if(this->mElements) {
            // reset auto ptr
            this->mHolder.reset();
            
            this->mElements = 0;
        }
    }
    
    template<typename T>
    void SoraArray<T>::map(T* mappedPtr, uint32 size) {
        clear();
        
        this->mCapacity = this->mSize = size;
        this->mElements = mappedPtr;
        this->mMapped = true;
    }
    
    template<typename T>
    void SoraArray<T>::destroy(T* ele) {
        ele->~T();
    }
    
    template<typename T>
    SoraArray<T>::SoraArray(const SoraArray<T>& rhs):
    mGrow(0),
    mCapacity(0),
    mSize(0),
    mElements(0),
    mMapped(rhs.mMapped) {
        *this = rhs;
    }
    
    template<typename T>
    SoraArray<T>::~SoraArray() {
        
    }
    
    template<typename T>
    void SoraArray<T>::realloc(uint32 capacity, uint32 grow) {
        this->destroyAll();
        this->mGrow = grow;
        this->mCapacity = capacity;
        this->mMapped= false;
        this->mSize = 0;
        if(this->mCapacity > 0) {
            this->mElements = (T*)sora_malloc(sizeof(T) * this->mCapacity);
            this->mHolder = this->mElements;
        } else {
            this->mElements = 0;
        }
    }
    
    template<typename T>
    void SoraArray<T>::operator=(const SoraArray<T>& rhs) {
        if(this != &rhs) {
            this->mGrow = rhs.mGrow;
            this->mCapacity = rhs.mCapacity;
            this->mSize = rhs.mSize;
            this->mMapped = rhs.mMapped;
            if(this->mCapacity > 0) {
                this->mElements = rhs.mElements;
                this->mHolder = rhs.mHolder;
            }
        }
    }
    
    template<typename T>
    void SoraArray<T>::growTo(uint32 newCapacity) {
        sora_assert(!this->mMapped);
        
        T* newArray = (T*)sora_malloc(sizeof(T) * newCapacity);
        if(this->mElements) {
            memcpy(newArray, this->mElements, sizeof(T) * this->mCapacity);
        }
        this->mElements = newArray;
        this->mHolder.reset(newArray);

        this->mCapacity = newCapacity;
    }
    
    template<typename T>
    void SoraArray<T>::grow() {
        uint32 toSize;
        if(this->mCapacity == 0) {
            if(this->mGrow == 0)
                toSize = 1;
            else {
                toSize = this->mGrow;
            }
        } else {
            toSize = this->mCapacity * 2;
        }
        this->growTo(toSize);
    }
    
    template<typename T>
    void SoraArray<T>::append(const T* arr, uint32 size) {
        sora_assert(size > 0);
        
        uint32 newSize = this->mSize + size;
        if(newSize >= this->mCapacity) {
            this->growTo(newSize);
        }
        memcpy(this->mElements + this->mSize, arr, size);
        this->mSize = newSize;
    }
    
    template<typename T>
    void SoraArray<T>::move(uint32 from, uint32 to) {
        sora_assert(this->mElements);
        sora_assert(from < this->mSize);
        sora_assert(!this->mMapped);
        
        if(from == to)
            return;
        
        uint32 num = this->mSize - from;
        uint32 neededSize = to + num;
        while(neededSize < this->mCapacity) {
            this->grow();
        }
        
        if(from > to) {
            // backward move
            int32 i;
            for(i=0; i<num; ++i) {
                this->mElements[to + i] = this->mElements[from + i];
            }
            
            for(i=(from+i)-1; i<this->mSize; ++i) {
                this->destroy(&(this->mElements[i]));
            }
        } else {
            int i=0;
            for(i=num-1; i>=0; --i) {
                this->mElements[to + i] = this->mElements[from + i];
            }
            
            for(i=int(from); i<int(to); ++i) {
                this->destroy(&(this->mElements[i]));
            }
        }
        
        this->mSize = to + num;
    }
    
    template<typename T>
    void SoraArray<T>::append(const T& elemt) {
        if(this->mSize == this->mCapacity) {
            this->grow();
        }
        sora_assert(this->mElements);
        this->mElements[this->mSize++] = elemt;
    }
    
    template<typename T>
    void SoraArray<T>::push_back(const T& elemt) {
        this->append(elemt);
    }
    
    template<typename T>
    void SoraArray<T>::pop_back() {
        sora_assert(this->mElements);
        sora_assert(this->mSize > 0);
        
        this->destroy(this->mElements[this->mSize-1]);
        this->mSize--;
    }
    
    template<typename T>
    void SoraArray<T>::appendArray(const SoraArray<T>& rhs) {
        uint32 size = rhs.size();
        for(int32 i=0; i<size; ++i) {
            this->append(rhs[i]);
        }
    }
    
    template<typename T>
    void SoraArray<T>::reserve(uint32 num) {
        sora_assert(num > 0);
        uint32 neededCapacity = this->mSize + num;
        if(neededCapacity > this->mCapacity) {
            this->growTo(neededCapacity);
        }
    }
    
    template<typename T>
    uint32 SoraArray<T>::size() const {
        return this->mSize;
    }
    
    template<typename T>
    uint32 SoraArray<T>::capacity() const {
        return this->mCapacity;
    }
    
    template<typename T>
    T& SoraArray<T>::operator[](uint32 index) const {
        sora_assert(index < this->mSize);
        sora_assert(this->mElements);
        
        return this->mElements[index];
    }
    
    template<typename T>
    bool SoraArray<T>::operator==(const SoraArray<T>& rhs) const {
        if(rhs.size() == this->size()) {
            for(int32 i=0; i<this->mSize(); ++i) {
                if(!(this->mElements[i] == rhs.mElements[i]))
                    return false;
            }
            return true;
        }
        return false;
    }
    
    template<typename T>
    bool SoraArray<T>::operator!=(const SoraArray<T>& rhs) const {
        return !(*this == rhs);
    }
    
    template<typename T>
    T& SoraArray<T>::front() const {
        sora_assert(this->mElements);
        sora_assert(this->mSize > 0);
        
        return this->mElements[0];
    }
    
    template<typename T>
    T& SoraArray<T>::back() const {
        sora_assert(this->mElements);
        sora_assert(this->mSize > 0);
        
        return this->mElements[this->mSize - 1];
    }
    
    template<typename T>
    bool SoraArray<T>::empty() const {
        return this->mSize == 0;
    }
    
    template<typename T>
    typename SoraArray<T>::iterator SoraArray<T>::erase(typename SoraArray<T>::iterator iter) {
        sora_assert(this->mElements);
        sora_assert(iter < (this->mElements + this->mSize));
        sora_assert(iter >= this->mElements);
        sora_assert(!this->mMapped);
        
        this->erase(uint32(iter - this->mElements));
        return iter;
    }
    
    template<typename T>
    void SoraArray<T>::erase(uint32 index) {
        sora_assert(this->mElements);
        sora_assert(index < this->mSize);
        sora_assert(!this->mMapped);
        
        if(index == this->mSize-1) {
            this->destroy(&(this->mElements[index]));
            this->mSize--;
        } else {
            this->move(index+1, index);
        }
    }
    
    template<typename T>
    void SoraArray<T>::insert(uint32 index, const T& elemt) {
        sora_assert(index <= this->mSize);
        sora_assert(!this->mMapped);
        
        if(index == this->mSize) 
            this->append(elemt);
        else {
            this->move(index, index+1);
            this->mElements[index] = elemt;
        }
    }
    
    template<typename T>
    void SoraArray<T>::pop_front() {
        sora_assert(this->mElements);
        sora_assert(this->mSize >= 1);
        sora_assert(!this->mMapped);
        
        if(this->mSize == 1)
            this->clear();
        else {
            this->move(1, 0);
        }
    }
    
    template<typename T>
    void SoraArray<T>::destroyAll() {
        for(int32 i=0; i<this->mSize; ++i) 
            this->destroy(&(this->mElements[i]));
        this->mSize = 0;
    }
    
    template<typename T>
    void SoraArray<T>::reset() {
        this->mSize = 0;
    }
    
    template<typename T>
    typename SoraArray<T>::const_iterator SoraArray<T>::begin() const {
        return this->mElements;
    }
    
    template<typename T>
    typename SoraArray<T>::const_iterator SoraArray<T>::end() const {
        return this->mElements + this->mSize;
    }
    
    template<typename T>
    typename SoraArray<T>::iterator SoraArray<T>::begin() {
        return this->mElements;
    }
    
    template<typename T>
    typename SoraArray<T>::iterator SoraArray<T>::end() {
        return this->mElements + this->mSize;
    }
    
    template<typename T>
    typename SoraArray<T>::iterator SoraArray<T>::find(const T& elemt) const {
        for (int32 i=0; i<this->mSize; ++i) {
            if(this->mElements[i] == elemt)
                return &(this->mElements[index]);
        }
        return 0;
    }
    
    template<typename T>
    uint32 SoraArray<T>::findIndex(const T& elemt) const {
        for(int32 i=0; i<this->mSize; ++i) {
            if(this->mElements[i] == elemt)
                return i;
        }
        return -1;
    }
    
    template<typename T>
    void SoraArray<T>::fill(uint32 index, uint32 num, const T& elemt) {
        if(index + num > this->mSize)
            this->growTo(index + num);
        
        for(int32 i=0; i<index+num; ++i) {
            this->mElements[i] = elemt;
        }
    }
    
    template<typename T>
    void SoraArray<T>::push_front(const T& elemt) {
        if(this->mSize == 0) {
            grow();
            this->mElements[0] = elemt;
            this->mSize++;
        } else {
            this->move(0, 1);
            this->mElements[0] = elemt;
            this->mSize++;
        }
    }
    
    template<typename T>
    void SoraArray<T>::insertSorted(const T& elm) {
        if(this->mSize == 0) 
            this->append(elm);
        else {
            uint32 num = this->mSize;
            uint32 half;
            uint32 lo = 0;
            uint32 hi = this->mSize - 1;
            uint32 mid;
            while(lo < hi) {
                if(0 != (half = num/2)) {
                    mid = lo + ((num & 1) ? half : (half - 1));
                    if(elm < this->mElements[mid]) {
                        hi = mid - 1;
                        num = num & 1 ? half : half - 1;
                    } else if(elm > this->mElements[mid]) {
                        lo = mid + 1;
                        num = half;
                    } else {
                        uint32 i = mid +1;
                        for(; i<this->mSize; ++i) {
                            if(this->mElements[i] != elm) {
                                this->insert(i, elm);
                                return;
                            }
                        }
                        this->append(elm);
                        return;
                    }
                } else if(0 != num) {
                    if(elm < this->mElements[lo]) {
                        this->insert(lo, elm);
                        return;
                    } else if(elm > this->mElements[lo]) {
                        this->insert(lo+1, elm);
                        return;
                    } else {
                        uint32 i = lo +1;
                        for(; i<this->mSize; ++i) {
                            if(this->mElements[i] != elm) {
                                this->insert(i, elm);
                                return;
                            }
                        }
                        this->append(elm);
                        return;
                    }
                    
                } else {
                    sora_assert(0 == lo);
                    this->insert(lo, elm);
                    return;
                }
            }
            if(elm < this->mElements[lo]) {
                this->insert(lo, elm);
                return;
            } else if(elm > this->mElements[lo]) {
                this->insert(lo+1, elm);
                return;
            } else {
                // can't happen
                sora_assert(false);
            }
        }
    }
    
    template<typename T>
    bool SoraArray<T>::isSorted() const {
        if(this->mSize > 1) {
            for(uint32 i=0; i<this->mSize; ++i) {
                if(this->mElements[i] > this->mElements[i+1]) 
                    return false;
            }
        }
        return true;
    }
   
    template<typename T>
    void SoraArray<T>::sort() const {
        std::sort(this->begin(), this->end());
    }
    
    template<typename T>
    uint32 SoraArray<T>::binarySearchIndex(const T& elm) {
        if(this->mSize > 0) {
            uint32 num = this->mSize;
            uint32 half;
            uint32 lo;
            uint32 hi = this->mSize - 1;
            uint32 mid;
            while(lo < hi) {
                if(0 != (half = num/2)) {
                    mid = lo + ((num & 1) ? half : (half-1));
                    if(elm < this->mElements[mid]) {
                        hi = mid-1;
                        num = num & 1 ? half : half - 1;
                    } else if(elm > this->mElements[mid]) {
                        lo = mid+1;
                        num = half;
                    } else {
                        return mid;
                    }
                } else if(0 != num) {
                    if(elm != this->mElements[lo]) 
                        return InvalidIndex;
                    else 
                        return lo;
                } else 
                    break;
            }
        }
        return InvalidIndex;
    }
    
} // namespace sora

#endif
