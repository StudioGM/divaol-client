//
//  ITimerManager.cpp
//  Sora
//
//  Created by Robert Bu on 8/4/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "ITimerManager.h"

namespace sora  {
      
    template<> 
    ITimerManager* SoraDirectSingleton<ITimerManager>::_singleton = NULL;
    
}
