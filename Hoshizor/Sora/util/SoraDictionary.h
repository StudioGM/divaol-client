//
//  SoraDictionary.h
//  Sora
//
//  Created by Ruiwei Bu on 11/3/11.
//  Copyright (c) 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraDictionary_h
#define Sora_SoraDictionary_h

#include "SoraArray.h"

namespace sora {
    
    template<typename Key, typename Value>
    class SoraPair {
    public:
        SoraPair() { }
        
        SoraPair(const Key& key, const Value& value):
        mKey(key),
        mValue(value) {
        
        }
 
        SoraPair(const Key& key):
        mKey(key) {
            
        }
        
        Key& key() { return mKey; }
        Value& value() { return mValue; }
        
        Key& first() { return mKey; }
        Value& second() { return mValue; }
        
        const Key& key() const { return mKey; }
        const Value& value() const { return mValue; }
        
        const Key& first() const { return mKey; }
        const Value& second() const { return mValue; }
        
        typedef Key KeyType;
        typedef Value ValueType;
        typedef SoraPair<Key, Value> SelfType;
        
        bool operator == (const SelfType& rhs) const { return this->mKey == rhs.mKey && this->mValue == rhs.mValue; }
        bool operator != (const SelfType& rhs) const { return this->mKey != rhs.mKey; }
        bool operator <= (const SelfType& rhs) const { return this->mKey <= rhs.mKey; }
        bool operator >= (const SelfType& rhs) const { return this->mKey >= rhs.mKey; }
        bool operator < (const SelfType& rhs) const { return this->mKey < rhs.mKey; }
        bool operator > (const SelfType& rhs) const { return this->mKey > rhs.mKey; }
       
        SoraPair& operator=(const SelfType& rhs) {
            this->mKey = rhs.mKey;
            this->mValue = rhs.mValue;
        }
        
    private:
        Key mKey;
        Value mValue;
    };
    
    template<typename Key, typename Value>
    class SoraDictionary {
    public:
        typedef Key KeyType;
        typedef Value ValueType;
        typedef SoraPair<Key, Value> PairType;
        typedef SoraArray<PairType> DictionaryType;
        typedef SoraDictionary<Key, Value> SelfType;
        
        typedef typename DictionaryType::iterator iterator;
        typedef typename DictionaryType::const_iterator const_iterator;
        typedef PairType reference;
        typedef const PairType const_reference;
        
        SoraDictionary();
        SoraDictionary(const SelfType& rhs);
        
        SoraDictionary& operator=(const SelfType& rhs);
        
        void add(const Key& key, const Value& val);
        void del(const Key& key);
        
        void insert(const PairType& pair);
        void insert(const Key& key, const Value& val);
        void remove(const Key& key);
        
        void push_back(const PairType& pair);
        
        void erase(iterator iter);
        
        iterator begin();
        iterator end();
        
        const_iterator begin() const;
        const_iterator end() const;
        
        iterator find(const Key& key) const;
        
        PairType& front() const;
        PairType& back() const;
        
        ValueType& get(const Key& key);
        const ValueType& get(const Key& key) const;
        
        ValueType& at(const Key& key);
        const ValueType& at(const Key& key) const;
        
        ValueType& operator[](const Key& key);
        ValueType& operator[](uint32 index);
        const ValueType& operator[](const Key& key) const;
        const ValueType& operator[](uint32 index) const;
        
        uint32 getIndex(const Key& key);
        ValueType& atIndex(uint32 index) const;
        
        bool contains(const Key& key); 
        
        void beginBulkInsert();
        void endBulkInsert();
        
        SoraArray<Value> valueAsArray() const;
        SoraArray<Key> keyAsArray() const;
        
    private:
        bool mIsBulkInsert;
        
        DictionaryType mDictionary;
    };
    
    template<typename Key, typename Value>
    SoraDictionary<Key, Value>::SoraDictionary():
    mIsBulkInsert(false) {
        
    }
    
    template<typename Key, typename Value>
    SoraDictionary<Key, Value>::SoraDictionary(const SelfType& rhs):
    mIsBulkInsert(false),
    mDictionary(rhs.mDictionary) {
        sora_assert(!rhs.mIsBulkInsert);
    }
    
    template<typename Key, typename Value>
    SoraDictionary<Key, Value>& SoraDictionary<Key, Value>::operator=(const SelfType& rhs) {
        sora_assert(!rhs.mIsBulkInsert);
        
        mDictionary = rhs.mDictionary;
        return *this;
    }
    
    template<typename Key, typename Value>
    void SoraDictionary<Key, Value>::insert(const PairType& pair) {
        if(!mIsBulkInsert)
            mDictionary.insertSorted(pair);
        else
            mDictionary.insert(pair);
    }
    
    template<typename Key, typename Value>
    void SoraDictionary<Key, Value>::add(const Key& key, const Value& val) {
        if(!mIsBulkInsert)
            mDictionary.insertSorted(PairType(key, val));
        else
            mDictionary.insert(PairType(key, val));
    }
    
    template<typename Key, typename Value>
    void SoraDictionary<Key, Value>::del(const Key& key) {
        uint32 index = mDictionary.findIndex(key);
        if(index != DictionaryType::InvalidIndex) {
            mDictionary.erase(index);
        }
    }
    
    template<typename Key, typename Value>
    void SoraDictionary<Key, Value>::insert(const Key& key, const Value& val) {
        if(!mIsBulkInsert)
            mDictionary.insertSorted(PairType(key, val));
        else
            mDictionary.insert(PairType(key, val));
    }
    
    template<typename Key, typename Value>
    void SoraDictionary<Key, Value>::remove(const Key& key) {
        uint32 index = mDictionary.findIndex(key);
        if(index != DictionaryType::InvalidIndex) {
            mDictionary.erase(index);
        }
    }
    
    template<typename Key, typename Value>
    void SoraDictionary<Key, Value>::push_back(const PairType& pair) {
        if(!mIsBulkInsert)
            mDictionary.insertSorted(pair);
        else
            mDictionary.insert(pair);
    }
    
    template<typename Key, typename Value>
    void SoraDictionary<Key, Value>::erase(iterator iter) {
        mDictionary.erase(iter);
    }
    
    template<typename Key, typename Value>
    typename SoraDictionary<Key, Value>::iterator SoraDictionary<Key, Value>::begin() {
        return mDictionary.begin();
    }
    
    template<typename Key, typename Value>
    typename SoraDictionary<Key, Value>::iterator SoraDictionary<Key, Value>::end() {
        return mDictionary.end();
    }
    
    template<typename Key, typename Value>
    typename SoraDictionary<Key, Value>::const_iterator SoraDictionary<Key, Value>::begin() const {
        return mDictionary.begin();
    }
    
    template<typename Key, typename Value>
    typename SoraDictionary<Key, Value>::const_iterator SoraDictionary<Key, Value>::end() const {
        return mDictionary.end();
    }
    
    template<typename Key, typename Value>
    typename SoraDictionary<Key, Value>::PairType& SoraDictionary<Key, Value>::front() const {
        return mDictionary.front();
    }
    
    template<typename Key, typename Value>
    typename SoraDictionary<Key, Value>::PairType& SoraDictionary<Key, Value>::back() const {
        return mDictionary.front();
    }
    
    template<typename Key, typename Value>
    typename SoraDictionary<Key, Value>::ValueType& SoraDictionary<Key, Value>::get(const Key& key) {
        uint32 index = mDictionary.findIndex(key);
        sora_assert(index != DictionaryType::InvalidIndex);
        
        return mDictionary[index];
    }
    
    template<typename Key, typename Value>
    const typename SoraDictionary<Key, Value>::ValueType& SoraDictionary<Key, Value>::get(const Key& key) const {
        uint32 index = mDictionary.findIndex(key);
        sora_assert(index != DictionaryType::InvalidIndex);
        
        return mDictionary[index];
    }
    
    template<typename Key, typename Value>
    typename SoraDictionary<Key, Value>::ValueType& SoraDictionary<Key, Value>::at(const Key& key) {
        uint32 index = mDictionary.findIndex(key);
        sora_assert(index != DictionaryType::InvalidIndex);
        
        return mDictionary[index];
    }
    
    template<typename Key, typename Value>
    const typename SoraDictionary<Key, Value>::ValueType& SoraDictionary<Key, Value>::at(const Key& key) const {
        uint32 index = mDictionary.findIndex(key);
        sora_assert(index != DictionaryType::InvalidIndex);
        
        return mDictionary[index];
    }
    
    template<typename Key, typename Value>
    const typename SoraDictionary<Key, Value>::ValueType& SoraDictionary<Key, Value>::operator[](const Key& key) const {
        uint32 index = mDictionary.findIndex(key);
        sora_assert(index != DictionaryType::InvalidIndex);
        
        return mDictionary[index];
    }
    
    template<typename Key, typename Value>
    const typename SoraDictionary<Key, Value>::ValueType& SoraDictionary<Key, Value>::operator[](uint32 index) const {
        sora_assert(index != DictionaryType::InvalidIndex);
        
        return mDictionary[index];
    }
    
    template<typename Key, typename Value>
    typename SoraDictionary<Key, Value>::ValueType& SoraDictionary<Key, Value>::operator[](const Key& key) {
        uint32 index = mDictionary.findIndex(key);
        sora_assert(index != DictionaryType::InvalidIndex);
        
        return mDictionary[index];
    }
    
    template<typename Key, typename Value>
    typename SoraDictionary<Key, Value>::ValueType& SoraDictionary<Key, Value>::operator[](uint32 index) {
        sora_assert(index != DictionaryType::InvalidIndex);
        
        return mDictionary[index]; 
    }
    
    template<typename Key, typename Value>
    uint32 SoraDictionary<Key, Value>::getIndex(const Key& key) {
        uint32 index = mDictionary.findIndex(key);
        return index;
    }
    
    template<typename Key, typename Value>
    typename SoraDictionary<Key, Value>::ValueType& SoraDictionary<Key, Value>::atIndex(uint32 index) const {
        sora_assert(index != DictionaryType::InvalidIndex);
        
        return mDictionary[index];
    }
    
    template<typename Key, typename Value>
    bool SoraDictionary<Key, Value>::contains(const Key& key) {
        uint32 index = mDictionary.findIndex(key);
        return (index != DictionaryType::InvalidIndex);
    }
    
    template<typename Key, typename Value>
    typename SoraDictionary<Key, Value>::iterator SoraDictionary<Key, Value>::find(const Key& key) const {
        return mDictionary.find(key);
    }
    
    template<typename Key, typename Value>
    void SoraDictionary<Key, Value>::beginBulkInsert() {
        mIsBulkInsert = true;
    }
    
    template<typename Key, typename Value>
    void SoraDictionary<Key, Value>::endBulkInsert() {
        mIsBulkInsert = false;
        mDictionary.sort();
    }
    
    template<typename Key, typename Value>
    SoraArray<Value> SoraDictionary<Key, Value>::valueAsArray() const {
        SoraArray<Value> array;
        iterator it = this->begin();
        iterator end = this>end();
        for(; it != end; ++it) 
            array.insert(it->second());
        return array;
    }
    
    template<typename Key, typename Value>
    SoraArray<Key> SoraDictionary<Key, Value>::keyAsArray() const {
        SoraArray<Value> array;
        iterator it = this->begin();
        iterator end = this>end();
        for(; it != end; ++it) 
            array.insert(it->first());
        return array;
    }
    
} // namespace sora

#endif
