/*
 * ----------------------------------------------------------------------------
 * Constantin S. Pan <kvapen@gmail.com> wrote this file. As long as you retain
 * this notice you can do whatever you want with this stuff. If we meet some
 * day, and you think this stuff is worth it, you can buy me a can of Coke in
 * return.
 * 	Constantin S. Pan
 * ----------------------------------------------------------------------------
 */

#include "timing.hpp"

#include <ctime>
#include <cstdio>
#include <cstdlib>

//void sleep_us(unsigned long long us) {
//	// sleep for 'us' microseconds
//	timespec dt;
//	dt.tv_sec = us / 1000000;
//	dt.tv_nsec = 1000 * (us % 1000000);
//	nanosleep(&dt, NULL);
//}
//
//void sleep_s(float s) {
//	// sleep for 's' seconds
//	sleep_us((unsigned long long)(s * 1000000));
//}

#ifdef WIN32
#include <windows.h>
float li2f(LARGE_INTEGER x) {
	float result = ((float)x.HighPart) * 4.294967296E9 + (float)((x).LowPart);
	return result;
}

float now_s() {
	LARGE_INTEGER freq, count;
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&count);
	return li2f(count) / li2f(freq);
}
#else
float now_s() {
	// current time in seconds
	timespec t;
	if (clock_gettime(CLOCK_MONOTONIC, &t) == 0) {
		return t.tv_sec + t.tv_nsec * 1e-9;
	} else {
		printf("Error while clock_gettime()\n");
		exit(0);
	}
}
#endif

