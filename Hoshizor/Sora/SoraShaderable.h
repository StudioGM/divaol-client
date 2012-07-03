//
//  SoraShaderEnabled.h
//  Sora
//
//  Created by Robert Bu on 8/7/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraShaderEnabled_h
#define Sora_SoraShaderEnabled_h

#include "SoraPlatform.h"
#include "SoraObject.h"
#include "SoraUncopyable.h"
#include "SoraString.h"

namespace sora {
    
    class SoraShader;
    class SoraShaderContext;
    
    /**
     * Base class from objects that can render with shader
     **/
    
    class SORA_API SoraShaderable {
    public:
        SoraShaderable();
        virtual ~SoraShaderable();
        
        void attachShaderContext(const StringType& tag);
        SoraShaderContext* getShaderContext() const;
        
        SoraShader* attachShader(const StringType& file, const SoraString& entry, int32 type);
        SoraShader* attachFragmentShader(const StringType& file, const SoraString& entry);
        SoraShader* attachVertexShader(const StringType& file, const SoraString& entry);
        
        void detachFragmentShader();
        void detachVertexShader();
        
        void attachShaderToRender();
        void detachShaderFromRender();
        
        SoraShader* getFragmentShader() const;
        SoraShader* getVertexShader() const;
        
        bool hasShader() const;
    
    private:
        inline void checkShaderContext();
        
        bool mInternalContext;
        SoraShaderContext* mShaderContext;
    };
    
}


#endif
