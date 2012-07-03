/*
 *  SoraEnvValues.h
 *  Sora
 *
 *  Created by Griffin Bu on 5/2/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef SORA_ENV_VALUES_H_
#define SORA_ENV_VALUES_H_

#include "SoraPlatform.h"
#include "SoraStringId.h"

#include "SoraSingleton.h"
#include <string>
#include <map>

namespace sora {
	
	class SORA_API SoraEnvValues {
		friend class EnvValuesExporter;
		
	protected:
		SoraEnvValues() {}
		~SoraEnvValues() {}
		
	public:
		static SoraEnvValues* Instance();
		static void Destroy();

		bool			getValue		(SoraStringId name, bool defaultVal);
		int32			getValue		(SoraStringId name, int32 defaultVal);
		float			getValue		(SoraStringId name, float defaultVal);
		std::string		getValue		(SoraStringId name, const std::string& defaultVal);
		std::wstring	getValue		(SoraStringId name, const std::wstring& defaultVal);
		void*			getValue		(SoraStringId name);
		
		void setValue	(SoraStringId name, bool val);
		void setValue	(SoraStringId name, int32 val);
		void setValue	(SoraStringId name, float val);
		void setValue	(SoraStringId name, const std::string& val);
		void setValue	(SoraStringId name, const std::wstring& val);
		void setValue	(SoraStringId name, void* data);
		
		// for script export
		// because there's no polyphormism in scripts
		// and it's hard in scripts to use string id
		bool			getBool		(const std::string& name, bool defaultVal);
		int32			getInt		(const std::string& name, int32 defaultVal);
		float			getFloat	(const std::string& name, float defaultVal);
		std::string		getString	(const std::string& name, const std::string& defaultVal);
		std::wstring	getWString	(const std::string& name, const std::wstring& defaultVal);
		void*			getData		(const std::string& name);
		
		void setBool		(const std::string& name, bool val);
		void setInt			(const std::string& name, int32 val);
		void setFloat		(const std::string& name, float val);
		void setString		(const std::string& name, const std::string& val);
		void setWString		(const std::string& name, const std::wstring& val);
		void setData		(const std::string& name, void* data);
		
		void removeData		(const std::string& name);
		void removeData		(SoraStringId name);

	private:
		static SoraEnvValues* mInstance;
		enum {
			VALUE_INT = 1,
			VALUE_BOOL = 2,
			VALUE_FLOAT = 3,
			VALUE_STRING = 4,
			VALUE_USERDATA = 5,
		};
		
		struct VALUE_STORE {
			union {
				bool boolVal;
				int32 intVal;
				float floatVal;
				SoraStringId stringVal;
			} data;
			
			void* userData;
			uint8 VALUE_TYPE;
		};
		
		typedef std::map<SoraStringId, VALUE_STORE> GLOBAL_VALUE_MAP;
		typedef GLOBAL_VALUE_MAP::iterator VALUE_ITERATOR;
		GLOBAL_VALUE_MAP values;
	};
	
	#define SET_ENV(name, val)		SoraEnvValues::Instance()->setValue(name, val) 
	#define GET_ENV(name, default)	SoraEnvValues::Instance()->getValue(name, default)
	
	#define SET_ENV_INT(name, val)		SoraEnvValues::Instance()->setInt(name, val)
	#define SET_ENV_FLOAT(name, val)	SoraEnvValues::Instance()->setFloat(name, val)
	#define SET_ENV_BOOL(name, val)		SoraEnvValues::Instance()->setBool(name, val)
	#define SET_ENV_STRING(name, val)	SoraEnvValues::Instance()->setString(name, val)
	#define SET_ENV_WSTRING(name, val)	SoraEnvValues::Instance()->setWString(name, val)
	#define SET_ENV_DATA(name, data)	SoraEnvValues::Instance()->setData(name, data)
	
	#define GET_ENV_INT(name, val)		SoraEnvValues::Instance()->getInt(name, val)
	#define GET_ENV_FLOAT(name, val)	SoraEnvValues::Instance()->getFloat(name, val)
	#define GET_ENV_BOOL(name, val)		SoraEnvValues::Instance()->getBool(name, val)
	#define GET_ENV_STRING(name, val)	SoraEnvValues::Instance()->getString(name, val)
	#define GET_ENV_WSTRING(name, val)	SoraEnvValues::Instance()->getWString(name, val)
	#define GET_ENV_DATA(name)			SoraEnvValues::Instance()->getData(name)
	
	#define REMOVE_ENV_DATA(name)	SoraEnvValues::Instance()->removeData(name)
    
} // namespace sora


#endif // SORA_GLOBAL_VALUES_H_