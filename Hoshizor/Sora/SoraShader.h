/*
 *  SoraShader.h
 *  SoraPureCore
 *
 *  Created by Robert Bu on 11/21/10.
 *  Copyright 2010 GameMaster&. All rights reserved.
 *
 */


#ifndef SORA_SHADER_H__
#define SORA_SHADER_H__

#include "SoraPlatform.h"
#include "SoraRefCounted.h"
#include "SoraString.h"
#include "SoraAutoPtr.h"

#include <list>

namespace sora {
	
    
    class SoraShaderContext;
	class SoraShaderParameter;
    
	/* 
		Class to hold a shader
		Automatically created and managed by SoraShaderContext
	*/
	class SORA_API SoraShader: public SoraRefCounted {
		friend class SoraShaderContext;
        
	public:
        enum { ShaderNoError = 0 };
        
        enum {
                /* vertex shader */
            VertexShader = 1,
                /* fragment shader */
            FragmentShader = 2,
        };
    
        SoraShader();
        virtual ~SoraShader();
        
		/*
			Set a parameterf
			@param name, the name of the parameter to set
			@param val, the value to set
			@param size, the size of the value array, 1 = float, 2 float2, 16 = float4x4 etc
			@retval, succeed or not
		 */
		virtual bool setParameterfv(const char* name, const float* val, uint32 size) = 0;
		/*
			Set a parameteri
			@param name, the name of the parameter to set
			@param val, the value to set
			@param size, the size of the value array, 1 = float, 2 float2, 16 = float4x4 etc
			@retval, succeed or not
		 */
		virtual bool setParameteriv(const char* name, const int32* val, uint32 size) = 0;
        
        virtual bool getParameterfv(const char* name, float* val, uint32 size) = 0;
        virtual bool getParameteriv(const char* name, int32* val, uint32 size) = 0;
		
		bool setParameter1f(const char* name, float v1);
		bool setParameter2f(const char* name, float v1, float v2);
		bool setParameter3f(const char* name, float v1, float v2, float v3);
		bool setParameter4f(const char* name, float v1, float v2, float v3, float v4);

		bool setParameter1i(const char* name, int32 v1);
		bool setParameter2i(const char* name, int32 v1, int32 v2);
		bool setParameter3i(const char* name, int32 v1, int32 v2, int32 v3);
		bool setParameter4i(const char* name, int32 v1, int32 v2, int32 v3, int32 v4);
        
        SoraShaderParameter getParameter(const char* name);
        		
        /*
         add a sample texture
         may use as TEX1 in shader
         */
		virtual void setTexture(const SoraString& decalName, SoraHandle tex) = 0;

		/*
			Get the type of the shader
			See SORA_SHADER_TYPE
		 */
		int32 getType() const;
        SoraShaderContext* getShaderContext() const;
        
        void setError(int32 error);
        int32 getError() const;

	protected:
		/* 
		 Attach the shader to render
		 Applies to the rendering after attach
		 Would clear when endFrame (if not detach before endFrame)
		 */
		virtual bool attach() = 0;
		/*
		 Detach the shader from render
		 */
		virtual bool detach() = 0;
        
        uint32 mType;
        int32 mErrorCode;
        SoraShaderContext* mShaderContext;
	};
    
    template<class T>
    struct ShaderReleasePolicy {
        static void release(T* obj) {
            
        }
    };
    
    template<>
    struct ShaderReleasePolicy<SoraShader> {
        static void Release(SoraShader* shader) {
            if(shader)
                shader->release();
        }
    };
	
	class SORA_API SoraShaderContext {
    public:
        SoraShaderContext();
        virtual ~SoraShaderContext();

        typedef std::list<SoraShader*> ShaderList;

		void clear();
        
        static SoraShaderContext* Create();

		/**
         * attach a shader to context
         * @param file, the path of the shader file to attach
         * @param entry, entry function of the shader
         * @param type, the type of the shader
         * @retval, the handle to the attached shader, is 0 if attach failed
		 */
        SoraShader* attachShader(const StringType& file, const SoraString& entry, int32 type);
        
        /**
         * Create and attach a fragment shader
         **/
        SoraShader* attachFragmentShader(const StringType& file, const SoraString& entry);
        /**
         * Create and attach a vertex shader
         **/
        SoraShader* attachVertexShader(const StringType& file, const SoraString& entry);
    
        SoraShader* getFragmentShader() const;
        SoraShader* getVertexShader() const;
                
		void detachFragmentShader();
        void detachVertexShader();
        
        bool isAvailable();

		/**
         * attach all shaders in context to render
		 */
		void attachToRender();
		/**
         * detach all shaders in context from render
		 */
		void detachFromRender();
        
		/*
		called by render system
		DO NOT CALL THESE unless you know what you are doing
		*/
		virtual bool attachShaderList();
		virtual bool detachShaderList();
		
	protected:		        
        /**
         * create a shader from context, not attach
         * @param file, the path of the shader file to attach
         * @param entry, entry function of the shader
         * @param type, the type of the shader
         * @retval, the handle to the attached shader, is 0 if attach failed
		 */
        virtual SoraShader* createShader(const StringType& file, const SoraString& entry, int32 type) = 0;
        virtual SoraShader* createShaderFromMem(const char* data, const SoraString& entry, int32 type) = 0;
        
        typedef SoraAutoPtr<SoraShader, autoptr::RefCounter, ShaderReleasePolicy<SoraShader> > ShaderPtr;
        
        ShaderPtr mVertexShader;
        ShaderPtr mFragmentShader;
    };

} // namespace sora


#endif