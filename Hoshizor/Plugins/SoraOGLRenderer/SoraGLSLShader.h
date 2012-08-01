/*
 *  SoraGLSLShader.h
 *  Sora
 *
 *  Created by Robert Bu on 6/29/11.
 *  Copyright 2011 GameMaster Studio(Project Hoshizora). All rights reserved.
 *
 */

#ifndef SORA_GLSL_SHADER_H_
#define SORA_GLSL_SHADER_H_

#include "SoraShader.h"

#include "SoraPlatform.h"

#ifdef WIN32

#include <GL/GL.h>
#include <GL/GLU.h>

#elif defined(OS_OSX)

#include <OpenGL/OpenGL.h>
#include <OpenGL/glext.h>

#elif defined(OS_LINUX)

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#else defined(OS_IOS)

#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

#endif

namespace sora {
	
	/**
	 *	Implemention for GLSL Shader support
	 *  Note that glsl not use entry function but main
	 */
	
	class SoraGLSLShaderContext: public SoraShaderContext {
	public:
		SoraGLSLShaderContext();
		~SoraGLSLShaderContext();
		
        SoraShader* createShader(const StringType& file, const SoraString& entry, int32 type);
        SoraShader* createShaderFromMem(const char* data, const SoraString& entry, int32 type);
		
		virtual bool attachShaderList();
		virtual bool detachShaderList();
		
		bool isAttached() const;
        bool isLinked() const;
        GLuint getProgram() const { return mProgram; }
        
        void link();
				
	private:
		GLuint mProgram;
		
		bool mAttached;
        bool mLinked;
	};
	
	class SoraGLSLShader: public SoraShader {
	protected:
		friend class SoraGLSLShaderContext;
		
		SoraGLSLShader();
		~SoraGLSLShader();
		
		bool loadShader(const StringType& file, const SoraString& entry, int32 type);
        bool loadShaderFromMem(const char* data, const SoraString& entry, int32 type);
		
	public:
		
		void setTexture(const SoraString& decalName, SoraHandle tex);
		bool setParameterfv(const char* name, const float* val, uint32 size);
		bool setParameteriv(const char* name, const int32* val, uint32 size);
        bool getParameterfv(const char* name, float* val, uint32 size);
        bool getParameteriv(const char* name, int32* val, uint32 size);             
        
        
	private:
		bool attach();
		bool detach();
		
		GLuint mShader;
		GLuint mProgram;
		
		std::string mTexture1Name;
		SoraHandle mTexture1;
		
		void printProgramError();
		void printShaderError();
		
		SoraGLSLShaderContext* mContext;
	};
	
} // namespace sora


#endif // SORA_GLSL_SHADER_H_