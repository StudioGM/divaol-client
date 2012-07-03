/*
 *  SoraThread.cpp
 *  Hoshizora
 *
 *  Created by Robert Bu on 8/27/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "SoraThread.h"

namespace sora {
    
	int32 SoraThread::active_thread_num = 0;

    SoraThread::SoraThread() {
    
    }
    
    SoraThread::SoraThread(const SoraThreadTask& task, const std::string _name):
    name(_name) {
        setThreadTaskImpl(task);
    }
    
    SoraThread::~SoraThread() {
        if(isActive())
			--active_thread_num;
    }
    
    void SoraThread::setName(const std::string& _name) {
        name = _name;
    }
    
    std::string SoraThread::getName() const {
        return name;
    }
    
    void SoraThread::setThreadTask(const SoraThreadTask& task) {
        setThreadTaskImpl(task);
    }
    
    bool SoraThread::isActive() const {
        return isActiveImpl();
    }
    
    int32 SoraThread::getActiveThreadNum() const {
        return active_thread_num;
    }
    
    SoraThreadTask SoraThread::getThreadTask() const {
        return getThreadTaskImpl();
    }
    
    int32 SoraThread::start() {
	    ++active_thread_num;
        return startImpl();
    }
    
    int32 SoraThread::startWithTask(const SoraThreadTask& task) {
        return startWithTaskImpl(task);
    }
    
    void SoraThread::join() {
		if(isActive()) {
	        joinImpl();
			--active_thread_num;
		}
    }
	
    void SoraThread::exit() {
		if(isActive()) {
	        exitImpl();
			--active_thread_num;
		};
    }
} // namespace sora
