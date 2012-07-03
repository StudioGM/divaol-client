#ifndef TIMER_H_
#define TIMER_H_

#include "SoraPlatform.h"
#include "SoraException.h"
#include "SoraEvent.h"

namespace sora {
	
	class SORA_API SoraTimerEvent: public SoraEvent {
	public:
		float getTime() const { 
            return this->time;
        }
		void setTime(float _time) { 
            this->time = _time;
        }
		
		float getTotalTime() const { 
            return this->totalTime;
        }
		void setTotalTime(float _totalTime) { 
            this->totalTime = _totalTime;
        }
	
#ifndef SORA_USE_RTTI
		SORA_EVENT_IDENTIFIER(18446744070739255393ULL);
#endif
		
	private:
		float time;
		float totalTime;
	};


} // namespace sora

#endif