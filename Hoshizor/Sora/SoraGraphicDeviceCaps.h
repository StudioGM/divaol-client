//
//  SoraGraphicDeviceCaps.h
//  Sora
//
//  Created by Ruiwei Bu on 11/13/11.
//  Copyright (c) 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraGraphicDeviceCaps_h
#define Sora_SoraGraphicDeviceCaps_h

#include "SoraPlatform.h"

namespace sora {
    
    struct SoraGraphicDeviceCaps {
        uint32 max_indices;
        uint32 max_vertices;
        
        uint32 max_texture_cube_map_size;
        uint32 max_texture_width;
        uint32 max_texture_height;
        
        uint32 max_pixel_texture_units;
        uint32 max_vertex_texture_units;
    };
    
} // namespace sora

#endif
