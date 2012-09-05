//
//  SoraLuaExporter.cpp
//  Sora
//
//  Created by Robert Bu on 8/25/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraLuaExporter.h"

namespace sora {
    
    void SoraLuaExporter::OnExport(LuaPlus::LuaState* state) {
        getExporter().sig(state);
    }
    
} // namespace sora