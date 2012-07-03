//
//  SoraPathImpliOS.h
//  Sora
//
//  Created by Robert Bu on 7/9/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraPathImpliOS_h
#define Sora_SoraPathImpliOS_h

#include "SoraPlatform.h"
#include "SoraException.h"

#ifdef OS_IOS

namespace sora {
    
    class SoraPathImpl {
    public:
        static std::string getEnvImpl(const std::string& name);
        static std::string currentImpl();
        static std::string homeImpl();
        static std::string tempImpl();
        static std::string nullImpl();
        static std::string expandImpl(const std::string patH);
        static void listRootsImpl(std::vector<std::string>& roots);
        static std::string resourceImpl();
        static std::string writtableImpl();
        static std::string fontImpl();
    };
    
} // namespace sora

#endif // OS_IOS

#endif
