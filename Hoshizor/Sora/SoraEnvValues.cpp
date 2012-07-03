/*
 *  SoraEnvValues.cpp
 *  Sora
 *
 *  Created by Griffin Bu on 5/2/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#include "SoraEnvValues.h"

namespace sora {

	SoraEnvValues* SoraEnvValues::mInstance = NULL;

	SoraEnvValues* SoraEnvValues::Instance() {
		if(!mInstance)
			mInstance = new SoraEnvValues;
		return mInstance;
	}

	void SoraEnvValues::Destroy() {
		if(mInstance) {
			delete mInstance;
			mInstance = NULL;
		}
	}

	bool SoraEnvValues::getValue(SoraStringId name, bool defaultVal) {
		VALUE_ITERATOR itVal = values.find(name);
		if(itVal != values.end()) {
			switch(itVal->second.VALUE_TYPE) {
				case VALUE_BOOL: return itVal->second.data.boolVal;
				case VALUE_INT: return itVal->second.data.intVal>0?true:false;
				default: break;
			}
		}
		return defaultVal;
	}
	
	int32 SoraEnvValues::getValue(SoraStringId name, int32 defaultVal) {
		VALUE_ITERATOR itVal = values.find(name);
		if(itVal != values.end()) {
			switch(itVal->second.VALUE_TYPE) {
				case VALUE_BOOL: return itVal->second.data.boolVal?1:0;
				case VALUE_INT: return itVal->second.data.intVal;
				case VALUE_FLOAT: return (int32)itVal->second.data.floatVal;
				default: break;
			}
		}
		return defaultVal;
	}
	
	float	SoraEnvValues::getValue(SoraStringId name, float defaultVal) {
		VALUE_ITERATOR itVal = values.find(name);
		if(itVal != values.end()) {
			switch(itVal->second.VALUE_TYPE) {
				case VALUE_BOOL: return itVal->second.data.boolVal?1.f:0.f;
				case VALUE_INT: return (float)itVal->second.data.intVal;
				case VALUE_FLOAT: return itVal->second.data.floatVal;
				default: break;
			}
		}
		return defaultVal;
	}
	
	std::string	SoraEnvValues::getValue(SoraStringId name, const std::string& defaultVal) {
		VALUE_ITERATOR itVal = values.find(name);
		if(itVal != values.end()) {
			switch(itVal->second.VALUE_TYPE) {
				case VALUE_STRING: return id2str(itVal->second.data.stringVal);
				default: break;
			}
		}
		return defaultVal;
	}
	
	std::wstring SoraEnvValues::getValue(SoraStringId name, const std::wstring& defaultVal) {
		VALUE_ITERATOR itVal = values.find(name);
		if(itVal != values.end()) {
			switch(itVal->second.VALUE_TYPE) {
				case VALUE_STRING: return id2strw(itVal->second.data.stringVal);
				default: break;
			}
		}
		return defaultVal;
	}
	
	void* SoraEnvValues::getValue(SoraStringId name) {
		VALUE_ITERATOR itVal = values.find(name);
		if(itVal != values.end()) {
			switch(itVal->second.VALUE_TYPE) {
				case VALUE_USERDATA:
					return itVal->second.userData;
			}
		}
		return NULL;
	}
	
	void SoraEnvValues::setValue(SoraStringId name, bool val) {
		VALUE_STORE myData;
		myData.VALUE_TYPE = VALUE_BOOL;
		myData.data.boolVal = val;
		values[name] = myData;
	}
	
	void SoraEnvValues::setValue(SoraStringId name, int32 val) {
		VALUE_STORE myData;
		myData.VALUE_TYPE = VALUE_INT;
		myData.data.intVal = val;
		values[name] = myData;
	}
	
	void SoraEnvValues::setValue(SoraStringId name, float val) {
		VALUE_STORE myData;
		myData.VALUE_TYPE = VALUE_FLOAT;
		myData.data.floatVal = val;
		values[name] = myData;
	}
	
	void SoraEnvValues::setValue(SoraStringId name, const std::string& val) {
		VALUE_STORE myData;
		myData.VALUE_TYPE = VALUE_STRING;
		myData.data.stringVal = GetUniqueStringId(val);
		values[name] = myData;
	}
	
	void SoraEnvValues::setValue(SoraStringId name, const std::wstring& val) {
		VALUE_STORE myData;
		myData.VALUE_TYPE = VALUE_STRING;
		myData.data.stringVal = GetUniqueStringId(val);
		values[name] = myData;
	}
	
	void SoraEnvValues::setValue(SoraStringId name, void* data) {
		VALUE_STORE myData;
		myData.VALUE_TYPE = VALUE_USERDATA;
		myData.userData = data;
		values[name] = myData;
	}
	
	bool SoraEnvValues::getBool(const std::string& name, bool defaultVal) {
		SoraStringId sname = GetUniqueStringId(name);
		return getValue(sname, defaultVal);
	}
	
	int32 SoraEnvValues::getInt(const std::string& name, int32 defaultVal) {
		SoraStringId sname = GetUniqueStringId(name);
		return getValue(sname, defaultVal);
	}
	
	float	SoraEnvValues::getFloat(const std::string& name, float defaultVal) {
		SoraStringId sname = GetUniqueStringId(name);
		return getValue(sname, defaultVal);
	}
	
	std::string	SoraEnvValues::getString(const std::string& name, const std::string& defaultVal) {
		SoraStringId sname = GetUniqueStringId(name);
		return getValue(sname, defaultVal);
	}
	
	std::wstring SoraEnvValues::getWString(const std::string& name, const std::wstring& defaultVal) {
		SoraStringId sname = GetUniqueStringId(name);
		return getValue(sname, defaultVal);
	}
	
	void* SoraEnvValues::getData(const std::string& name) {
		SoraStringId sname = GetUniqueStringId(name);
		return getValue(sname);
	}
	
	void SoraEnvValues::setBool(const std::string& name, bool val) {
		setValue(GetUniqueStringId(name), val);
	}
	
	void SoraEnvValues::setInt(const std::string& name, int32 val) {
		setValue(GetUniqueStringId(name), val);
	}
	
	void SoraEnvValues::setFloat(const std::string& name, float val) {
		setValue(GetUniqueStringId(name), val);
	}
	
	void SoraEnvValues::setString(const std::string& name, const std::string& val) {
		setValue(GetUniqueStringId(name), val);
	}
	
	void SoraEnvValues::setWString(const std::string& name, const std::wstring& val) {
		setValue(GetUniqueStringId(name), val);
	}
	
	void SoraEnvValues::setData(const std::string& name, void* data) {
		setValue(GetUniqueStringId(name), data);
	}
	
	void SoraEnvValues::removeData(const std::string& name) {
		setValue(GetUniqueStringId(name), (void*)NULL);
	}
	
	void SoraEnvValues::removeData(SoraStringId name) {
		setValue(name, (void*)NULL);
	}
	
	

} // namespace sora