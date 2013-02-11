/*
 *  SoraLuaMainWindow.h
 *  Sora
 *
 *  Created by Griffin Bu on 6/16/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef SORA_LUA_MAINWINDOW_H_
#define SORA_LUA_MAINWINDOW_H_

#include "SoraPlatform.h"
#include "SoraWindowInfo.h"
#include "SoraLuaObject.h"

namespace sora {
	
	/**		
	 *		A Lua MainWindow Implemention
	 *		Would delegate MainWindow callbacks to a lua script
	 *		To run sora with a start lua script
	 **/
	
	class SoraLuaMainWindow: public SoraWindowInfoBase {
	public:
		SoraLuaMainWindow();
		SoraLuaMainWindow(const SoraWString& scriptPath);
		virtual ~SoraLuaMainWindow();
		
		int32 getWindowWidth();
		int32 getWindowHeight();
		
		int32 getWindowPosX();
		int32 getWindowPosY();
		
		StringType getWindowName();
		SoraString getWindowId();
		
		void init();
		
		bool isWindowed();
		bool hideMouse();
		
		bool renderFunc();
		bool updateFunc();
		
	private:
		SoraLuaObject mScriptObj;
	};
	
} // namespace sora


#endif // SORA_LUA_MAINWINDOW_H_