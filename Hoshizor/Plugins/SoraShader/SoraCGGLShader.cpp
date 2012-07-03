#include "SoraCGGLShader.h"
#include "SoraCore.h"
#include "SoraLogger.h"
#include "SoraTexture.h"

#ifdef SORA_USE_SHADER

namespace sora {

	SoraCGGLShaderContext::SoraCGGLShaderContext() {
		context = cgCreateContext();
		cgSetParameterSettingMode(context, CG_DEFERRED_PARAMETER_SETTING);
        
        vertexProfile = cgGLGetLatestProfile(CG_GL_VERTEX);
        fragmentProfile = cgGLGetLatestProfile(CG_GL_FRAGMENT);
        cgGLGetOptimalOptions(fragmentProfile);
        cgGLGetOptimalOptions(vertexProfile);
	}

	SoraCGGLShaderContext::~SoraCGGLShaderContext() {	
		if(context) {
			cgDestroyContext(context);
		}
    }

    SoraShader* SoraCGGLShaderContext::createShader(const StringType& file, const SoraString& entry, int32 type) {
        SoraCGGLShader* shader = NULL;
        switch (type) {
			case SoraShader::VertexShader: {
                shader = new SoraCGGLShader();
                shader->load(file, entry, type, context, vertexProfile);
                break;
            }
			case SoraShader::FragmentShader: {
                shader = new SoraCGGLShader();
                shader->load(file, entry, type, context, fragmentProfile);
                break;
            }
		}
		
		if(!shader || shader->getError() != SoraShader::ShaderNoError) {
			if(shader) delete shader;
			return NULL;
		}
        shader->mShaderContext = this;
        return shader;
    }
    
    SoraShader* SoraCGGLShaderContext::createShaderFromMem(const char* data, const SoraString& entry, int32 type) {
        SoraCGGLShader* shader = NULL;
        switch (type) {
			case SoraShader::VertexShader: {
                shader = new SoraCGGLShader();
                shader->loadFromMem(data, entry, type, context, vertexProfile);
                break;
            }
			case SoraShader::FragmentShader: {
                shader = new SoraCGGLShader();
                shader->loadFromMem(data, entry, type, context, fragmentProfile);
                break;
            }
		}
		
		if(!shader || shader->getError() != SoraShader::ShaderNoError) {
			if(shader) delete shader;
			return NULL;
		}
        shader->mShaderContext = this;
        return shader;
    }

	SoraCGGLShader::~SoraCGGLShader() {
        if(textureParam)
            cgDestroyParameter(textureParam);
		cgDestroyProgram(program);
	}
	
	void SoraCGGLShader::checkError(CGcontext context) {
		CGerror error;
		const char* str = cgGetLastErrorString(&error);
		if(error != CG_NO_ERROR) {
			printf("%s\n", vamssg("SoraShader: %s\n", str).c_str());
			if(error == CG_COMPILER_ERROR) {
				printf("%s\n", (vamssg("SoraShaderContext: %s", cgGetLastListing(context))).c_str());
			}			
			setError(1);
		}
	}

	SoraCGGLShader::SoraCGGLShader() {
		
	}
    
    bool SoraCGGLShader::load(const StringType& file, const SoraString& entry, int32 type, CGcontext context, CGprofile profile) {
        mType = type;
		this->context = context;
        this->profile = profile;
		
		uint32 size;
		const char* data = (const char*)SoraCore::Instance()->getResourceFile(file, size);
		if(data) {
            program = cgCreateProgram(context,
									  CG_SOURCE,
									  data,
									  profile,
									  entry.c_str(),
									  NULL);
			SoraCore::Ptr->freeResourceFile((void*)data);
            
			checkError(context);
			cgGLLoadProgram(program);
		} else {
			setError(1);
            return false;
		}
        
		textureParam = 0;
        return true;
    }
    
    bool SoraCGGLShader::loadFromMem(const char* data, const SoraString& entry, int32 type, CGcontext context, CGprofile profile) {
		mType = type;
        sora_assert(data);
                    
		this->context = context;
        this->profile = profile;
		
		if(data) {
            program = cgCreateProgram(context,
									  CG_SOURCE,
									  data,
									  profile,
									  entry.c_str(),
									  NULL);
            
			checkError(context);
			cgGLLoadProgram(program);
		} else {
			setError(1);
            return false;
		}
        
		textureParam = 0;
        return true;
	}

	void SoraCGGLShader::setTexture(const SoraString& decalName, SoraHandle tex) {
		textureParam = cgGetNamedParameter(program, decalName.c_str());
        cgGLSetTextureParameter(textureParam, (GLuint)((SoraTexture*)tex)->mTextureID);
	}
	
	bool SoraCGGLShader::attach() {
		cgGLBindProgram(program);
		checkError(context);
		cgGLEnableProfile(profile);
		checkError(context);
		if(textureParam) {
			cgGLEnableTextureParameter(textureParam);
			checkError(context);
		}
		return (getType() != 0);
	}
	
	bool SoraCGGLShader::detach() {
		cgGLDisableProfile(profile);
		checkError(context);
		
		return (getType() != 0);
	}

	bool SoraCGGLShader::setParameterfv(const char* name, const float* fv, uint32 size) {
		if(getType() == 0) return false;
		CGparameter param = cgGetNamedParameter(program, name);
        if(!param) return false;
		
		switch (size) {
            case 0: break;
			case 1: cgSetParameter1fv(param, fv); break;
			case 2: cgSetParameter2fv(param, fv); break;
			case 3: cgSetParameter3fv(param, fv); break;
			case 4: cgSetParameter4fv(param, fv); break;
            default:
                cgSetParameterValuefc(param, size, fv); break;
		}
		checkError(context);
		return (getType() != 0);
	}
	
	bool SoraCGGLShader::setParameteriv(const char* name, const int32* fv, uint32 size) {
		if(getType() == 0) return false;
		CGparameter param = cgGetNamedParameter(program, name);
		if(!param) return false;
		
		switch (size) {
            case 0: break;
			case 1: cgSetParameter1iv(param, fv); break;
			case 2: cgSetParameter2iv(param, fv); break;
			case 3: cgSetParameter3iv(param, fv); break;
			case 4: cgSetParameter4iv(param, fv); break;
            default:
                cgSetParameterValueic(param, size, fv); break;
		}
		checkError(context);
		return (getType() != 0);
	}
    
    bool SoraCGGLShader::getParameterfv(const char* name, float* val, uint32 size) {
        if(getType() == 0) return false;
		CGparameter param = cgGetNamedParameter(program, name);
		if(!param) return false;
        
        cgGetParameterValuefc(param, size, val);
		return true;
    }
    
    bool SoraCGGLShader::getParameteriv(const char* name, int32* val, uint32 size) {
        if(getType() == 0) return false;
		CGparameter param = cgGetNamedParameter(program, name);
		if(!param) return false;
        
        cgGetParameterValueic(param, size, val);
		return true;
    }

} // namespace sora

#endif // SORA_USE_SHADER