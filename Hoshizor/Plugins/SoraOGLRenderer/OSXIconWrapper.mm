//
//  OSXIconWrapper.cpp
//  Sora
//
//  Created by Robert Bu on 7/4/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "OSXIconWrapper.h"

#ifdef OS_OSX 

#include <Cocoa/Cocoa.h>
#include <Foundation/Foundation.h>

namespace sora {
    
    static NSImage* dock_image = NULL;
    
    static NSCursor* current_cursor = NULL;
    static NSImage* cursor_image = NULL;
    
    void osx_setDockIcon(const SoraString& icon) {
        NSString* nsicon = [[NSString alloc] initWithUTF8String:icon.c_str()];
        
        if(dock_image != NULL)
            [dock_image release];
        
        dock_image = [[NSImage alloc] initWithContentsOfFile: nsicon];
        [NSApp setApplicationIconImage: dock_image];
        
        [nsicon release];
    }
    
    void osx_setAppCursor(const SoraString& cursor) {
        NSString* nscursor = [[NSString alloc] initWithUTF8String:cursor.c_str()];
        
        if(cursor_image != NULL)
            [cursor_image release];
        cursor_image = [[NSImage alloc] initWithContentsOfFile: nscursor];
        
        if(current_cursor != NULL)
            [current_cursor release];
        current_cursor = [[NSCursor alloc] initWithImage: cursor_image hotSpot:NSMakePoint(0.f, 0.f)];
                
        [current_cursor set];
        
        [nscursor release];
    }
    
    void osx_activeCurrentCursor() {
        if(current_cursor)
            [current_cursor set];
    }
    
    
} // namespace sora

#endif // OS_OSX