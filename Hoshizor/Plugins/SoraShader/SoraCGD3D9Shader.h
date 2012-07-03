#ifndef SORA_CGD3D9_SHADER_H_
#define SORA_CGD3D9_SHADER_H_

#include "SoraShader.h"

#ifdef SORA_USE_SHADER


#include "Cg/cg.h"
#include "Cg/cgD3D9.h"

#ifdef WIN32
#pragma comment(lib, "cg.lib")
#pragma comment(lib, "cgd3d9.lib")
#endif


namespace sora {

	/*
		SoraShader for D3D9 renderer
		use when USE_OGL_RENDERER is not defined
		see SoraPlatform.h for USE_OGL_RENDERER definition
	 */
	class SoraCGD3D9Shader: public SoraShader {
		friend class SoraShaderContext;
		friend class SoraCGD3D9ShaderContext;

		SoraCGD3D9Shader();
		bool load(const StringType& file, const SoraString& entry, int32 type, CGcontext context, CGprofile profile);
		bool loadFromMem(const char* data, const SoraString& entry, int32 type, CGcontext context, CGprofile profile);
		
		~SoraCGD3D9Shader();
		
	public:
	
		void setTexture(const SoraString& decalName, SoraTextureHandle tex);
		bool setParameterfv(const char* name, const float* val, uint32 size);
		bool setParameteriv(const char* name, const int32* val, uint32 size);
        
		bool getParameterfv(const char* name, float* val, uint32 size);
		bool getParameteriv(const char* name, int32* val, uint32 size);
        
	private:
		bool attach();
		bool detach();
	
		enum {
		/* DirectX 9 Vertex Shader */
			DX_VERTEX_SHADER	= CG_PROFILE_VS_2_X,
			/* DirectX 9 Pixel Shader */
			DX_PIXEL_SHADER		= CG_PROFILE_PS_2_X,
		};

		void checkError(CGcontext context);

		CGprofile profile;
		CGprogram program;
		CGcontext context;
		CGparameter textureParam;
	};

	class SoraCGD3D9ShaderContext: public SoraShaderContext {
	public:
		SoraCGD3D9ShaderContext();
		~SoraCGD3D9ShaderContext();

		SoraShader* createShader(const StringType& file, const SoraString& entry, int32 type);
		SoraShader* createShaderFromMem(const char* data, const SoraString& entry, int32 type);
		CGcontext context;
        CGprofile fragmentProfile;
        CGprofile vertexProfile;
	};
	

} // namespace sora

#endif // USE_SHADER

#endif // SORA_DX_SHADER_H_