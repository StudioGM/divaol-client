//
//  SoraShaderEnabled.cpp
//  Sora
//
//  Created by Robert Bu on 8/7/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#include "SoraShaderable.h"
#include "SoraCore.h"
#include "SoraLogger.h"
#include "SoraShader.h"
#include "SoraFileSystem.h"

namespace sora {
    
    SoraShaderable::SoraShaderable():
    mInternalContext(false),
    mShaderContext(0) {
        
    }
    
    SoraShaderable::~SoraShaderable() {
        if(mInternalContext && mShaderContext) {
            delete mShaderContext;
        }
    }

    void SoraShaderable::attachShaderContext(const StringType& tag) {
        SoraShaderContext* context = SoraCore::Ptr->getFileSystem()->getShaderResource(tag);
        if(context) {
            if(mInternalContext && mShaderContext)
                delete mShaderContext;
            mShaderContext = context;
            mInternalContext = false;
        }
    }
    
    SoraShaderContext* SoraShaderable::getShaderContext() const {
        return mShaderContext;
    }
    
    SoraShader* SoraShaderable::attachShader(const StringType& file, const SoraString& entry, int32 type) {
        checkShaderContext();
        
        if(mShaderContext) {
            return mShaderContext->attachShader(file, entry, type);
        }
        return NULL;
    }
    
    SoraShader* SoraShaderable::attachFragmentShader(const StringType& file, const SoraString& entry) {
        return attachShader(file, entry, SoraShader::FragmentShader);
    }
    
    SoraShader* SoraShaderable::attachVertexShader(const StringType& file, const SoraString& entry) {
        return attachShader(file, entry, SoraShader::VertexShader);
    }

    void SoraShaderable::detachFragmentShader() {
        if(mShaderContext) {
            mShaderContext->detachFragmentShader();
        }
    }
    
    void SoraShaderable::detachVertexShader() {
        if(mShaderContext) {
            mShaderContext->detachVertexShader();
        }
    }
    
    void SoraShaderable::attachShaderToRender() {
        if(mShaderContext) {
            mShaderContext->attachToRender();
        }
    }
    
    void SoraShaderable::detachShaderFromRender() {
        if(mShaderContext) {
            mShaderContext->detachFromRender();
        }
    }
    
    void SoraShaderable::checkShaderContext() {
        if(!mShaderContext) {
            mShaderContext = SoraCore::Instance()->createShaderContext();
            if(!mShaderContext)
                log_error("SoraShaderable: error creating shader context");
            mInternalContext = true;
        }
    }
    
    bool SoraShaderable::hasShader() const {
        return mShaderContext && mShaderContext->isAvailable();
    }
    
    SoraShader* SoraShaderable::getFragmentShader() const {
        if(mShaderContext)
            return mShaderContext->getFragmentShader();
        return 0;
    }
    
    SoraShader* SoraShaderable::getVertexShader() const {
        if(mShaderContext)
            return mShaderContext->getVertexShader();
        return 0;
    }
    
} // namespace sora
