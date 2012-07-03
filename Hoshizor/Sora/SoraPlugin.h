/*
 *  SoraPlugin.h
 *  Hoshizora
 *
 *  Created by Robert Bu on 8/22/10.
 *  Copyright 2010 Robert Bu. All rights reserved.
 *
 */

#ifndef SORA_PLUGIN_H
#define SORA_PLUGIN_H

#include "SoraPlatform.h"
#include "SoraUncopyable.h"

namespace sora {

	class SORA_API SoraPlugin: public SoraUncopyable {
	public:	
        SoraPlugin() {}
        virtual ~SoraPlugin() {}
        
		// calls when install a plugin
		virtual void install() {}
	
		// calls when initialise a plugin
		virtual void initialise() {}
	
		// calls when shutdown a plugin
		virtual void shutdown() {}

		// calls when unistsall a plugin
		virtual void unistall() {}
		
		// calls every frame
		virtual void update(float dt) {}
        
        // calls when reinitialise a plugin under some special situations
        virtual void reinitialise() {}
	
		virtual const SoraString getName() const = 0;
	};

} // namespace sora

#endif // sora_plugin_h