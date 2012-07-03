/*
 *  SoraCommandLine.h
 *  Sora
 *
 *  Created by Griffin Bu on 6/4/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef SORA_COMMAND_LINE_H_
#define SORA_COMMAND_LINE_H_

#include "SoraPlatform.h"
#include "SoraEvent.h"
#include "SoraKeyInfo.h"
#include "SoraHotkey.h"
#include "SoraFrameListener.h"
#include "SoraIteratorHelper.h"
#include "SoraPreDeclare.h"

#include <vector>

namespace sora {
	
    class SoraFont;
    
	class SORA_API SoraConsoleEvent: public SoraEvent {
	public:
		SoraConsoleEvent() {}
		SoraConsoleEvent(const std::string& cmd, const char* params):
		mCmd(cmd),
		mParams(params) {}
		
#ifndef SORA_USE_RTTI
		SORA_EVENT_IDENTIFIER(18446744073250990306ULL);
#endif
		
		void setCmd(const std::string& cmd) { mCmd = cmd; }
		void setParams(const std::string& params) { mParams = params; }
		
		/**
		 * Push a line pf the return result of the executed cmd, would be displayed in the console
		 * @parem result, a line of the result of the cmd
		 */
		void pushResult(const std::string& result) { 
            mResults.push_back(result);
        }
		
		std::string getCmd() const { return mCmd; }
		std::string getParams() const { return mParams; }
		const std::vector<std::string>& getResults() const { return mResults; }
        
        SORA_ITERABLE(std::vector<std::string>, mResults);
	
	private:
		std::string mCmd;
		std::string mParams;
        std::vector<std::string> mResults;
	};
	
	class SORA_API SoraConsole: public SoraFrameListener, public SoraEventHandler {
	protected:
		SoraConsole();
		~SoraConsole();
	
	public:	
        static SoraConsole* Instance();
        static void Destroy();
        
        enum {
			TAB_CMDLINE = 1,
			TAB_MSSG = 2,
		};
		
		void render();
		
		void publishCmd(const std::string& cmd, const char* params);
		void outputMessage(const std::string& msg);
		
		void setTab(int32 tabid);
		int32 getTab() const;
		
		static void registerCmdHandler(SoraEventHandler* handler, const std::string& cmd);
		void onKeyEvent(SoraKeyEvent* kev);
        void onHotkeyEvent(SoraHotkeyEvent* hev);
		
		std::string getCurrentLine() const;
		
		uint32 getCaretRow() const;
		uint32 getCaretPosition() const;
        
		void reset();
        
		bool isActive() const;
		void setActiveKey(const SoraHotkey& key);
		
		void setPosition(float posx, float posy);
		void setSize(float width, float height);
        
        void setUseSysTerm(bool flag);
		
        void onFrameStart();
        void onFrameEnd();
        
        void setFont(SoraFont* font);
        SoraFont* getFont() const;
        
        SORA_CLASS_DEF_FIELD_SET_GET_P(uint32, CmdColor, m)
        SORA_CLASS_DEF_FIELD_SET_GET_P(uint32, BackgroundColor, m)
        SORA_CLASS_DEF_FIELD_SET_GET_P(uint32, CaretColor, m)
		SORA_CLASS_DEF_FIELD_SET_GET_P(uint32, ResultColor, m)
        
		SORA_CLASS_DEF_FIELD_SET_GET_P(float, Width, m)
        SORA_CLASS_DEF_FIELD_SET_GET_P(float, Height, m)
        
		SORA_CLASS_DEF_FIELD_SET_GET_P(float, PositionX, m)
        SORA_CLASS_DEF_FIELD_SET_GET_P(float, PositionY, m)
		
		typedef std::map<std::string, SoraEventHandler*> CommandHandlerMap;

	private:        
		void drawCmds();
		void drawMssg();
		void drawTab();
		
		bool mActive;
		int32 mActiveKeyId;
		
		bool mBackspaceDown;
		float mBackspaceTime;
		
		bool mUpDown, mDownDown;
		float mUpDownTime, mDownDownTime;
		
		bool mMssgReachTop;
        bool mUseSysTerm;
		
		int32 mTab;
		
		static CommandHandlerMap& getCommandHandlerMap();

		std::string mCurrentLine;
		typedef struct {
			std::wstring mCmd;
            std::vector<std::wstring> mResults;
		} CmdHistory;
		typedef std::vector<CmdHistory> CommandHistoryList;
		CommandHistoryList mHistory;
		
		uint32 mCaretRow;
		uint32 mCaretPosition;
		
        SoraFont* mFont;
		int32 mFontHeight;
		
		int32 mCurrLine;
		int32 mStartLine;
		int32 mCurrHeight;
		float mCaretShow;
	};
	
#define SORA_DEF_CONSOLE_EVT_FUNC(func, cmd) \
    SORA_DEF_FUNC_AS_EVENT_HANDLER(func, sora::SoraConsoleEvent) \
    SORA_STATIC_RUN_CODE_I(sora_static_##func, sora::SoraConsole::registerCmdHandler(func##EventHandler::Instance(), cmd))
    
} // namespace sora

#endif // SORA_COMMAND_LINE_H_
