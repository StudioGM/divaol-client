//
//  SoraThreadImplWin32.h
//  Sora
//
//  Created by Robert Bu on 7/7/11.
//  Copyright 2011 Robert Bu(Project Hoshizora). All rights reserved.
//

#ifndef Sora_SoraThreadImplWin32_h
#define Sora_SoraThreadImplWin32_h

#include "SoraPlatform.h"

#ifdef OS_WIN32

#include <windows.h>

#include "SoraThreadTask.h"

namespace sora {
    
    class SoraThreadImpl: public SoraUncopyable {
    protected:
        SoraThreadImpl():
			 thread_handle(NULL),
			 thread_id(0),
			 active(false) {
		}
        ~SoraThreadImpl()  {
			if(thread_handle == NULL)
				return;
			exitImpl();
			thread_id = 0;
			CloseHandle(thread_handle);
		}
        
        int32 startImpl()  {
			if(active || !thread_task.isValid())
			    return 0;
        
			thread_handle = CreateThread(NULL, 0, _ThreadProc, (LPVOID)this, 0, (LPDWORD)&thread_id);
			if(thread_handle == INVALID_HANDLE_VALUE || thread_handle == NULL) {
			    thread_id = 0;
			    return 0;
			}
			return 1;
		}
        int32 startWithTaskImpl(const SoraThreadTask& task) {
			setThreadTaskImpl(task);
			return startImpl();
		}
        
        void joinImpl() {
			WaitForSingleObject(thread_handle, INFINITE);
		}
    
		void exitImpl() {
			TerminateThread(thread_handle, 0);
		}
    
		bool isActiveImpl() const {
			return active;
		}
    
		void setActiveImpl(bool flag) {
			active = flag;
		}

		void setThreadTaskImpl(const SoraThreadTask& task) {
			thread_task = task;
		}
    
		SoraThreadTask getThreadTaskImpl() const {
			return thread_task;
		}
       
    private:
        uint32 thread_id;
        HANDLE thread_handle;
        
        SoraThreadTask thread_task;
        static DWORD WINAPI _ThreadProc(LPVOID param);
       
        bool active;
    };
    
	 
} // namespace sora

#endif

#endif
