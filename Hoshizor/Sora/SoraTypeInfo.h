#ifndef SORA_TYPE_INFO
#define SORA_TYPE_INFO

#include "SoraPlatform.h"
#include "SoraStringId.h"

#include <typeinfo>

namespace sora {
	
	/*
	 if RTTI has been disabled, then we use SoraStringId as a unique identifier
	 */
	
#if defined(SORA_USE_RTTI)

	class SORA_API SoraTypeInfo {
	public:
		explicit SoraTypeInfo(const std::type_info& info) : _typeInfo(info) {};

		bool operator < (const SoraTypeInfo& rhs) const {
			return _typeInfo.before(rhs._typeInfo) != 0;
		}
        
        std::string name() {
            return _typeInfo.name();
        }

	private:
		const std::type_info& _typeInfo;
	};
	
#else 
	
	class SORA_API SoraTypeInfo {
	public:
		explicit SoraTypeInfo(SoraStringId info): _typeInfo(info) {};
		
		bool operator < (const SoraTypeInfo& rhs) const {
			return _typeInfo < rhs._typeInfo;
		}
        
        std::string name() {
            return GetStringByUniqueId(_typeInfo);
        }
		
	private:
		SoraStringId _typeInfo;
	};
	
#endif // SORA_USE_RTTI

} // namespace sora

#endif