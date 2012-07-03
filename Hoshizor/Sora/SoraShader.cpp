/*
 *  SoraShader.cpp
 *  SoraPureCore
 *
 *  Created by Robert Bu on 11/21/10.
 *  Copyright 2010 Robert Bu. All rights reserved.
 *
 */

#include "SoraShader.h"
#include "SoraCore.h"
#include "SoraLogger.h"
#include "SoraShaderParamObject.h"

namespace sora {

	SoraShader::SoraShader() {
        mErrorCode = ShaderNoError;
        mShaderContext = NULL;
	}
	
	SoraShader::~SoraShader() {
		
	}

	bool SoraShader::setParameter1f(const char* name, float v1) {
		float arr[1];
		arr[0] = v1;
		return setParameterfv(name, arr, 1);
	}
	
	bool SoraShader::setParameter2f(const char* name, float v1, float v2) {
		float arr[2];
		arr[0] = v1; arr[1] = v2;
		return setParameterfv(name, arr, 2);
	}
	
	bool SoraShader::setParameter3f(const char* name, float v1, float v2, float v3) {
		float arr[3];
		arr[0] = v1; arr[1] = v2; arr[2] = v3;
		return setParameterfv(name, arr, 3);
	}
	
	bool SoraShader::setParameter4f(const char* name, float v1, float v2, float v3, float v4) {
		float arr[4];
		arr[0] = v1; arr[1] = v2; arr[2] = v3; arr[3] = v4;
		return setParameterfv(name, arr, 4);
	}
	
	bool SoraShader::setParameter1i(const char* name, int32 v1) {
		int32 arr[1];
		arr[0] = v1;
		return setParameteriv(name, arr, 1);
	}
	
	bool SoraShader::setParameter2i(const char* name, int32 v1, int32 v2) {
		int32 arr[2];
		arr[0] = v1; arr[1] = v2;
		return setParameteriv(name, arr, 2);
	}
	
	bool SoraShader::setParameter3i(const char* name, int32 v1, int32 v2, int32 v3) {
		int32 arr[3];
		arr[0] = v1; arr[1] = v2; arr[2] = v3;
		return setParameteriv(name, arr, 3);
	}
	
	bool SoraShader::setParameter4i(const char* name, int32 v1, int32 v2, int32 v3, int32 v4) {
		int32 arr[4];
		arr[0] = v1; arr[1] = v2; arr[2] = v3; arr[3] = v4;
		return setParameteriv(name, arr, 4);
	}
    
    SoraShaderParameter SoraShader::getParameter(const char* name) {
        return SoraShaderParameter(this, name);
    }
	
    int32 SoraShader::getType() const {
        return mType;
    }
    
    SoraShaderContext* SoraShader::getShaderContext() const {
        return mShaderContext;
    }
    
    void SoraShader::setError(int32 error) {
        mErrorCode = error;
    }
    
    int32 SoraShader::getError() const {
        return mErrorCode;
    }
	
	SoraShaderContext::SoraShaderContext() {
	}
	
	SoraShaderContext::~SoraShaderContext() {
		clear();
	}
	
    SoraShader* SoraShaderContext::attachShader(const StringType& file, const SoraString& entry, int32 type) {
        if(type == SoraShader::FragmentShader)
            return attachFragmentShader(file, entry);
        else if(type == SoraShader::VertexShader)
            return attachVertexShader(file, entry);
        else 
            THROW_SORA_EXCEPTION(RuntimeException, "Unknown shader type");
        return NULL;
    }
	
	void SoraShaderContext::attachToRender() {
		SoraCore::Instance()->attachShaderContext(this);
	}

	bool SoraShaderContext::attachShaderList() {
        bool result = true;
        if(mVertexShader.get())
            result = mVertexShader->attach();
        if(mFragmentShader.get())
            result = mFragmentShader->attach();
        return result;
	}
    
    bool SoraShaderContext::detachShaderList() {
        bool result = true;
        if(mVertexShader.get())
            result = mVertexShader->detach();
        if(mFragmentShader.get())
            result = mFragmentShader->detach();
        return result;
    }
	
	void SoraShaderContext::detachFromRender() {
		SoraCore::Instance()->detachShaderContext();
	}
    
	void SoraShaderContext::detachFragmentShader() {
        mFragmentShader.reset();
    }
    
    void SoraShaderContext::detachVertexShader() {
        mVertexShader.reset();
    }
    
	void SoraShaderContext::clear() {
		mFragmentShader.reset();
        mVertexShader.reset();
	}
    
    SoraShader* SoraShaderContext::attachFragmentShader(const StringType& file, const SoraString& entry) {
        SoraShader* shader = createShader(file, entry, SoraShader::FragmentShader);
        if(!shader) {
            log_error("Error creating shader");
            return 0;
        }
        if(shader->getError() != SoraShader::ShaderNoError) {
            shader->release();
            log_error(vamssg("Shader returned with error code %d", shader->getError()));
            return NULL;
        }
        mFragmentShader.assign(shader);
        return shader;
    }
    
    SoraShader* SoraShaderContext::attachVertexShader(const StringType& file, const SoraString& entry) {
        SoraShader* shader = createShader(file, entry, SoraShader::VertexShader);
        if(!shader)
            log_error("Error creating shader");
        if(shader->getError() != SoraShader::ShaderNoError) {
            shader->release();
            log_error(vamssg("Shader returned with error code %d", shader->getError()));
            return NULL;
        }
        mVertexShader.assign(shader);
        return shader;
    }
    
    SoraShader* SoraShaderContext::getFragmentShader() const {
        return mFragmentShader.get();
    }
    
    SoraShader* SoraShaderContext::getVertexShader() const {
        return mVertexShader.get();
    }
    
    bool SoraShaderContext::isAvailable() {
        return (mFragmentShader.get() != NULL || mVertexShader.get() != NULL);
    }
    
    SoraShaderContext* SoraShaderContext::Create() {
        return SoraCore::Instance()->createShaderContext();
    }

    
} // namespace sora