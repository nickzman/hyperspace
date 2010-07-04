/*
 * Copyright (C) 2004-2010  Terence M. Welsh
 *
 * This file is part of rsUtility.
 *
 * rsUtility is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * rsUtility is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


#ifndef RS_TIMER_H
#define RS_TIMER_H



#ifdef WIN32
#include <windows.h>
#else
#include <stdlib.h>
#include <sys/time.h>
#include <iostream>
#endif


class rsTimer{
public:
	// Time since last call to tick()
	float elapsed_time;
	// Time waited so far by wait()
	float waited_time;
	// Wait() would take a hair too long, if we didn't keep track of the
	// extra time it was spending.  That's what wait_overhead is for.
	float wait_function_overhead;
#ifdef WIN32
	BOOL highResCounterSupported;
	float freq;  // high frequency system counts per second
	LONGLONG curr, prev;
	// for low res timer if high res is unavailable
	DWORD lowResCurr, lowResPrev;
#else
	struct timeval prev_tv;
#endif

	rsTimer(){
		elapsed_time = 0.0f;
		waited_time = 0.0f;
		wait_function_overhead = 0.0f;
#ifdef WIN32
		// init high- and low-res timers
		LARGE_INTEGER n[1];
		highResCounterSupported = QueryPerformanceFrequency(n);
		freq = 1.0f / float(n[0].QuadPart);
		timeBeginPeriod(1);  // make Sleep() and timeGetTime() more accurate
		// get first tick for high- and low-res timers
		QueryPerformanceCounter(n);
		curr = n[0].QuadPart;
		lowResCurr = timeGetTime();
#else
		gettimeofday(&prev_tv, NULL);
#endif
	}

	~rsTimer(){}

	// return time elapsed since last call to tick()
	inline float tick(){
#ifdef WIN32
		if(highResCounterSupported){
			prev = curr;
			LARGE_INTEGER n[1];
			QueryPerformanceCounter(n);
			curr = n[0].QuadPart;
			if(curr >= prev) 
				elapsed_time = float(curr - prev) * freq;
			// else use time from last frame
		}
		else{
			lowResPrev = lowResCurr;
			lowResCurr = timeGetTime();
			if(lowResCurr >= lowResPrev) 
				elapsed_time = float(lowResCurr - lowResPrev) * 0.001f;
			// else use time from last frame
		}
#else
		struct timeval curr_tv;
		gettimeofday(&curr_tv, NULL);
		float elapsed_time = float(curr_tv.tv_sec - prev_tv.tv_sec)
			+ ((curr_tv.tv_usec - prev_tv.tv_usec) * 0.000001f);
		prev_tv = curr_tv;
#endif
		return elapsed_time;
	}

	// Wait until target_time has elapsed, then return.
	// If you call this function after target_time has already elapsed,
	// it will return immediately.  Think of target_time as a lower limit
	// on frame time.
	// Returns actual time elapsed since last call to wait().
	inline float wait(float target_time){
		waited_time = tick();
		float actual_waited_time(waited_time + wait_function_overhead);
		if(actual_waited_time < target_time){
#ifdef WIN32
			Sleep(DWORD(1000.0f * (target_time - actual_waited_time)));
#else
			usleep(useconds_t(1000000.0f * (target_time - actual_waited_time)));
#endif
			waited_time += tick();
		}
		wait_function_overhead += 0.05f * (waited_time - target_time);
		return waited_time;
	}
};



#endif
