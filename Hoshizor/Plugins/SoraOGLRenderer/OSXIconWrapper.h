//
//  OSXIconWrapper.h
//  Sora
//
//  Created by Robert Bu on 7/4/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_OSXIconWrapper_h
#define Sora_OSXIconWrapper_h

#include "SoraPlatform.h"

#ifdef OS_OSX

namespace sora {
    
    void osx_setDockIcon(const SoraString& iconPath);
    void osx_setAppCursor(const SoraString& cursorPath);
    
    void osx_activeCurrentCursor();
}

#endif // OS_OSX

#endif
