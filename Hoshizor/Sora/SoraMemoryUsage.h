/*
 *  MemoryUsage.h
 *  Sora
 *
 *  Created by Griffin Bu on 5/5/11.
 *  Copyright 2011 Griffin Bu(Project Hoshizor). All rights reserved.
 *
 */

#ifndef SORA_MEMORY_USAGE_H_
#define SORA_MEMORY_USAGE_H_

#include "SoraPlatform.h"

#if defined(OS_IOS) || defined(OS_OSX)

#include <mach/mach.h>

#elif defined(OS_WIN32)

#include <windows.h>
#include <psapi.h>

#pragma comment(lib, "psapi.lib")

#elif defined(OS_LINUX)
// libproc required
// install libproc-dev first
#include <proc/readproc.h>

#endif

namespace sora {
	
	static uint64 getMemoryUsage() {
		// osx kernal memory usage
#if defined(OS_IOS) || defined(OS_OSX)
		
		struct task_basic_info info;
		mach_msg_type_number_t size = sizeof(info);
		kern_return_t kerr = task_info(mach_task_self(),
										TASK_BASIC_INFO,
										(task_info_t)&info,
										&size);
		if(kerr == KERN_SUCCESS)
			return info.resident_size / 1024;
	
		// linux libproc 
#elif defined(OS_LINUX)
		
		struct proc_t usage;
		look_up_our_self(&usage);
		return usage.vm_size;
		
#elif defined(OS_WIN32)
		
		HANDLE hProcess;
		PROCESS_MEMORY_COUNTERS pmc;

		hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
			                   PROCESS_VM_READ,
			                   FALSE, 
			                   GetCurrentProcessId());
		if(NULL != hProcess) {
		    if(GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc))) {
			     return pmc.WorkingSetSize / 1024;
			}

			CloseHandle(hProcess);
		}
		
#endif
		return 0;
	
	}


} // namespace sora
#endif // SORA_MEMORY_USAGE_H_