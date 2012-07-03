//
//  SoraFunction.h
//  zeromq_client
//
//  Created by Ruiwei Bu on 8/15/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef SoraFunction_h
#define SoraFunction_h

#include "detail/SoraFunction0.h"
#include "detail/SoraFunction1.h"
#include "detail/SoraFunction2.h"
#include "detail/SoraFunction3.h"
#include "detail/SoraFunction4.h"
#include "detail/SoraFunction5.h"
#include "detail/SoraFunction6.h"
#include "detail/SoraFunction7.h"
#include "detail/SoraFunction8.h"
#include "detail/SoraFunction9.h"
#include "detail/SoraFunction10.h"
#include "detail/SoraFunction11.h"
#include "detail/SoraFunction12.h"

namespace sora {
    
    template<typename SIG, typename T>
    static SoraFunction<SIG> MakeFunction(const T& t) {
        return SoraFunction<SIG>(t);
    }
    
} // namespace sora

#endif
