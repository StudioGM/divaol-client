//
//  modifierFactory.h
//  Guichan_Sora
//
//  Created by Robert Bu(darkfall) 8/26/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_modifierFactory_h
#define Sora_modifierFactory_h

#include "guichan/factory.hpp"
#include "guichan/modifier.hpp"

namespace gcn {
    
    /**
     * Modifier factory typedef
     **/
    
    typedef AbstractFactory<Modifier> ModifierFactory;
    
} // namespace gcn


#endif
