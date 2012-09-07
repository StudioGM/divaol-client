#include "SoraCGD3D9Shader.h"
#include "SoraCore.h"
#include "SoraLogger.h"
#include "SoraTexture.h"

#ifdef SORA_USE_SHADER

namespace sora {

	SoraCGD3D9ShaderContext::SoraCGD3D9ShaderContext() {
		context = cgCreateContext();
		cgSetParameterSettingMode(context, CG_DEFERRED_PARAMETER_SETTING);

		cgD3D9SetDevice((IDirect3DDevice9*)(SoraCore::Instance()->getVideoDeviceHandle()));
        
        vertexProfile = cgD3D9GetLatestVertexProfile();
        fragmentProfile = cgD3D9GetLatestPixelProfile();
		cgD3D9GetOptimalOptions(vertexProfile);
		cgD3D9GetOptimalOptions(fragmentProfile);
	}

	SoraCGD3D9ShaderContext::~SoraCGD3D9ShaderContext() {	
		if(context) {
			cgDestroyContext(context);
		}
	}

	SoraShader* SoraCGD3D9ShaderContext::createShader(const StringType& file, const SoraString& entry, int32 type) {
		SoraCGD3D9Shader* shader;
        switch(type) {
            case SoraShader::FragmentShader: {
                shader = new SoraCGD3D9Shader();
                shader->load(file, entry, type, context, fragmentProfile);
                break;
            }
            case SoraShader::VertexShader: {
                shader = new SoraCGD3D9Shader();
                shader->load(file, entry, type, context, vertexProfile);
                break;
            }
        }
        
		if(!shader || shader->getType() == 0) {
			if(shader) delete shader;
			return 0;
		}
		shader->mShaderContext = this;
		return shader;
	}
	
	SoraShader* SoraCGD3D9ShaderContext::createShaderFromMem(const char* data, const SoraString& entry, int32 type) {
		SoraCGD3D9Shader* shader;
        switch(type) {
            case SoraShader::FragmentShader: {
                shader = new SoraCGD3D9Shader();
                shader->loadFromMem(data, entry, type, context, fragmentProfile);
                break;
            }
            case SoraShader::VertexShader: {
                shader = new SoraCGD3D9Shader();
                shader->loadFromMem(data, entry, type, context, vertexProfile);
                break;
            }
        }
        
		if(!shader || shader->getType() == 0) {
			if(shader) delete shader;
			return 0;
		}
		shader->mShaderContext = this;
		return shader;
	}
	
	SoraCGD3D9Shader::~SoraCGD3D9Shader() {
		cgDestroyProgram(program);
		if(textureParam)
			cgDestroyParameter(textureParam);
	}

	void SoraCGD3D9Shader::checkError(CGcontext context) {
		CGerror error;
		const char* str = cgGetLastErrorString(&error);
		if(error != CG_NO_ERROR) {
			log_error(std::string("SoraShader: ")+str);
			if(error == CG_COMPILER_ERROR) {
				std::string error = cgGetLastListing(context);
				log_error(std::string("SoraShaderContext: ")+cgGetLastListing(context));
			}			
			setError(1);
		}
	}
	
	SoraCGD3D9Shader::SoraCGD3D9Shader() {
	}
	
	bool SoraCGD3D9Shader::load(const StringType& file, const SoraString& entry, int32 type, CGcontext context, CGprofile profile) {
		this->mType = type;
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
									  0);
			sora::SoraCore::Ptr->freeResourceFile((void*)data);
			
			checkError(context);
			cgD3D9LoadProgram(program, true, 0);
			checkError(context);
		} else {
			setError(1);
			return false;
		}	

		textureParam = 0;
		return true;
	}
	
	bool SoraCGD3D9Shader::loadFromMem(const char*data, const SoraString& entry, int32 type, CGcontext context, CGprofile profile) {
		this->mType = type;
		this->context = context;
        this->profile = profile;

		program = cgCreateProgram(context,
							      CG_SOURCE,
								  data,
								  profile,
								  entry.c_str(),
								  0);
			
		checkError(context);
		cgD3D9LoadProgram(program, true, 0);
		checkError(context);	

		textureParam = 0;
		return true;
	}
	
	void SoraCGD3D9Shader::setTexture(const SoraString& decalName, SoraTextureHandle tex) {
		textureParam = cgGetNamedParameter(program, decalName.c_str());
		cgD3D9SetTexture(textureParam, (LPDIRECT3DTEXTURE9)(((SoraTexture*)tex)->mTextureID));
	}

	bool SoraCGD3D9Shader::attach() {
		
		cgD3D9BindProgram(program);
		checkError(context);
      //  if(textureParam) {
       //     cgD3D9SetTextureParameter(textureParam, (IDirect3DBaseTexture9*)(((SoraTexture*)textureHandle)->mTextureID));
       // }
		
		//cgDXEnableProfile(profile);
		
		return getType() != 0;
	}
	
	bool SoraCGD3D9Shader::detach() {
		//cgD3D9DisableProfile(profile);
		cgD3D9UnbindProgram(program);
		checkError(context);
		
		return getType() != 0;
	}

	bool SoraCGD3D9Shader::setParameterfv(const char* name, const float* fv, uint32 size) {
		CGparameter param = cgGetNamedParameter(program, name);
		checkError(context);
		if(getType() == 0) return false;
		
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
	
	bool SoraCGD3D9Shader::setParameteriv(const char* name, const int32* fv, uint32 size) {
		CGparameter param = cgGetNamedParameter(program, name);
		checkError(context);
		if(getType() == 0) return false;
		
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
    
    bool SoraCGD3D9Shader::getParameterfv(const char* name, float* val, uint32 size) {
        if(getType() == 0) return false;
		CGparameter param = cgGetNamedParameter(program, name);
		checkError(context);
		if(getType() == 0) return false;
        
        cgGetParameterValuefc(param, size, val);
		return true;
    }
    
    bool SoraCGD3D9Shader::getParameteriv(const char* name, int32* val, uint32 size) {
        if(getType() == 0) return false;
		CGparameter param = cgGetNamedParameter(program, name);
		checkError(context);
		if(getType() == 0) return false;
        
        cgGetParameterValueic(param, size, val);
		return true;
    }

} // namespace sora

#endif // USE_SHADER