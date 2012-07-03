#ifndef SORA_INFINITE_H_
#define SORA_INFINITE_H_

#define USE_SORA_CORES

#include "SoraTimer.h"
#include "SoraCore.h"

namespace sora{
    
    /**
     * For intergration with some other engine that requires C function callback
     **/
    
	static SoraTimer* g_timer;

	static bool bool_updateFrame() {
		assert(g_timer != 0);
		if(g_timer->update()) {
			SoraCore::Instance()->update();
			return true;
		}
		return false;
	}
	
	static void void_updateFrame() {
		assert(g_timer != 0);
		if(g_timer->update()) {
			SoraCore::Instance()->update();
		}
	}
	
	static int int_exitFunc() {
		SoraCore::Instance()->shutDown();
		return 1;
	}
}

#endif